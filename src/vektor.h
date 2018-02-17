#pragma once

#include "event.h"
#include "shape.h"
#include "shape_io.h"
#include <map>

#include "button.h"

enum class EventHandlerType {
    main,
    poly_line
};
using EventHandlers = std::map<EventHandlerType, IEventHandler*>;

struct VektorState {
    Point _size;
    ShapeState _shape_state;
    Shapes _interface_shapes;
    Shapes _scene_shapes;
    Shapes _temporary_shapes;

    EventHandlers _event_handlers;
    Buttons _buttons;

    void free_shapes(Shapes& shapes)
    {
        for(auto s : shapes)
            delete s;
        shapes.clear();
    }

    VektorState(const Point& size) : _size(size), _shape_state(Color(0xff, 0xff, 0xff, 0xff), Color(0x00, 0x00, 0x00, 0x00), "")
    {
        _buttons.push_back(new ButtonLine(Point(30, 30), 25));
        _buttons.push_back(new ButtonPolyLine(Point(90, 30), 25));
        _buttons.push_back(new ButtonRect(Point(150, 30), 25));
        _buttons.push_back(new ButtonCircle(Point(210, 30), 25));
        _buttons.push_back(new ButtonWrite(Point(270, 30), 25));
        _buttons.push_back(new ButtonRead(Point(330, 30), 25));

        _interface_shapes.push_back(
            new ShapeRect(
                ShapeState(
                    Color(0xff, 0xff, 0xff, 0x00),
                    Color(0x00, 0x00, 0x00, 0xff),
                    "clear_window"
                ),
                Rect(0, 0, _size._x, _size._y)
            )
        );
        for(auto b : _buttons)
            b->add_to_shapes(_interface_shapes);
    }
    ~VektorState()
    {
        for(auto b : _buttons)
            delete b;
        _buttons.clear();
        for(auto eh : _event_handlers)
            delete eh.second;
        _event_handlers.clear();
        free_shapes(_interface_shapes);
        free_shapes(_scene_shapes);
    }

    bool render(IWindow* window)
    {
        return !(window->render(_interface_shapes) && window->render(_scene_shapes) && window->render(_temporary_shapes));
    }

    Shape* by_tag(const Shapes& shapes, const std::string tag)
    {
        for(auto s : shapes) {
            Shape* shape = dynamic_cast<Shape*>(s);
            if(shape->_state._tag == tag)
                return shape;
        }
        return nullptr;
    }
};

class MainEventHandler : public IEventHandler
{
public:
    VektorState& _vektor_state;

    virtual IEventHandler* process(IEvent* ev) final {
        auto console = spdlog::get("console");
        switch(ev->type())
        {
        case EventType::mouse_button: {
            EventMouseButton* evMB = dynamic_cast<EventMouseButton*>(ev);

            if(evMB->_cursor._y > 60)
                return this;

            if(!evMB->_up)
                return this;

            for(auto b : _vektor_state._buttons)
                if(b->point_in(evMB->_cursor))
                    if(b->tag() == "poly_line")
                        return _vektor_state._event_handlers[EventHandlerType::poly_line];
                    else if(b->tag() == "write")
                        std::cout << _vektor_state._scene_shapes;
                    else if(b->tag() == "read") {
                        _vektor_state._scene_shapes.clear();
                        std::cin >> _vektor_state._scene_shapes;
                    }

            return this;
        }
        case EventType::mouse_move: {
            EventMouseMove* evMM = dynamic_cast<EventMouseMove*>(ev);

            for(auto b : _vektor_state._buttons) {
                if(b->tag() == "poly_line" || b->tag() == "read" || b->tag() == "write") {
                    Shape* bg = _vektor_state.by_tag(_vektor_state._interface_shapes, b->tag_bg());
                    if(bg)
                        if(b->point_in(evMM->_cursor)) {
                            bg->_state._bg._r = 0x00;
                            bg->_state._bg._b = 0x00;
                        } else {
                            bg->_state._bg._r = 0x7f;
                            bg->_state._bg._b = 0x7f;
                        }
                }
            }

            return this;
        }
        case EventType::quit:
            console->info("event: quit");
            return nullptr;
        case EventType::key: {
            EventKey* evKey = dynamic_cast<EventKey*>(ev);
            console->info("event: key: {0} {1}", evKey->_vk, evKey->_up ? "up" : "down");
            if(evKey->_up)
                switch(evKey->_vk) {
                case 41: // escape scan code
                    return nullptr;
                case 19: // 'P' scan code
                    return _vektor_state._event_handlers[EventHandlerType::poly_line];
                case 26: // 'W' scan code
                    std::cout << _vektor_state._scene_shapes;
                    return this;
                case 21: // 'R' scan code
                    std::cin >> _vektor_state._scene_shapes;
                    return this;
                }
            return this;
        }
        default:
            return this;
        }
    }

