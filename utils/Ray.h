#pragma once

#include "Vec2D.h"
#include "Conventions.h"

class Ray {
public:
    Vec2 origin;
    Vec2 direction;

    xDir_t xDir;    // 1 if x component of ray direction is positive, -1 if negative
    yDir_t yDir;    // 1 if y component of ray direction is positive, -1 if negative
    iVec2 xDirVec;  // <1,0> if xDir = 1, <-1,0> if xDir = -1
    iVec2 yDirVec;  // <0,1> if yDir = 1, <0,-1> if yDir = -1

    double xStep;    // Amount x changes for change of 1 unit in y
    double yStep;    // Amount y changes for change of 1 unit in x

private:
    double dxConst;  // Change in length along ray upon change in x of 1 unit
    double dyConst;  // Change in length along ray upon change in y of 1 unit

public:
    Ray(const Vec2& _origin, const Vec2& _direction);
    Ray();

    Point2 NextXIntersection(const Point2& pt) const;
    Point2 NextYIntersection(const Point2& pt) const;
    double DistToPoint(const Point2& p) const;          // Distance from origin to given point

private:
    double xAt(double y) const;            // x value at given y value on the ray
    double yAt(double x) const;                         // y value at given x value on the ray

    int nextX(const Point2& pt) const;     // Next integer x value from a given point along ray
    int nextY(const Point2& pt) const;     // Next integer y value from a given point along ray

    double RayDist_dx(double dx) const;    // Distance traversed *along ray* for a given x distance / interval
    double RayDist_dy(double dy) const;    // Distance traversed *along ray* for a given y distance / interval
};
