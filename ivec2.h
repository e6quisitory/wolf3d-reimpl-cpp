#pragma once

#include "vec2.h"

class ivec2 {
public:
    ivec2(int x, int y): e{x,y} {}
    ivec2(vec2 v): e{static_cast<int>(v.x()), static_cast<int>(v.y())} {}
    ivec2(): e{0,0} {}

    int& x() { return e[0]; };
    int& y() { return e[1]; };
    int x() const { return e[0]; };
    int y() const { return e[1]; };

public:
    int e[2];
};

using ipoint2 = ivec2;

inline std::ostream& operator << (std::ostream& out, const ivec2& v) {
    return out << "[" << v.x() << ", " << v.y() << "]";
}

inline ivec2 operator + (const ivec2& v1, const ivec2& v2) {
    return ivec2(v1.x() + v2.x(), v1.y() + v2.y());
}

inline bool operator == (const ivec2& v1, const ivec2& v2) {
    return (v1.x() == v2.x()) && (v1.y() == v2.y());
}