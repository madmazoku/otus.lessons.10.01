#pragma once

#include "shape.h"

std::ostream& operator<<(std::ostream& out, const Point& p) {
    out << p._x << ' ' << p._y;
    return out;
}

std::ostream& operator<<(std::ostream& out, const Points& pts) {
    out << pts.size();
    for(auto p : pts)
        out << ' ' << p;
    return out;
}

std::ostream& operator<<(std::ostream& out, const Rect& r) {
    out << r._lt << ' ' << r._rb;
    return out;
}

std::ostream& operator<<(std::ostream& out, const Color& c) {
    out << (int)c._r << ' ' << (int)c._g << ' ' << (int)c._b << ' ' << (int)c._a;
    return out;
}

std::ostream& operator<<(std::ostream& out, const ShapeState& state) {
    out << state._fg << ' ' << state._bg << ' ' << '"' << state._tag << '"';
    return out;
}

std::ostream& operator<<(std::ostream& out, const ShapeLine& shape) {
    out << shape._state << std::endl;
    out << shape._p1 << ' ' << shape._p2 << std::endl;
}

std::ostream& operator<<(std::ostream& out, const ShapePolyLine& shape) {
    out << shape._state << std::endl;
    out << shape._pts << ' ' << shape._closed << std::endl;
    return out;
}

std::ostream& operator<<(std::ostream& out, const ShapeRect& shape) {
    out << shape._state << std::endl;
    out << shape._r << std::endl;
    return out;
}

std::ostream& operator<<(std::ostream& out, const ShapeCircle& shape) {
    out << shape._state << std::endl;
    out << shape._o << ' ' << shape._r << std::endl;
    return out;
}

std::ostream& operator<<(std::ostream& out, const Shapes& shapes) {
    out << shapes.size() << std::endl;
    for(auto s : shapes) {
        switch(s->type()) {
            case ShapeType::line:
                out << "line" << std::endl;
                out << *(dynamic_cast<ShapeLine*>(s));
                break;
            case ShapeType::poly_line:
                out << "poly_line" << std::endl;
                out << *(dynamic_cast<ShapePolyLine*>(s));
                break;
            case ShapeType::rect:
                out << "rect" << std::endl;
                out << *(dynamic_cast<ShapeRect*>(s));
                break;
            case ShapeType::circle:
                out << "circle" << std::endl;
                out << *(dynamic_cast<ShapeCircle*>(s));
                break;
        }
    }
    return out;
}

std::istream& operator>>(std::istream& in, Point& p) {
    in >> p._x >> p._y;
    return in;
}

std::istream& operator>>(std::istream& in, Points& pts) {
    size_t n;
    in >> n;
    for(size_t i = 0; i < n; ++i) {
        Point p;
        in >> p;
        pts.push_back(p);
    }
    return in;
}

std::istream& operator>>(std::istream& in, Rect& r) {
    in >> r._lt >> r._rb;
    return in;
}

std::istream& operator>>(std::istream& in, Color& c) {
    int r, g, b, a;
    in >> r >> g >> b >> a;
    c._r = r;
    c._g = g;
    c._b = b;
    c._a = a;
    return in;
}

std::istream& operator>>(std::istream& in, ShapeState& state) {
    std::string tag;
    in >> state._fg >> state._bg >> tag;
    state._tag = tag.substr(1, tag.length() - 2);
    return in;
}

std::istream& operator>>(std::istream& in, ShapeLine& shape) {
    in >> shape._state;
    in >> shape._p1 >> shape._p2;
}

std::istream& operator>>(std::istream& in, ShapePolyLine& shape) {
    in >> shape._state;
    in >> shape._pts >> shape._closed;
    return in;
}

std::istream& operator>>(std::istream& in, ShapeRect& shape) {
    in >> shape._state;
    in >> shape._r;
    return in;
}

std::istream& operator>>(std::istream& in, ShapeCircle& shape) {
    in >> shape._state;
    in >> shape._o >> shape._r;
    return in;
}

std::istream& operator>>(std::istream& in, Shapes& shapes) {
    size_t n;
    in >> n;
    for(size_t i = 0; i < n; ++i) {
        std::string type;
        in >> type;
        if(type == "line") {
            ShapeLine* shape = new ShapeLine;
            in >> *shape;
            shapes.push_back(shape);
        } else if(type == "poly_line") {
            ShapePolyLine* shape = new ShapePolyLine;
            in >> *shape;
            shapes.push_back(shape);
        } else if(type == "rect") {
            ShapeRect* shape = new ShapeRect;
            in >> *shape;
            shapes.push_back(shape);
        } else if(type == "circle") {
            ShapeCircle* shape = new ShapeCircle;
            in >> *shape;
            shapes.push_back(shape);
        }
    }
    return in;
}

