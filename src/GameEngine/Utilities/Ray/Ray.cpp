#include "Ray.h"
#include "../MiscMath/MiscMath.h"
#include <cmath>

/*
================================
    Constructors
================================
*/

Ray::Ray(const Vec2& _origin, const Vec2& _direction):
    origin(_origin), direction(UnitVector(_direction)) {

    dxConst = std::sqrt(1 + pow(direction.y() / direction.x(), 2));
    dyConst = std::sqrt(1 + pow(direction.x() / direction.y(), 2));

    xDir = direction.x() > 0 ? xDir_t::EAST : xDir_t::WEST;
    yDir = direction.y() > 0 ? yDir_t::NORTH : yDir_t::SOUTH;
    xDirVec = iVec2(xDir);
    yDirVec = iVec2(yDir);

    yStep = std::sqrt(dxConst * dxConst - 1);
    xStep = std::sqrt(dyConst * dyConst - 1);
}

Ray::Ray() {}

/*
================================
    Public Methods
================================
*/

Point2 Ray::NextXIntersection(const Point2& pt) const {
    int next_x = nextX(pt);
    return Point2(next_x, yAt(next_x));
}

Point2 Ray::NextYIntersection(const Point2& pt) const {
    int next_y = nextY(pt);
    return Point2(xAt(next_y), next_y);
}

double Ray::DistToPoint(const Point2& p) const {
    return RayDist_dx(p.x() - origin.x());
}

/*
================================
    Private Methods
================================
*/

double Ray::yAt(double x) const {
    return origin.y() + ((x-origin.x())/direction.x())*direction.y();
}

double Ray::xAt(double y) const {
    return origin.x() + ((y-origin.y())/direction.y())*direction.x();
}

int Ray::nextX(const Point2& pt) const {
    if (!IsInteger(pt.x())) {
        if (xDir == xDir_t::EAST)
            return static_cast<int>(ceil(pt.x()));
        else
            return static_cast<int>(floor(pt.x()));
    } else {
        return pt.x() + static_cast<int>(xDir);
    }
}

int Ray::nextY(const Point2& pt) const {
    if (!IsInteger(pt.y())) {
        if (yDir == yDir_t::NORTH)
            return static_cast<int>(ceil(pt.y()));
        else
            return static_cast<int>(floor(pt.y()));
    } else {
        return pt.y() + static_cast<int>(yDir);
    }
}

double Ray::RayDist_dx(double dx) const {
    return abs(dx) * dxConst;
}

double Ray::RayDist_dy(double dy) const {
    return abs(dy) * dyConst;
}

