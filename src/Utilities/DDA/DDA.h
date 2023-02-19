#pragma once

#include <optional>

#include "../Vec2D.h"
#include "../Ray/Ray.h"

/*
=========================================================
    Relevant type definitions
=========================================================
*/

enum class wallType_t {
    HORIZONTAL,
    VERTICAL,
    CORNER,
    SPRITE_PERPLINE
};

/*
=========================================================
    RayCursor (DDA algorithm encapsulated within)
=========================================================
*/

class RayHitMarker {
public:
    Ray     ray;
    Point2  hitPoint;
    iPoint2 hitTile;

private:
    typedef std::pair<wallType_t, double>       wallTypeWidthPercentPair_t;
    std::optional<wallTypeWidthPercentPair_t>   wallTypeWidthPercentPair_o;

public:
    RayHitMarker(const Ray& _ray, const Point2& _hitPoint, const iPoint2& _hitTile);
    RayHitMarker(const Ray& _ray, const Point2& _hitPoint);
    RayHitMarker(const Ray& _ray);

    void           GoToNextHit();
    RayHitMarker   GetNextHit()          const;
    RayHitMarker   GetNextCenterHit();
    double         GetDistToHitPoint()   const;
    wallType_t     GetWallType();
    double         GetWidthPercent();
    void           InsertCustomWallTypeWidthPercentPair(const wallTypeWidthPercentPair_t wallTypeWidthPercentPair);

private:
    void           CalculateWallTypeWidthPercentPair();
};

typedef std::optional<RayHitMarker> RayHitMarker_o;