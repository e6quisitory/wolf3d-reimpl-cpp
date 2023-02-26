#include "DDA.h"

/*
================================
    Constructors
================================
*/

RayHitMarker::RayHitMarker(const Ray& _ray, const Point2& _hitPoint, const iPoint2& _hitTile):
        ray(_ray), hitPoint(_hitPoint), hitTile(_hitTile) {}

RayHitMarker::RayHitMarker(const Ray& _ray, const Point2& _hitPoint):
        ray(_ray), hitPoint(_hitPoint), hitTile(_hitPoint) {}

RayHitMarker::RayHitMarker(const Ray& _ray) {
    (*this) = RayHitMarker(_ray, _ray.origin);
}

/*
================================
Public Methods
================================
*/

void RayHitMarker::GoToNextHit() {
    Point2 nextX = ray.NextXIntrscPoint(hitPoint);
    Point2 nextY = ray.NextYIntrscPoint(hitPoint);

    double distNextX = ray.DistToPoint(nextX);
    double distNextY = ray.DistToPoint(nextY);

    if (distNextX < distNextY) {
        hitPoint = nextX;
        hitTile += ray.xDirVec;
    } else if (distNextY < distNextX) {
        hitPoint = nextY;
        hitTile += ray.yDirVec;
    } else {
        hitPoint = ray.direction;
        hitTile += (ray.xDirVec + ray.yDirVec);
    }

    wallTypeWidthPercentPair_o.reset();
}

RayHitMarker RayHitMarker::GetNextHit() const {
    RayHitMarker temp = (*this);
    temp.GoToNextHit();
    return temp;
}

void RayHitMarker::GoToNextCenterHit() {
    Vec2 vecToCenter;
    if (GetWallType() == wallType_t::VERTICAL) {
        vecToCenter.x() = static_cast<double>(ray.xDir) / 2;
        vecToCenter.y() = static_cast<double>(ray.yDir) * (ray.yStep / 2);
    } else {
        vecToCenter.x() = static_cast<double>(ray.xDir) * (ray.xStep / 2);
        vecToCenter.y() = static_cast<double>(ray.yDir) / 2;
    }

    (*this) = RayHitMarker(ray, hitPoint + vecToCenter);
}

RayHitMarker RayHitMarker::GetNextCenterHit() {
    RayHitMarker temp = (*this);
    temp.GoToNextCenterHit();
    return temp;
}

double RayHitMarker::GetDistToHitPoint() const {
    return ray.DistToPoint(hitPoint);
}

wallType_t RayHitMarker::GetWallType() {
    if (!wallTypeWidthPercentPair_o.has_value())
        CalculateWallTypeWidthPercentPair();
    return wallTypeWidthPercentPair_o->first;
}

double RayHitMarker::GetWidthPercent() {
    if (!wallTypeWidthPercentPair_o.has_value())
        CalculateWallTypeWidthPercentPair();
    return wallTypeWidthPercentPair_o->second;
}

void RayHitMarker::InsertCustomWallTypeWidthPercentPair(const wallTypeWidthPercentPair_t wallTypeWidthPercentPair) {
    wallTypeWidthPercentPair_o = wallTypeWidthPercentPair;
}

/*
================================
    Private Methods
================================
*/

void RayHitMarker::CalculateWallTypeWidthPercentPair() {
    bool x_is_int = IsInteger(hitPoint.x());
    bool y_is_int = IsInteger(hitPoint.y());
    bool x_is_middle = (hitPoint.x() - static_cast<int>(hitPoint.x())) == 0.5;
    bool y_is_middle = (hitPoint.y() - static_cast<int>(hitPoint.y())) == 0.5;

    if ((x_is_int || x_is_middle) && !y_is_int)
        wallTypeWidthPercentPair_o = {wallType_t::VERTICAL, hitPoint.y() - int(hitPoint.y())};
    else if ((y_is_int || y_is_middle) && !x_is_int)
        wallTypeWidthPercentPair_o = {wallType_t::HORIZONTAL, hitPoint.x() - int(hitPoint.x())};
    else
        wallTypeWidthPercentPair_o = {wallType_t::CORNER, 0};
}