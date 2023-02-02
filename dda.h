/*
 * dda.h:
 *
 * Main DDA (Digital Differential Analyzer) algorithm to find intersections of a line (ray) with a grid map.
 * This is the core algorithm we use to render our game world.
 *
 */

#pragma once

#include <utility>

#include "utils/Vec2D.h"
#include "utils/MiscMath.h"
#include "utils/Ray.h"

enum wallType_t {
    WALL_TYPE_HORIZONTAL,
    WALL_TYPE_VERTICAL,
    WALL_TYPE_CORNER,
    WALL_TYPE_SPRITE_PERPLINE
};

class HitInfo {
public:
    Ray ray;
    Point2 hitPoint;
    iPoint2 hitTile;

private:
    typedef std::pair<wallType_t, double> wallTypeWidthPercentPair_t;
    std::optional<wallTypeWidthPercentPair_t> wallTypeWidthPercentPair_o;

public:
    HitInfo(const Ray& _ray, const Point2& _point2, const iPoint2& _ipoint2): ray(_ray), hitPoint(_point2), hitTile(_ipoint2) {}
    HitInfo(const Ray& _ray, const Point2& _point2): ray(_ray), hitPoint(_point2), hitTile(_point2) {}

    void GoToNextHit() {
        Point2 nextX = ray.NextXIntersection(hitPoint);
        Point2 nextY = ray.NextYIntersection(hitPoint);

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

    HitInfo GetNextHit() {
        HitInfo temp = (*this);
        temp.GoToNextHit();
        return temp;
    }

    HitInfo GetNextCenterHit() {
        Vec2 vecToCenter;
        if (GetWallType() == WALL_TYPE_VERTICAL) {
            vecToCenter.x() = static_cast<double>(ray.xDir) / 2;
            vecToCenter.y() = static_cast<double>(ray.yDir) * (ray.yStep / 2);
        } else {
            vecToCenter.x() = static_cast<double>(ray.xDir) * (ray.xStep / 2);
            vecToCenter.y() = static_cast<double>(ray.yDir) / 2;
        }
        return HitInfo(ray, hitPoint + vecToCenter);
    }

    double GetDistToHitPoint() const {
        return ray.DistToPoint(hitPoint);
    }

    wallType_t GetWallType() {
        if (!wallTypeWidthPercentPair_o.has_value())
            CalculateWallTypeWidthPercentPair();
        return wallTypeWidthPercentPair_o->first;
    }

    double GetWidthPercent() {
        if (!wallTypeWidthPercentPair_o.has_value())
            CalculateWallTypeWidthPercentPair();
        return wallTypeWidthPercentPair_o->second;
    }

    void InsertCustomWallTypeWidthPercentPair(const wallTypeWidthPercentPair_t& wallTypeWidthPercentPair) {
        wallTypeWidthPercentPair_o = wallTypeWidthPercentPair;
    }

private:
    void CalculateWallTypeWidthPercentPair() {
        bool x_is_int = IsInteger(hitPoint.x());
        bool y_is_int = IsInteger(hitPoint.y());
        bool x_is_middle = (hitPoint.x() - static_cast<int>(hitPoint.x())) == 0.5;
        bool y_is_middle = (hitPoint.y() - static_cast<int>(hitPoint.y())) == 0.5;

        if ((x_is_int || x_is_middle) && !y_is_int)
            wallTypeWidthPercentPair_o = {WALL_TYPE_VERTICAL, hitPoint.y() - int(hitPoint.y())};
        else if ((y_is_int || y_is_middle) && !x_is_int)
            wallTypeWidthPercentPair_o = {WALL_TYPE_HORIZONTAL, hitPoint.x() - int(hitPoint.x())};
        else
            wallTypeWidthPercentPair_o = {WALL_TYPE_CORNER, 0};
    }
};

typedef std::optional<HitInfo> HitInfo_o;