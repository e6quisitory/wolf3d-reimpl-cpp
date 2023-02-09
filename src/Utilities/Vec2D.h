#pragma once

#include <cmath>
#include <iostream>
#include <iterator>

#include "Conventions.h"

/*
=========================================
    Vec2D (2D int or double vector)
=========================================
*/

template<typename T>
class Vec2D {
public:
    T e[2];

public:
    Vec2D(const T& x, const T& y);
    Vec2D(const T& a);
    Vec2D();

    template<typename U>
    Vec2D(const Vec2D<U>& v);

    Vec2D<int>(const xDir_t& xDir);
    Vec2D<int>(const yDir_t& yDir);

    T& x();
    T& y();
    T x() const;
    T y() const;

    T& operator [] (const int& index);
    T operator [] (const int& index) const;

    Vec2D<T>& operator += (const Vec2D<T>& v);
    Vec2D<T>& operator -= (const Vec2D<T>& v);
    Vec2D<T>& operator *= (const double& d);

    bool operator == (const Vec2D& v);

    double LengthSquared() const;
    double Length() const;
    Vec2D<double> Rotate(const double& radians) const;
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
    Constructors
================================
*/

template<typename T>
    Vec2D<T>::Vec2D(const T& x, const T& y):
        e{x, y} {}

template<typename T>
    Vec2D<T>::Vec2D(const T& a):
        e{a, a} {}

template<typename T>
    Vec2D<T>::Vec2D():
        e{static_cast<T>(0), static_cast<T>(0)} {}

template<typename T>
    template<typename U>
        Vec2D<T>::Vec2D(const Vec2D<U>& v):
            e{static_cast<T>(v.x()), static_cast<T>(v.y())} {}

template<typename T>
    Vec2D<T>::Vec2D(const xDir_t& xDir):
        e{static_cast<int>(xDir), 0} {}

template<typename T>
Vec2D<T>::Vec2D(const yDir_t& yDir):
        e{0, static_cast<int>(yDir)} {}

/*
================================
    Getters / Setters
================================
*/

template<typename T>
    T& Vec2D<T>::x() {
        return e[0];
    }

template<typename T>
    T& Vec2D<T>::y() {
        return e[1];
    }

template<typename T>
    T Vec2D<T>::x() const {
        return e[0];
    }

template<typename T>
    T Vec2D<T>::y() const {
        return e[1];
    }

/*
================================
    lvalue Operators
================================
*/

template<typename T>
    T& Vec2D<T>::operator [] (const int& index) {
        return e[index];
    }

template<typename T>
    T Vec2D<T>::operator [] (const int& index) const {
        return e[index];
    }

template<typename T>
    Vec2D<T>& Vec2D<T>::operator += (const Vec2D<T>& v) {
        e[0] += v.e[0];
        e[1] += v.e[1];
        return *this;
    }

template<typename T>
    Vec2D<T>& Vec2D<T>::operator -= (const Vec2D<T>& v) {
        e[0] -= v.e[0];
        e[1] -= v.e[1];
        return *this;
    }

template<typename T>
    Vec2D<T>& Vec2D<T>::operator *= (const double& d) {
        e[0] *= d;
        e[1] *= d;
        return *this;
    }

template<typename T>
    bool Vec2D<T>::operator == (const Vec2D& v) {
        return e[0] == v.e[0] && e[1] == v.e[1] ? true : false;
    }

/*
================================
    rvalue Operators
================================
*/

template<typename T>
    inline std::ostream& operator << (std::ostream& out, const Vec2D<T>& v) {
        return out << "[" << v.x() << ", " << v.y() << "]";
    }

template<typename T>
    inline Vec2D<T> operator + (const Vec2D<T>& v1, const Vec2D<T>& v2) {
        return Vec2D<T>(v1.x() + v2.x(), v1.y() + v2.y());
    }

template<typename T>
    inline Vec2D<T> operator - (const Vec2D<T>& v1, const Vec2D<T>& v2) {
        return v1 + (-v2);
    }

template<typename T>
    inline Vec2D<T> operator * (const Vec2D<T>& v, const double& t) {
        return Vec2D<T>(t * v.x(), t * v.y());
    }

template<typename T>
    inline Vec2D<T> operator * (const double& t, const Vec2D<T>& v) {
        return Vec2D<T>(t * v.x(), t * v.y());
    }

template<typename T>
    inline Vec2D<T> operator - (const Vec2D<T>& v) {
        return Vec2D<T>(-v.x(), -v.y());
    }

template<typename T>
    inline Vec2D<T> operator / (const Vec2D<T>& v, const double& t) {
        return Vec2D<T>(v.x() / t, v.y() / t);
    }

/*
================================
    Member Methods
================================
*/

template<typename T>
    double Vec2D<T>::LengthSquared() const {
        return e[0]*e[0] + e[1]*e[1];
    }

template<typename T>
    double Vec2D<T>::Length() const {
        return std::sqrt(LengthSquared());
    }

template<typename T>
    Vec2D<double> Vec2D<T>::Rotate(const double& radians) const {
        return Vec2D<double>(e[0] * std::cos(radians) - e[1] * std::sin(radians), e[0] * std::sin(radians) + e[1] * std::cos(radians));
    }

/*
================================
    Non-member Functions
================================
*/

template<typename T>
    Vec2D<T> UnitVector(const Vec2D<T>& v) {
        return v / v.Length();
    }