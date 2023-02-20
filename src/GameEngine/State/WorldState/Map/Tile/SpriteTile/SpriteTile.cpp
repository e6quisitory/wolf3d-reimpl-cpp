#include "SpriteTile.h"

/*
================================
    Constructors
================================
*/

SpriteTile::SpriteTile(const iPoint2 &tileCoord, const texturePair_t &_texture) : texture(_texture) {
    coordinate = tileCoord;
    type = tileType_t::SPRITE;
    perplineOrigin = coordinate + Point2(0.5, 0.5);
}

/*
================================
    Static members
================================
*/

Vec2 SpriteTile::perplinesDir;

/*
================================
    Public methods
================================
*/

textureSliceDistPair_o SpriteTile::RayTileHit(RayHitMarker& hitInfo, const texturePair_o textureOverride) const {
    // Get intersection of incoming ray with perpline
    Ray incomingRay = hitInfo.ray;
    RayHitMarker_o perpLineHitInfo = RayPerplineHit(incomingRay);

    if (perpLineHitInfo.has_value()) {
        SDL_Rect textureRect = {static_cast<int>(perpLineHitInfo.value().GetWidthPercent() * TEXTURE_PITCH), 0, 1, TEXTURE_PITCH};  // One vertical line of pixels from texture
        double   hitDistance = perpLineHitInfo->GetDistToHitPoint();
        return std::pair(textureSlice_t(texture.first, textureRect), hitDistance);
    } else
        return std::nullopt;
}

bool SpriteTile::PlayerTileHit() const {
    return true;
}

/*
================================
    Private methods
================================
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
