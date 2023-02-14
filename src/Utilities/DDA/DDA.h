#pragma once

#include "../COMMON_HEADERS.h"

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
    HitInfo (DDA algorithm encapsulated within)
=========================================================
*/

class HitInfo {
public:
    Ray     ray;
    Point2  hitPoint;
    iPoint2 hitTile;

private:
    typedef std::pair<wallType_t, double>       wallTypeWidthPercentPair_t;
    std::optional<wallTypeWidthPercentPair_t>   wallTypeWidthPercentPair_o;

public:
    HitInfo(const Ray& _ray, const Point2& _hitPoint, const iPoint2& _hitTile);
    HitInfo(const Ray& _ray, const Point2& _hitPoint);
    HitInfo(const Ray& _ray);

    void           GoToNextHit();
    HitInfo        GetNextHit()        const;
    HitInfo        GetNextCenterHit();
    double         GetDistToHitPoint() const;
    wallType_t     GetWallType();
    double         GetWidthPercent();
    void           InsertCustomWallTypeWidthPercentPair(const wallTypeWidthPercentPair_t wallTypeWidthPercentPair);

private:
    void           CalculateWallTypeWidthPercentPair();
};

typedef std::optional<HitInfo> HitInfo_o;