/*
 * tile.h:
 *
 * Tile objects constructed using OOP concepts (inheritance, polymorphism).
 *
 * Goal is that when the raycaster "hits" a tile, the tile *itself* spits out:
 *
 *     (i)  whether it is empty of not
 *     (ii) if not empty, then the texture + portion of it that the ray hits
 *
 *  In addition to a ray-tile intersection, we can also calculate player-tile intersections, so that player doesn't pass through non-empty blocks.
 *
 */

#pragma once

#include <optional>
#include <SDL2/SDL.h>

#include "utils/Vec2D.h"
#include "Ray.h"
#include "dda.h"
#include "global.h"

/*
=========================================================
 Texture-related typedefs
=========================================================
*/

typedef std::pair<SDL_Texture*, SDL_Texture*> texturePair_t;
typedef std::pair<texturePair_t, texturePair_t> texturePairsPair_t;

struct textureSlice_t {
    textureSlice_t(SDL_Texture* t, SDL_Rect r): texture(t), textureRect(r) {}
    textureSlice_t() {}

    SDL_Texture* texture;    // If non-empty, then the texture of the tile hit
    SDL_Rect textureRect;   // The portion of the texture that the ray intersected with
};

typedef std::optional<std::pair<textureSlice_t, double>> textureSliceDistPair_o;
typedef std::optional<texturePair_t> textureOverride_o;

/*
================================
 Tile base class (abstract)
================================
*/

enum tileType_t {
    TILE_TYPE_EMPTY,
    TILE_TYPE_WALL,
    TILE_TYPE_DOOR,
    TILE_TYPE_SPRITE
};

class Tile {
public:
    virtual ~Tile() {}
    virtual textureSliceDistPair_o RayTileHit(HitInfo& hitInfo, const textureOverride_o& textureOverride) const = 0;
    virtual bool PlayerTileHit() const = 0;

public:
    tileType_t type;

protected:
    SDL_Texture* LightTexture(const texturePair_t& _texture_pair, HitInfo& hitInfo) const {
        if (hitInfo.GetWallType() == WALL_TYPE_VERTICAL)
            return _texture_pair.first;
        else
            return _texture_pair.second;
    }
};

/*
================================
 Empty tile
================================
*/

class EmptyTile : public Tile {
public:
    EmptyTile() {
        type = TILE_TYPE_EMPTY;
    }

    virtual textureSliceDistPair_o RayTileHit(HitInfo& hitInfo, const textureOverride_o& textureOverride) const override {
        return std::nullopt;
    }

    virtual bool PlayerTileHit() const override {
        return false;
    }
};

/*
================================
 Basic textured wall tile
================================
*/

class WallTile : public Tile {
public:
    WallTile(const texturePair_t& _texture): texture(_texture) {
        type = TILE_TYPE_WALL;
    }

    virtual textureSliceDistPair_o RayTileHit(HitInfo& hitInfo, const textureOverride_o& textureOverride) const override {
        SDL_Texture* litTexture = textureOverride.has_value() ? LightTexture(textureOverride.value(), hitInfo) : LightTexture(texture, hitInfo);
        SDL_Rect textureRect = {static_cast<int>(hitInfo.GetWidthPercent() * TEXTURE_PITCH), 0, 1, TEXTURE_PITCH};  // One vertical line of pixels from texture
        textureSlice_t textureSlice(litTexture, textureRect);

        return std::pair(textureSlice, hitInfo.GetDistToHitPoint());
    }

    virtual bool PlayerTileHit() const override {
        return true;
    }

private:
    texturePair_t texture;
};

/*
================================
 Door tile
================================
*/

enum doorStatus_t {
    DOOR_STATUS_CLOSED,
    DOOR_STATUS_OPENING,
    DOOR_STATUS_OPEN,
    DOOR_STATUS_CLOSING
};

enum doorPosition_t {
    DOOR_POSITION_OPEN = 0,
    DOOR_POSITION_CLOSED = 1
};

