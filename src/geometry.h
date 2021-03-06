#pragma once

#include <vector>
#include <algorithm>

/*! \file
    \brief Utility structures for 2d geometry
*/

//! \brief Point in 2d space
struct Point {
    int32_t _x;
    int32_t _y;

    Point() : _x(0), _y(0) {}
    Point(size_t x, size_t y): _x(x), _y(y) {}
    Point(const Point& p): _x(p._x), _y(p._y) {}
};
//! \brief Sequence of points
using Points = std::vector<Point>;

//! \brief Rectangle defined by left top and right bottom corners
struct Rect {
//! \brief left top point
    Point _lt;
//! \brief right bottom point
    Point _rb;

    Rect() : _lt(0, 0), _rb(0, 0) {}
    Rect(size_t x1, size_t y1, size_t x2, size_t y2): _lt(std::min(x1, x2), std::min(y1, y2)), _rb(std::max(x1, x2), std::max(y1, y2)) {}
    Rect(const Point &p1, const Point &p2): _lt(std::min(p1._x, p2._x), std::min(p1._y, p2._y)), _rb(std::max(p1._x, p2._x), std::max(p1._y, p2._y)) {}

    Point size()
    {
        return Point(_rb._x - _lt._x, _rb._y - _lt._y);
    }
};

//! \brief Color defined by Red, Green, Blue levels, plus transparency alpha
struct Color {
//! \brief red
    uint8_t _r;
//! \brief green
    uint8_t _g;
//! \brief blue
    uint8_t _b;
//! \brief alpha (transparency)
    uint8_t _a;

    Color() : _r(0), _g(0), _b(0), _a(0) {}
    Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 0) : _r(r), _g(g), _b(b), _a(a) {}
    Color(const Color& c) : _r(c._r), _g(c._g), _b(c._b), _a(c._a) {}
};
