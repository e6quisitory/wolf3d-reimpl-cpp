#include "SpriteTile.h"

/*
=========================================================
    Static members
=========================================================
*/

Vec2 SpriteTile::perplinesDir;

/*
=========================================================
    Public methods
=========================================================
*/

void SpriteTile::UpdateUniversalPerpline(const Vec2 &playerViewDir) {
    static const double negNinetyDeg = -PI / 2;
    Vec2 playerViewDirPerp = playerViewDir.Rotate(negNinetyDeg);
    perplinesDir = playerViewDirPerp;
}

/*
=========================================================
    Protected methods
=========================================================
*/

RayHitMarker_o SpriteTile::RayPerplineHit(const Ray& incomingRay) const {
    Point2 O1 = incomingRay.origin;
    Vec2   D1 = incomingRay.direction;
    Point2 O2 = perplineOrigin;
    Vec2   D2 = perplinesDir;

    double denominator = D2.x()*D1.y()-D2.y()*D1.x();
    if (denominator == 0)
        return {};
    else {
        double numerator = D1.x()*(O2.y()-O1.y())-D1.y()*(O2.x()-O1.x());
        double t = numerator/denominator;

        if (abs(t) > 0.5)
            return std::nullopt;
        else {
            Point2 perpLineHitPoint = O2 + t * D2;
            double perpLineWidthPercent = 0.5 + t;

            RayHitMarker perpLineHitInfo(incomingRay, perpLineHitPoint);
            perpLineHitInfo.InsertCustomWallTypeWidthPercentPair({wallType_t::SPRITE_PERPLINE, perpLineWidthPercent});

            return perpLineHitInfo;
        }
    }
}