enum timerValue_t {
    TIMER_VALUE_NONE = 0,
    TIMER_VALUE_FULL = 1
};

class DoorTile : public Tile {
public:
    DoorTile(const texturePairsPair_t& doorTextures) {
        gate_texture = doorTextures.first;
        gate_sidewall_texture = doorTextures.second;

        type = TILE_TYPE_DOOR;

        // Gate initial status is closed, with timer reset to full-time left (to be decremented when door fully opens)
        status = DOOR_STATUS_CLOSED;
        position = DOOR_POSITION_CLOSED;
        timer = TIMER_VALUE_FULL;
    }

    virtual textureSliceDistPair_o RayTileHit(HitInfo& hitInfo, const textureOverride_o& textureOverride) const override {

        // Center hit point
        HitInfo centeredHitInfo = hitInfo.GetNextCenterHit();

        // First check if incoming ray actually intersects with middle of tile (the gate)
        if (centeredHitInfo.hitTile == hitInfo.hitTile) {

            // Ray does intersect gate, but now check if the gate *blocks* the ray
            if (centeredHitInfo.GetWidthPercent() < position) {

                // If ray is blocked by gate, then output the proper gate texture and rect
                SDL_Texture* litGateTexture = LightTexture(gate_texture, centeredHitInfo);
                double gateTextureWidthPercent = position - centeredHitInfo.GetWidthPercent();
                SDL_Rect gateTextureRect = {static_cast<int>(gateTextureWidthPercent * TEXTURE_PITCH), 0, 1, TEXTURE_PITCH};
                textureSlice_t gateTextureSlice(litGateTexture, gateTextureRect);

                return std::pair(gateTextureSlice, centeredHitInfo.GetDistToHitPoint());

            } else
                return std::nullopt;
        } else
            return std::nullopt;
    }

    virtual bool PlayerTileHit() const override {
        return position <= 0.2 ? false : true;  // Allow player to pass through door if door is at least 80% open
    }

public:
    doorStatus_t status;
    double position;
    double timer;

private:
    texturePair_t gate_texture;
    texturePair_t gate_sidewall_texture;
};

/*
================================
 Sprite tile
================================
*/

class SpriteTile : public Tile {
public:
    SpriteTile(const Point2& _center, const texturePair_t& _texture): texture(_texture) {
        type = TILE_TYPE_SPRITE;
        perpLine.origin = _center;
    }
    
    virtual textureSliceDistPair_o RayTileHit(HitInfo& hitInfo, const textureOverride_o& textureOverride) const override {
        HitInfo_o perpLineHitInfo = PerpLineHit(hitInfo);
        
        if (perpLineHitInfo.has_value()) {
            SDL_Rect rect = {static_cast<int>(perpLineHitInfo.value().GetWidthPercent() * TEXTURE_PITCH), 0, 1, TEXTURE_PITCH};  // One vertical line of pixels from texture
            double distance = perpLineHitInfo->GetDistToHitPoint();
            return std::pair(textureSlice_t(texture.first, rect), distance);
        } else
            return std::nullopt;
    }

    virtual bool PlayerTileHit() const override {
        return true;
    }

    void CalculatePerpLine(const Vec2& view_dir) {
        static double neg_ninety_deg = -PI/2;
        perpLine.direction = view_dir.Rotate(neg_ninety_deg);
    }
    
private:
    // Calculates intersection between incoming ray and perp_line
    HitInfo_o PerpLineHit(const HitInfo& hitInfo) const {
        Ray incomingRay = hitInfo.ray;

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
                perpLineHitInfo.InsertCustomWallTypeWidthPercentPair({WALL_TYPE_SPRITE_PERPLINE, perpLineWidthPercent});

                return perpLineHitInfo;
            }
        }
    }

private:
    texturePair_t texture;
    Ray perpLine;          // Vector line (ray) perpendicular to player view direction ; same for ALL sprites
};
