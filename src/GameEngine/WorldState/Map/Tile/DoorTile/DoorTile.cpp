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

DoorTile::DoorTile() {
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

rayTileHitVariant_o DoorTile::RayTileHit(RayHitMarker& hitInfo) const {
    // Center hit point
    RayHitMarker centeredHitInfo = hitInfo.GetNextCenterHit();

    // First check if incoming ray actually intersects with middle of tile (the gate)
    if (centeredHitInfo.hitTile == hitInfo.hitTile) {

        // Ray does intersect gate, but now check if the gate *blocks* the ray
        if (centeredHitInfo.GetWidthPercent() < door->position) {

            // If ray is blocked by gate, then output the proper gate texture and rect
            SDL_Texture* litGateTexture = LightTexture(gateTexture, centeredHitInfo);
            double gateTextureWidthPercent = door->position - centeredHitInfo.GetWidthPercent();
            textureSlice_t gateTextureSlice(litGateTexture, gateTextureWidthPercent);
            return textureSliceDistPair_t(gateTextureSlice, centeredHitInfo.GetDistToHitPoint());

        } else
            // Ray is not blocked by gate, meaning it passes through the DoorTile entirely
            return std::nullopt;

    } else
        // Ray does not intersect with middle of tile ==> it hits sidewall
        // Let it pass through ; renderer will detect sidewall hit and swap texture accordingly
        return std::nullopt;
}

bool DoorTile::PlayerTileHit() const {
    return door->position <= 0.2 ? false : true;  // Allow player to pass through door if door is at least 80% open
}

