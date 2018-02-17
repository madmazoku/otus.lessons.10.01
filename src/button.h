#pragma once

#include "shape.h"
#include "shape_io.h"

class IButton {
public:
    virtual std::string tag() = 0;
    virtual void add_to_shapes(Shapes&) = 0;
    virtual bool point_in(const Point&) = 0;
};
using Buttons = std::vector<IButton*>;

class SquareButton : public IButton {
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

    std::string tag_bg() { return tag() + "_bg"; }

    SquareButton() : _o(0, 0), _r(0) {}
    SquareButton(const Point& o, size_t r) : _o(o), _r(r) {}
};

class LineButton : public SquareButton {
public:
    virtual std::string tag() final { return "line"; }
    virtual void add_to_shapes(Shapes& shapes) final {
        SquareButton::add_to_shapes(shapes);

        shapes.push_back(
            new ShapeLine(
                ShapeState(
                    Color(0xff, 0xff, 0xff, 0xff), 
                    Color(0x00, 0x00, 0x00, 0x00), 
                    "line"
                ), 
                Point(_o._x - _r + 5, _o._y - _r + 5), Point(_o._x + _r - 5, _o._y + _r - 5)
            )
        );
    }

    LineButton() : SquareButton() {}
    LineButton(const Point& o, size_t r) : SquareButton(o, r) {}
};

class PolyLineButton : public SquareButton {
public:
    virtual std::string tag() final { return "line"; }
    virtual void add_to_shapes(Shapes& shapes) final {
        SquareButton::add_to_shapes(shapes);

        shapes.push_back(
            new ShapePolyLine(
                ShapeState(
                    Color(0xff, 0xff, 0xff, 0xff), 
                    Color(0x00, 0x00, 0x00, 0x00), 
                    "poly_line"
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

    PolyLineButton() : SquareButton() {}
    PolyLineButton(const Point& o, size_t r) : SquareButton(o, r) {}
};