    virtual void enter() final {
    }
    virtual void leave() final {
        Shape* poly_bg = _vektor_state.by_tag(_vektor_state._interface_shapes, "poly_line_bg");
        poly_bg->_state._bg._g = 0x7f;
    }

    MainEventHandler(VektorState& vektor_state) : _vektor_state(vektor_state) { }
};

class PolyLineEventHandler : public IEventHandler
{
public:
    VektorState& _vektor_state;

    std::vector<ShapeCircle*> _points;
    ShapePolyLine* _poly_line;
    size_t _current_point;
    bool _store_shape;

    virtual IEventHandler* process(IEvent* ev) final {
        auto console = spdlog::get("console");
        switch(ev->type())
        {
        case EventType::mouse_button: {
            EventMouseButton* evMB = dynamic_cast<EventMouseButton*>(ev);
            if(evMB->_cursor._y < 60)
                return this;

            if(evMB->_mb == EventMouseButton::MouseButton::Left)
                if(!evMB->_up && _poly_line == nullptr) {
                    console->info("new poly_line");
                    _poly_line = new ShapePolyLine;
                    _poly_line->_state = _vektor_state._shape_state;
                    _vektor_state._temporary_shapes.push_back(_poly_line);

                    add_point(evMB->_cursor);
                    add_point(evMB->_cursor);
                    _current_point = _points.size() - 1;
                } else {
                    _current_point = _points.size();
                    if(!evMB->_up) {
                        for(size_t i = 0; i < _points.size(); ++i)
                            if(std::abs(_points[i]->_o._x - evMB->_cursor._x) <= 5
                               && std::abs(_points[i]->_o._y - evMB->_cursor._y) <= 5
                              ) {
                                console->info("found point {0}", i);
                                _current_point = i;
                                _points[_current_point]->_o = evMB->_cursor;
                                _poly_line->_pts[_current_point] = evMB->_cursor;
                                break;
                            }
                        if(_current_point == _points.size()) {
                            console->info("add point {0}", _points.size());
                            add_point(evMB->_cursor);
                            _current_point = _points.size() - 1;
                        }
                    }
                }
            return this;
        }
        case EventType::mouse_move: {
            EventMouseMove* evMM = dynamic_cast<EventMouseMove*>(ev);
            if(evMM->_cursor._y < 60)
                return this;
            if(_current_point < _points.size()) {
                _points[_current_point]->_o = evMM->_cursor;
                _poly_line->_pts[_current_point] = evMM->_cursor;
            }
            return this;
        }
        case EventType::key: {
            EventKey* evKey = dynamic_cast<EventKey*>(ev);
            console->info("event: key: {0} {1}", evKey->_vk, evKey->_up ? "up" : "down");
            if(evKey->_up)
                switch(evKey->_vk) {
                case 41: // escape scan code
                    _store_shape = false;
                    return _vektor_state._event_handlers[EventHandlerType::main];
                case 40: // return scan code
                    _store_shape = true;
                    return _vektor_state._event_handlers[EventHandlerType::main];
                }
            return this;
        }
        default:
            return this;
        }
    }

    virtual void enter() final {
        Shape* poly_bg = _vektor_state.by_tag(_vektor_state._interface_shapes, "poly_line_bg");
        poly_bg->_state._bg._r = 0x7f;
        poly_bg->_state._bg._g = 0x00;
        poly_bg->_state._bg._b = 0x00;
    }
    virtual void leave() final {
        Shape* poly_bg = _vektor_state.by_tag(_vektor_state._interface_shapes, "poly_line_bg");
        poly_bg->_state._bg._r = 0x7f;
        poly_bg->_state._bg._g = 0x7f;
        poly_bg->_state._bg._b = 0x7f;

        _vektor_state._temporary_shapes.clear();
        if(_poly_line != nullptr)
        {
            if(_store_shape)
                _vektor_state._scene_shapes.push_back(_poly_line);
            else
                delete _poly_line;
            _poly_line = nullptr;
        }
        cleanup();
    }

    void add_point(const Point& p)
    {
        _poly_line->_pts.push_back(p);
        ShapeCircle* shape = new ShapeCircle(
            ShapeState(Color(0xff, 0xff, 0xff, 0xff), Color(0x00, 0x00, 0x00, 0x00), ""),
            p, 5
        );
        _points.push_back(shape);
        _vektor_state._temporary_shapes.push_back(shape);
    }

    void cleanup()
    {
        for(auto sc : _points)
            delete sc;
        _points.clear();
        if(_poly_line)
            delete _poly_line;
        _current_point = 0;
        _store_shape = false;
    }

    PolyLineEventHandler(VektorState& vektor_state) : _vektor_state(vektor_state), _poly_line(nullptr), _current_point(0), _store_shape(false) { }
    ~PolyLineEventHandler()
    {
        cleanup();
    }
};

