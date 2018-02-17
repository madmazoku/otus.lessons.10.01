#pragma once

#include <vector>

#include "geometry.h"

/*! \file
    \brief Declaration for Shape descriptor and some definitions
*/

//! \brief supported shape types
enum class ShapeType {
    unknown,
    line,
    poly_line,
    rect,
    circle
};

//! \brief shape info, line color, fill color and tag
struct ShapeState {
//! \brief foreground color (line)
    Color _fg;
//! \brief background color (fill)
    Color _bg;
//! \brief short informational tag
    std::string _tag;

    ShapeState() : _fg(0, 0, 0, 0), _bg(0, 0, 0, 0), _tag("") {}
    ShapeState(const Color& fg, const Color& bg, const std::string& tag = ""): _fg(fg), _bg(bg), _tag(tag) {}
    ShapeState(const ShapeState& state): _fg(state._fg), _bg(state._bg), _tag(state._tag) {}
};

//! \brief base shape descriptor
class IShape
{
public:
//! \brief shape descriptor type
    virtual ShapeType type() = 0;
};
//! \brief sequence of shape descriptors
using Shapes = std::vector<IShape*>;

// specific shapes

//! \brief shape with state
class Shape : public IShape
{
public:
    ShapeState _state;

    Shape() : _state(Color(0, 0, 0, 0), Color(0, 0, 0, 0)) {}
    Shape(const ShapeState& state) : _state(state) {}
};

//! \brief shape descriptor which represent line connecting two points
class ShapeLine : public Shape
{
public:
    ShapeType type() final { return ShapeType::line; }

    Point _p1;
    Point _p2;

    ShapeLine() : Shape(), _p1(0, 0), _p2(0, 0) {}
    ShapeLine(const ShapeState& state, const Point& p1, const Point& p2) : Shape(state), _p1(p1), _p2(p2) {}
};

//! \brief shape descriptor which represent polyline connecting several point, may be closed and filled
class ShapePolyLine : public Shape
{
public:
    ShapeType type() final { return ShapeType::poly_line; }

    Points _pts;
    bool _closed;

    ShapePolyLine() : Shape() {}
    ShapePolyLine(const ShapeState& state, const Points& pts, bool closed) : Shape(state), _pts(pts), _closed(closed) {}
};

//! \brief shape descriptor which represent rect
class ShapeRect : public Shape
{
public:
    ShapeType type() final { return ShapeType::rect; }

    Rect _r;
    ShapeRect() : Shape(), _r(0, 0, 0, 0) {}
    ShapeRect(const ShapeState& state, const Rect& r) : Shape(state), _r(r) {}
};

//! \brief shape descriptor which represent circle with origin point and radius
class ShapeCircle : public Shape
{
public:
    ShapeType type() final { return ShapeType::circle; }

    Point _o;
    size_t _r;

    ShapeCircle() : Shape(), _o(0, 0), _r(0) {}
    ShapeCircle(const ShapeState& state, const Point& o, int32_t r) : Shape(state), _o(o), _r(r) {}
};
