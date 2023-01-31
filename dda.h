/*
 * dda.h:
 *
 * Main DDA (Digital Differential Analyzer) algorithm to find intersections of a line (ray) with a grid map.
 * This is the core algorithm we use to render our game world.
 *
 */

#pragma once

#include <utility>

#include "vec2.h"
#include "Ray.h"

enum wallType_t {
    WALL_TYPE_HORIZONTAL,
    WALL_TYPE_VERTICAL,
    WALL_TYPE_CORNER,
    WALL_TYPE_SPRITE
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
        Point2 nextX = ray.next_x_intersection(hitPoint);
        Point2 nextY = ray.next_y_intersection(hitPoint);

        double distNextX = ray.dist_to_pt(nextX);
        double distNextY = ray.dist_to_pt(nextY);

        if (distNextX < distNextY) {
            hitPoint = nextX;
            hitTile += ray.x_dir_vec;
        } else if (distNextY < distNextX) {
            hitPoint = nextY;
            hitTile += ray.y_dir_vec;
        } else {
            hitPoint = ray.direction;
            hitTile += (ray.x_dir_vec + ray.y_dir_vec);
        }

        wallTypeWidthPercentPair_o.reset();
    }

    HitInfo GetNextCenterHit() {
        Vec2 vecToCenter;
        if (GetWallType() == WALL_TYPE_VERTICAL) {
            vecToCenter.x() = static_cast<double>(ray.x_dir)/2;
            vecToCenter.y() = static_cast<double>(ray.y_dir)*(ray.y_step/2);
        } else {
            vecToCenter.x() = static_cast<double>(ray.x_dir)*(ray.x_step/2);
            vecToCenter.y() = static_cast<double>(ray.y_dir)/2;
        }
        return HitInfo(ray, hitPoint + vecToCenter);
    }

    double GetDistToHitPoint() const {
        return ray.dist_to_pt(hitPoint);
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
        bool x_is_int = is_integer(hitPoint.x());
        bool y_is_int = is_integer(hitPoint.y());
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