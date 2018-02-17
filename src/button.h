#pragma once

#include "shape.h"
#include "shape_io.h"

class IButton {
public:
    virtual std::string tag() = 0;
    virtual std::string tag_bg() = 0;
    virtual void add_to_shapes(Shapes&) = 0;
    virtual bool point_in(const Point&) = 0;
};
using Buttons = std::vector<IButton*>;

class ButtonSquare : public IButton {
public:

    virtual bool point_in(const Point& p) final {
        return std::abs(p._x - _o._x) < _r && std::abs(p._y - _o._y) < _r;
    }
    virtual void add_to_shapes(Shapes& shapes) override {
        shapes.push_back(
            new ShapeRect(
                ShapeState(
                    Color(0xff, 0xff, 0xff, 0xff), 
                    Color(0x7f, 0x7f, 0x7f, 0xff), 
                    tag_bg()
                ), 
                Rect(_o._x - _r, _o._y - _r, _o._x + _r, _o._y + _r)
            )
        );
    }

    Point _o;
    size_t _r;

    std::string tag_bg() final { return tag() + "_bg"; }

    ButtonSquare() : _o(0, 0), _r(0) {}
    ButtonSquare(const Point& o, size_t r) : _o(o), _r(r) {}
};

class ButtonLine : public ButtonSquare {
public:
    virtual std::string tag() final { return "line"; }
    virtual void add_to_shapes(Shapes& shapes) final {
        ButtonSquare::add_to_shapes(shapes);

        shapes.push_back(
            new ShapeLine(
                ShapeState(
                    Color(0xff, 0xff, 0xff, 0xff), 
                    Color(0x00, 0x00, 0x00, 0x00), 
                    tag()
                ), 
                Point(_o._x - _r + 5, _o._y - _r + 5), Point(_o._x + _r - 5, _o._y + _r - 5)
            )
        );
    }

    ButtonLine() : ButtonSquare() {}
    ButtonLine(const Point& o, size_t r) : ButtonSquare(o, r) {}
};

class ButtonPolyLine : public ButtonSquare {
public:
    virtual std::string tag() final { return "poly_line"; }
    virtual void add_to_shapes(Shapes& shapes) final {
        ButtonSquare::add_to_shapes(shapes);

        shapes.push_back(
            new ShapePolyLine(
                ShapeState(
                    Color(0xff, 0xff, 0xff, 0xff), 
                    Color(0x00, 0x00, 0x00, 0x00), 
                    tag()
                ), 
                Points{
                    Point(_o._x - _r + 5, _o._y - _r + 5), 
                    Point(_o._x + _r - 5, _o._y + _r - 5),
                    Point(_o._x - _r + 5, _o._y + _r - 5),
                    Point(_o._x + _r - 5, _o._y - _r + 5),
                }, false
            )
        );
    }

    ButtonPolyLine() : ButtonSquare() {}
    ButtonPolyLine(const Point& o, size_t r) : ButtonSquare(o, r) {}
};

class ButtonRect : public ButtonSquare {
public:
    virtual std::string tag() final { return "rect"; }
    virtual void add_to_shapes(Shapes& shapes) final {
        ButtonSquare::add_to_shapes(shapes);

        shapes.push_back(
            new ShapeRect(
                ShapeState(
                    Color(0xff, 0xff, 0xff, 0xff), 
                    Color(0x00, 0x00, 0x00, 0x00), 
                    tag()
                ), 
                Rect(Point(_o._x - _r + 5, _o._y - _r + 5), Point(_o._x + _r - 5, _o._y + _r - 5))
            )
        );
    }

    ButtonRect() : ButtonSquare() {}
    ButtonRect(const Point& o, size_t r) : ButtonSquare(o, r) {}
};

class ButtonCircle : public ButtonSquare {
public:
    virtual std::string tag() final { return "circle"; }
    virtual void add_to_shapes(Shapes& shapes) final {
        ButtonSquare::add_to_shapes(shapes);

        shapes.push_back(
            new ShapeCircle(
                ShapeState(
                    Color(0xff, 0xff, 0xff, 0xff), 
                    Color(0x00, 0x00, 0x00, 0x00), 
                    tag()
                ), 
                _o, _r - 5
            )
        );
    }

    ButtonCircle() : ButtonSquare() {}
    ButtonCircle(const Point& o, size_t r) : ButtonSquare(o, r) {}
};

class ButtonWrite : public ButtonSquare {
public:
    virtual std::string tag() final { return "write"; }
    virtual void add_to_shapes(Shapes& shapes) final {
        ButtonSquare::add_to_shapes(shapes);

        shapes.push_back(
            new ShapePolyLine(
                ShapeState(
                    Color(0xff, 0xff, 0xff, 0xff), 
                    Color(0x00, 0x00, 0x00, 0x00), 
                    tag()
                ), 
                Points{
                    Point(_o._x - _r + 5, _o._y - _r + 5), 
                    Point(_o._x - _r + 5, _o._y + _r - 5), 
                    Point(_o._x, _o._y), 
                    Point(_o._x + _r - 5, _o._y + _r - 5), 
                    Point(_o._x + _r - 5, _o._y - _r + 5), 
                }, false
            )
        );
    }

    ButtonWrite() : ButtonSquare() {}
    ButtonWrite(const Point& o, size_t r) : ButtonSquare(o, r) {}
};

class ButtonRead : public ButtonSquare {
public:
    virtual std::string tag() final { return "read"; }
    virtual void add_to_shapes(Shapes& shapes) final {
        ButtonSquare::add_to_shapes(shapes);

        shapes.push_back(
            new ShapePolyLine(
                ShapeState(
                    Color(0xff, 0xff, 0xff, 0xff), 
                    Color(0x00, 0x00, 0x00, 0x00), 
                    tag()
                ), 
                Points{
                    Point(_o._x - _r + 5, _o._y + _r - 5), 
                    Point(_o._x - _r + 5, _o._y - _r + 5), 
                    Point(_o._x + _r - 5, _o._y - _r + 5), 
                    Point(_o._x + _r - 5, _o._y), 
                    Point(_o._x - _r + 5, _o._y), 
                    Point(_o._x + _r - 5, _o._y + _r - 5), 
                }, false
            )
        );
    }

    ButtonRead() : ButtonSquare() {}
    ButtonRead(const Point& o, size_t r) : ButtonSquare(o, r) {}
};
