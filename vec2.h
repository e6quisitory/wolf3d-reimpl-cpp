#pragma once

#include <iostream>
#include <cmath>

class vec2 {
public:
    vec2(double x, double y): e{x,y} {}
    vec2(double a): e{a,a} {}
    vec2(): e{0,0} {}

    inline double& x() { return e[0]; };
    inline double& y() { return e[1]; };
    inline double x() const { return e[0]; };
    inline double y() const { return e[1]; };

    double& operator [] (int index) {
        return e[index];
    }

    double operator [] (int index) const {
        return e[index];
    }

    vec2& operator += (const vec2& v) {
        e[0] += v.e[0];
        e[1] += v.e[1];
        return *this;
    }

    vec2& operator -= (const vec2& v) {
        e[0] -= v.e[0];
        e[1] -= v.e[1];
        return *this;
    }

    vec2& operator *= (const double& d) {
        e[0] *= d;
        e[1] *= d;
        return *this;
    }

    bool operator == (const vec2& v) {
        return e[0] == v.e[0] && e[1] == v.e[1] ? true : false;
    }

    double length_squared() const {
        return e[0]*e[0] + e[1]*e[1];
    }

    double length() const {
        return std::sqrt(length_squared());
    }

    vec2 rotate(double radians) const {
        return vec2(e[0]*std::cos(radians)-e[1]*std::sin(radians), e[0]*std::sin(radians)+e[1]*std::cos(radians));
    }

public:
    double e[2];
};

using point2 = vec2;

inline std::ostream& operator << (std::ostream& out, const vec2& v) {
    return out << "[" << v.x() << ", " << v.y() << "]";
}

inline vec2 operator + (const vec2& v1, const vec2& v2) {
    return vec2(v1.x() + v2.x(), v1.y() + v2.y());
}

inline vec2 operator - (const vec2& v) {
    return vec2(-v.x(), -v.y());
}

inline vec2 operator - (const vec2& v1, const vec2& v2) {
    return v1 + (-v2);
}

inline vec2 operator * (const vec2& v, const double t) {
    return vec2(t*v.x(), t*v.y());
}

inline vec2 operator * (const double t, const vec2& v) {
    return vec2(t*v.x(), t*v.y());
}

inline vec2 operator / (const vec2& v, const double t) {
    return vec2(v.x()/t, v.y()/t);
}

vec2 unit_vector(const vec2& v) {
    return v / v.length();
}