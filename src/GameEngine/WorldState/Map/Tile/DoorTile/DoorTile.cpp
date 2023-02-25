#include "DoorTile.h"

/*
=========================================================
    Static members
=========================================================
*/

texturePair_t DoorTile::gateTexture;

/*
=========================================================
    Constructors
=========================================================
*/

DoorTile::DoorTile(const iPoint2& tileCoord) {
    coordinate = tileCoord;
    type = tileType_t::DOOR;

    door = new Door();
}

/*
=========================================================
    Destructors
=========================================================
*/

DoorTile::~DoorTile() {
    delete door;
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
        if (centeredHitInfo.GetWidthPercent() < door->position) {

            // If ray is blocked by gate, then output the proper gate texture and rect
            SDL_Texture* litGateTexture = LightTexture(gateTexture, centeredHitInfo);
            double gateTextureWidthPercent = door->position - centeredHitInfo.GetWidthPercent();
            SDL_Rect gateTextureRect = {static_cast<int>(gateTextureWidthPercent * TEXTURE_PITCH), 0, 1, TEXTURE_PITCH};
            textureSlice_t gateTextureSlice(litGateTexture, gateTextureRect);

            return std::pair(gateTextureSlice, centeredHitInfo.GetDistToHitPoint());

        } else
            return std::nullopt;
    } else
        return std::nullopt;
}

bool DoorTile::PlayerTileHit() const {
    return door->position <= 0.2 ? false : true;  // Allow player to pass through door if door is at least 80% open
}

