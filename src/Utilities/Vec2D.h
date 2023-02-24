#pragma once

#include <iostream>
#include <cmath>
#include <cstdlib>
#include <ctime>

#include "Conventions.h"
#include "MiscMath/MiscMath.h"

/*
=====================================================
    Vec2D (used as 2D int or double vector)
=====================================================
*/

template<typename T>
class Vec2D {
public:
    T e[2];

public:
    /*
    =================================================
        Constructors
    =================================================
    */

    Vec2D(const T x, const T y) : e{x, y} {}
    Vec2D(const T a)            : e{a, a} {}
    Vec2D()                     : e{static_cast<T>(0), static_cast<T>(0)} {}

    template<typename U>
    Vec2D(const Vec2D<U>& v)    : e{static_cast<T>(v.x()), static_cast<T>(v.y())} {}

    Vec2D(const xDir_t xDir)    : e{static_cast<int>(xDir), 0} {}
    Vec2D(const yDir_t yDir)    : e{0, static_cast<int>(yDir)} {}

    /*
    =================================================
        Getters / Setters
    =================================================
    */

    T& x()         { return e[0]; }
    T& y()         { return e[1]; }
    T  x()  const  { return e[0]; }
    T  y()  const  { return e[1]; }

    /*
    =================================================
        lvalue Operators
    =================================================
    */

    T& operator [] (const int index) {
        return e[index];
    }

    T operator [] (const int index) const {
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

    Vec2D<T>& operator *= (const double d) {
        e[0] *= d;
        e[1] *= d;
        return *this;
    }

    bool operator == (const Vec2D& v) const {
        return e[0] == v.e[0] && e[1] == v.e[1] ? true : false;
    }

    bool operator != (const Vec2D& v) const {
        return (*this) == v ? false : true;
    }

    /*
    =================================================
        Member Methods
    =================================================
    */

    double LengthSquared() const {
        return e[0]*e[0] + e[1]*e[1];
    }

    double Length() const {
        return std::sqrt(LengthSquared());
    }

    Vec2D<double> Rotate(const double radians) const {
        return Vec2D<double>(e[0] * std::cos(radians) - e[1] * std::sin(radians), e[0] * std::sin(radians) + e[1] * std::cos(radians));
    }
};

/*
=================================================
    Aliases
=================================================
*/

typedef  Vec2D <double>  Point2;
typedef  Vec2D <int>     iPoint2;
typedef  Vec2D <double>  Vec2;
typedef  Vec2D <int>     iVec2;
typedef  Vec2D <int>     Pixel;

/*
=================================================
    rvalue Operators
=================================================
*/

template<typename T>
    inline std::ostream& operator << (std::ostream& out, const Vec2D<T>& v) {
        return out << "[" << v.x() << ", " << v.y() << "]";
    }

template<typename T>
    inline Vec2D<T> operator + (const Vec2D<T>& v1, const Vec2D<T>& v2) {
        return Vec2D<T>(v1.x() + v2.x(), v1.y() + v2.y());
    }

inline Vec2D<double> operator + (const Vec2D<int>& v1, const Vec2D<double>& v2) {
    return Vec2D<double>(static_cast<double>(v1.x()) + v2.x(), static_cast<double>(v1.y()) + v2.y());
}

inline Vec2D<double> operator + (const Vec2D<double>& v1, const Vec2D<int>& v2) {
    return Vec2D<double>(static_cast<double>(v2.x()) + v1.x(), static_cast<double>(v2.y()) + v1.y());
}

template<typename T>
    inline Vec2D<T> operator - (const Vec2D<T>& v1, const Vec2D<T>& v2) {
        return v1 + (-v2);
    }

template<typename T>
    inline Vec2D<T> operator * (const Vec2D<T>& v, const double t) {
        return Vec2D<T>(t * v.x(), t * v.y());
    }

template<typename T>
    inline Vec2D<T> operator * (const double t, const Vec2D<T>& v) {
        return Vec2D<T>(t * v.x(), t * v.y());
    }

template<typename T>
    inline Vec2D<T> operator - (const Vec2D<T>& v) {
        return Vec2D<T>(-v.x(), -v.y());
    }

template<typename T>
    inline Vec2D<T> operator / (const Vec2D<T>& v, const double t) {
        return Vec2D<T>(v.x() / t, v.y() / t);
    }

/*
=================================================
    Non-member Functions
=================================================
*/

template<typename T>
    Vec2D<T> UnitVector(const Vec2D<T>& v) {
        return v / v.Length();
    }

static void AddNoiseIfAnyIntegerComponents(Vec2D<double>& v) {
    for (int index = 0; index < 2; ++index)
        if (IsInteger(v[index]))
            v[index] += 0.01;
}

static Vec2D<double> RandomUnitVector() {
    double x      = std::rand() / static_cast<double>(RAND_MAX);
    double y      = std::rand() / static_cast<double>(RAND_MAX);
    int    signX  = std::rand() % 2;
    int    signY  = std::rand() % 2;
    return UnitVector(Vec2D<double>(signX*x, signY*y));
}