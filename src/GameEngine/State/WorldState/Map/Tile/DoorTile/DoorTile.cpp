#include "DoorTile.h"

/*
=========================================================
    Constructors
=========================================================
*/

DoorTile::DoorTile(const iPoint2 &tileCoord, const texturePairsPair_t doorTextures) {
    coordinate = tileCoord;

    auto [_gateTexture, _gateSidewallTexture] = doorTextures;
    gateTexture         = _gateTexture;
    gateSidewallTexture = _gateSidewallTexture;

    type = tileType_t::DOOR;

    // Gate initial status is closed, with timer reset to full-time left (to be decremented when door fully opens)
    doorStatus   = doorStatus_t::CLOSED;
    doorPosition = static_cast<double>(doorPosition_t::CLOSED);
    doorTimerVal = static_cast<double>(doorTimerVal_t::FULL_TIME_LEFT);
}

/*
=========================================================
    Public methods
=========================================================
*/

textureSliceDistPair_o DoorTile::RayTileHit(RayHitMarker& hitInfo, const texturePair_o textureOverride) const {
    // Center hit point
    RayHitMarker centeredHitInfo = hitInfo.GetNextCenterHit();

    // First check if incoming ray actually intersects with middle of tile (the gate)
    if (centeredHitInfo.hitTile == hitInfo.hitTile) {

        // Ray does intersect gate, but now check if the gate *blocks* the ray
        if (centeredHitInfo.GetWidthPercent() < doorPosition) {

            // If ray is blocked by gate, then output the proper gate texture and rect
            SDL_Texture* litGateTexture = LightTexture(gateTexture, centeredHitInfo);
            double gateTextureWidthPercent = doorPosition - centeredHitInfo.GetWidthPercent();
            SDL_Rect gateTextureRect = {static_cast<int>(gateTextureWidthPercent * TEXTURE_PITCH), 0, 1, TEXTURE_PITCH};
            textureSlice_t gateTextureSlice(litGateTexture, gateTextureRect);

            return std::pair(gateTextureSlice, centeredHitInfo.GetDistToHitPoint());

        } else
            return std::nullopt;
    } else
        return std::nullopt;
}

bool DoorTile::PlayerTileHit() const {
    return doorPosition <= 0.2 ? false : true;  // Allow player to pass through door if door is at least 80% open
}

