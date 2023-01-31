#pragma once

#include <iostream>
#include <cmath>

#include "global.h"

template<typename T>
class Vec2D {
public:
    Vec2D(const T& x, const T& y): e{x, y} {}
    Vec2D(const T& a): e{a, a} {}
    Vec2D(): e{static_cast<T>(0), static_cast<T>(0)} {}

    template<typename U>
    Vec2D(Vec2D<U> v): e{static_cast<T>(v.x()), static_cast<T>(v.y())} {}

    T& x() { return e[0]; };
    T& y() { return e[1]; };
    T x() const { return e[0]; };
    T y() const { return e[1]; };

    T& operator [] (const int& index) {
        return e[index];
    }

    T operator [] (const int& index) const {
        return e[index];
    }

    Vec2D<T>& operator += (const Vec2D<T>& v) {
        e[0] += v.e[0];
        e[1] += v.e[1];
        return *this;
    }

    Vec2D<T>& operator -= (const Vec2D<T>& v) {
        e[0] -= v.e[0];
        e[1] -= v.e[1];
        return *this;
    }

    Vec2D<T>& operator *= (const double& d) {
        e[0] *= d;
        e[1] *= d;
        return *this;
    }

    bool operator == (const Vec2D& v) {
        return e[0] == v.e[0] && e[1] == v.e[1] ? true : false;
    }

    double length_squared() const {
        return e[0]*e[0] + e[1]*e[1];
    }

    double length() const {
        return std::sqrt(length_squared());
    }

    Vec2D<double> rotate(const double& radians) const {
        return Vec2D<double>(e[0] * std::cos(radians) - e[1] * std::sin(radians), e[0] * std::sin(radians) + e[1] * std::cos(radians));
    }

public:
    T e[2];
};

/*
================================
 Aliases
================================
*/

typedef Vec2D<double> Point2;
typedef Vec2D<int> iPoint2;
typedef Vec2D<double> Vec2;
typedef Vec2D<int> iVec2;

/*
================================
 Insertion into output stream
================================
*/

template<typename T>
inline std::ostream& operator << (std::ostream& out, const Vec2D<T>& v) {
    return out << "[" << v.x() << ", " << v.y() << "]";
}

/*
================================
 Addition
================================
*/

template<typename T>
inline Vec2D<T> operator + (const Vec2D<T>& v1, const Vec2D<T>& v2) {
    return Vec2D<T>(v1.x() + v2.x(), v1.y() + v2.y());
}

/*
================================
 Subtraction
================================
*/

template<typename T>
inline Vec2D<T> operator - (const Vec2D<T>& v1, const Vec2D<T>& v2) {
    return v1 + (-v2);
}

/*
================================
 Multiplication
================================
*/

template<typename T>
inline Vec2D<T> operator * (const Vec2D<T>& v, const double& t) {
    return Vec2D<T>(t * v.x(), t * v.y());
}

template<typename T>
inline Vec2D<T> operator * (const double& t, const Vec2D<T>& v) {
    return Vec2D<T>(t * v.x(), t * v.y());
}

/*
================================
 Negative
================================
*/

template<typename T>
inline Vec2D<T> operator - (const Vec2D<T>& v) {
    return Vec2D<T>(-v.x(), -v.y());
}

/*
================================
 Division by scalar
================================
*/

template<typename T>
inline Vec2D<T> operator / (const Vec2D<T>& v, const double& t) {
    return Vec2D<T>(v.x() / t, v.y() / t);
}

/*
================================
 Misc.
================================
*/

template<typename T>
Vec2D<T> unit_vector(const Vec2D<T>& v) {
    return v / v.length();
}