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

    void free_shapes(Shapes& shapes) {
        for(auto s : shapes)
            delete s;
        shapes.clear();
    }

    VektorState(const Point& size) : _size(size), _shape_state(Color(0xff, 0xff, 0xff, 0xff), Color(0x00, 0x00, 0x00, 0x00), "") {
        _buttons.push_back(new LineButton(Point(30, 30), 25));
        _buttons.push_back(new PolyLineButton(Point(90, 30), 25));

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
        // for(auto b : _buttons)
        //     b->add_to_shapes(_interface_shapes);
        _interface_shapes.push_back(
            new ShapeRect(
                ShapeState(
                    Color(0xff, 0xff, 0xff, 0xff), 
                    Color(0x7f, 0x7f, 0x7f, 0xff), 
                    "line_bg"
                ), 
                Rect(5, 5, 55, 55)
            )
        );
        _interface_shapes.push_back(
            new ShapeLine(
                ShapeState(
                    Color(0xff, 0xff, 0xff, 0xff), 
                    Color(0x00, 0x00, 0x00, 0x00), 
                    "line"
                ), 
                Point(10, 10), Point(50, 50)
            )
        );
        _interface_shapes.push_back(
            new ShapeRect(
                ShapeState(
                    Color(0xff, 0xff, 0xff, 0xff), 
                    Color(0x7f, 0x7f, 0x7f, 0xff), 
                    "poly_line_bg"
                ), 
                Rect(60, 5, 110, 55)
            )
        );
        _interface_shapes.push_back(
            new ShapePolyLine(
                ShapeState(
                    Color(0xff, 0xff, 0xff, 0xff), 
                    Color(0x00, 0x00, 0x00, 0x00), 
                    "poly_line"
                ), 
                Points{
                    Point(65, 10), 
                    Point(105, 50),
                    Point(105, 10),
                    Point(65, 50),
                }, false
            )
        );
        _interface_shapes.push_back(
            new ShapeRect(
                ShapeState(
                    Color(0xff, 0xff, 0xff, 0xff), 
                    Color(0x7f, 0x7f, 0x7f, 0xff), 
                    "rect_bg"
                ), 
                Rect(115, 5, 165, 55)
            )
        );
        _interface_shapes.push_back(
            new ShapeRect(
                ShapeState(
                    Color(0xff, 0xff, 0xff, 0xff), 
                    Color(0x00, 0x00, 0x00, 0x00), 
                    "rect"
                ), 
                Rect(120, 10, 160, 50)
            )
        );
        _interface_shapes.push_back(
            new ShapeRect(
                ShapeState(
                    Color(0xff, 0xff, 0xff, 0xff), 
                    Color(0x7f, 0x7f, 0x7f, 0xff), 
                    "circle_bg"
                ), 
                Rect(170, 5, 220, 55)
            )
        );
        _interface_shapes.push_back(
            new ShapeCircle(
                ShapeState(
                    Color(0xff, 0xff, 0xff, 0xff), 
                    Color(0x00, 0x00, 0x00, 0x00), 
                    "circle"
                ), 
                Point(195, 30), 20
            )
        );
    }
    ~VektorState() {
        for(auto b : _buttons)
            delete b;
        _buttons.clear();
        for(auto eh : _event_handlers) 
            delete eh.second;
        _event_handlers.clear();
        free_shapes(_interface_shapes);
        free_shapes(_scene_shapes);
    }

    bool render(IWindow* window) {
       return !(window->render(_interface_shapes) && window->render(_scene_shapes) && window->render(_temporary_shapes)); 
    }

    Shape* by_tag(const Shapes& shapes, const std::string tag) {
        for(auto s : shapes) {
            Shape* shape = dynamic_cast<Shape*>(s);
            if(shape->_state._tag == tag)
                return shape;
        }
        return nullptr;
    }
};

class MainEventHandler : public IEventHandler {
public:
    VektorState& _vektor_state;

    virtual IEventHandler* process(IEvent* ev) final {
        auto console = spdlog::get("console");
        switch(ev->type()) {
            case EventType::mouse_button:
            {
                EventMouseButton* evMB = dynamic_cast<EventMouseButton*>(ev);

                if(std::abs(evMB->_cursor._x - 85) < 25 && std::abs(evMB->_cursor._y - 30) < 25)
                    return _vektor_state._event_handlers[EventHandlerType::poly_line];

                return this;
            }
            case EventType::mouse_move:
            {
                EventMouseMove* evMM = dynamic_cast<EventMouseMove*>(ev);

                Shape* poly_bg = _vektor_state.by_tag(_vektor_state._interface_shapes, "poly_line_bg");
                if(std::abs(evMM->_cursor._x - 85) < 25 && std::abs(evMM->_cursor._y - 30) < 25)
                    poly_bg->_state._bg._g = 0xff;
                else
                    poly_bg->_state._bg._g = 0x7f;

                return this;
            }
            case EventType::quit:
                console->info("event: quit");
                return nullptr;
            case EventType::key:
            {
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

class PolyLineEventHandler : public IEventHandler {
public:
    VektorState& _vektor_state;

    std::vector<ShapeCircle*> _points;
    ShapePolyLine* _poly_line;
    size_t _current_point;
    bool _store_shape;

    virtual IEventHandler* process(IEvent* ev) final {
        auto console = spdlog::get("console");
        switch(ev->type()) {
            case EventType::mouse_button:
            {
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
            case EventType::mouse_move:
            {
                EventMouseMove* evMM = dynamic_cast<EventMouseMove*>(ev);
                if(evMM->_cursor._y < 60)
                    return this;
                if(_current_point < _points.size()) {
                    _points[_current_point]->_o = evMM->_cursor;
                    _poly_line->_pts[_current_point] = evMM->_cursor;
                }
                return this;
            }
            case EventType::key:
            {
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
        poly_bg->_state._bg._r = 0xff;
    }
    virtual void leave() final {
        Shape* poly_bg = _vektor_state.by_tag(_vektor_state._interface_shapes, "poly_line_bg");
        poly_bg->_state._bg._r = 0x7f;

        _vektor_state._temporary_shapes.clear();
        if(_poly_line != nullptr) {
            if(_store_shape)
                _vektor_state._scene_shapes.push_back(_poly_line);
            else
                delete _poly_line;
            _poly_line = nullptr;
        }
        cleanup();
    }

    void add_point(const Point& p) {
        _poly_line->_pts.push_back(p);
        ShapeCircle* shape = new ShapeCircle(
                ShapeState(Color(0xff, 0xff, 0xff, 0xff), Color(0x00, 0x00, 0x00, 0x00), ""),
                p, 5
            );
        _points.push_back(shape);
        _vektor_state._temporary_shapes.push_back(shape);
    }

    void cleanup() {
        for(auto sc : _points)
            delete sc;
        _points.clear();
        if(_poly_line)
            delete _poly_line;
        _current_point = 0;
        _store_shape = false;
    }

    PolyLineEventHandler(VektorState& vektor_state) : _vektor_state(vektor_state), _poly_line(nullptr), _current_point(0), _store_shape(false) { }
    ~PolyLineEventHandler() { cleanup(); }
};

