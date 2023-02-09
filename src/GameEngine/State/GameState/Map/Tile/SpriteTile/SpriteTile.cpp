#include "SpriteTile.h"

/*
================================
    Constructors
================================
*/

SpriteTile::SpriteTile(const Point2& _center, const texturePair_t& _texture): texture(_texture) {
    tileType = tileType_t::SPRITE;
    perpLine.origin = _center;
}

/*
================================
    Public methods
================================
*/

textureSliceDistPair_o SpriteTile::RayTileHit(HitInfo& hitInfo, const texturePair_o& textureOverride) const {
    HitInfo_o perpLineHitInfo = RayPerpLineHit(hitInfo.ray);

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

void SpriteTile::CalculatePerpLine(const Vec2& view_dir) {
    static double negNinetyDeg = -PI / 2;
    perpLine.direction = view_dir.Rotate(negNinetyDeg);
}

/*
================================
    Private methods
================================
*/

HitInfo_o SpriteTile::RayPerpLineHit(const Ray& incomingRay) const {
    Vec2 O1 = incomingRay.origin;
    Vec2 D1 = incomingRay.direction;
    Vec2 O2 = perpLine.origin;
    Vec2 D2 = perpLine.direction;

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
            double perpLineWidthPercent = t > 0 ? 0.5 + t : 0.5 - abs(t);

            HitInfo perpLineHitInfo(incomingRay, perpLineHitPoint);
            perpLineHitInfo.InsertCustomWallTypeWidthPercentPair({wallType_t::SPRITE_PERPLINE, perpLineWidthPercent});

            return perpLineHitInfo;
        }
    }
}
