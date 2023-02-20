#include "PlayerManager.h"

/*
================================
 Public Methods
================================
*/

void PlayerManager::Init(InputsBuffer* const _inputsBuffer, WorldState* const _worldState, const int _screenRefreshRate) {
    inputsBuffer = _inputsBuffer;
    worldState   = _worldState;

    // Set movement and swivel speeds based on display refresh rate (assumed that fps = refresh rate)
    movementIncrement = 3.645 / _screenRefreshRate;
    swivelIncrement   = 0.075 / _screenRefreshRate;
}

void PlayerManager::SetPlayer(const Point2& location, const Vec2& viewDir) {
    worldState->player.location = location;
    worldState->player.viewDir  = UnitVector(viewDir);
    worldState->player.eastDir = worldState->player.viewDir.Rotate(-PI/2);

    // If spawn is set for a perfect grid corner (i.e. x and y vals are both integers), there is some weird clipping that happens when you first move
    // Certainly a bug that I will investigate. But for now, if user enters in integers, a quick fix is just to add a little decimal value to them to
    // avoid the bug
    for (int i = 0; i < 2; ++i)
        if (IsInteger(worldState->player.location[i]))
            worldState->player.location[i] += 0.01;
}

void PlayerManager::Update() const {

    inputCommand_t currentLookingCommand = inputsBuffer->currentCommands[inputCommandType_t::LOOKING];

    switch(currentLookingCommand) {
        case inputCommand_t::LOOK_RIGHT:
            Swivel(swivelDir_t::CLOCKWISE); break;

        case inputCommand_t::LOOK_LEFT:
            Swivel(swivelDir_t::COUNTER_CLOCKWISE); break;
    }

    inputCommand_t currentMovementCommand = inputsBuffer->currentCommands[inputCommandType_t::MOVEMENT];

    switch(currentMovementCommand) {
        case inputCommand_t::MOVE_EAST:
            MoveX(xDir_t::EAST, playerSpeed_t::FULL);
            break;

        case inputCommand_t::MOVE_WEST:
            MoveX(xDir_t::WEST, playerSpeed_t::FULL);
            break;

        case inputCommand_t::MOVE_NORTH:
            MoveY(yDir_t::NORTH, playerSpeed_t::FULL);
            break;

        case inputCommand_t::MOVE_SOUTH:
            MoveY(yDir_t::SOUTH, playerSpeed_t::FULL);
            break;

        case inputCommand_t::MOVE_NORTHEAST:
            MoveY(yDir_t::NORTH, playerSpeed_t::HALF);
            MoveX(xDir_t::EAST, playerSpeed_t::HALF);
            break;

        case inputCommand_t::MOVE_NORTHWEST:
            MoveY(yDir_t::NORTH, playerSpeed_t::HALF);
            MoveX(xDir_t::WEST, playerSpeed_t::HALF);
            break;

        case inputCommand_t::MOVE_SOUTHEAST:
            MoveY(yDir_t::SOUTH, playerSpeed_t::HALF);
            MoveX(xDir_t::EAST, playerSpeed_t::HALF);
            break;

        case inputCommand_t::MOVE_SOUTHWEST:
            MoveY(yDir_t::SOUTH, playerSpeed_t::HALF);
            MoveX(xDir_t::WEST, playerSpeed_t::HALF);
            break;
    }

    inputCommand_t currentDoorsCommand = inputsBuffer->currentCommands[inputCommandType_t::DOORS];

    if (currentDoorsCommand == inputCommand_t::OPEN_DOOR)
        OpenDoor();
}

/*
================================
    Private Methods
================================
*/

double PlayerManager::GetPlayerSpeedCoeff(const playerSpeed_t playerSpeed) const {
    switch (playerSpeed) {
        case playerSpeed_t::FULL: return movementIncrement;
        case playerSpeed_t::HALF: return 0.70711 * movementIncrement;  // Reduce speed in each direction if going diagonally (45-45 triangle)
    }
}

void PlayerManager::MoveX(const xDir_t xDir, const playerSpeed_t playerSpeed) const {
    Vec2 moveVec       = static_cast<int>(xDir) * GetPlayerSpeedCoeff(playerSpeed) * worldState->player.eastDir;
    Point2 proposedLoc = worldState->player.location + moveVec;

    MovePlayerIfValid(proposedLoc);
}

void PlayerManager::MoveY(const yDir_t yDir, const playerSpeed_t playerSpeed) const {
    Vec2 movVec        = static_cast<int>(yDir) * GetPlayerSpeedCoeff(playerSpeed) * worldState->player.viewDir;
    Point2 proposedLoc = worldState->player.location + movVec;

    MovePlayerIfValid(proposedLoc);
}

void PlayerManager::Swivel(const swivelDir_t swivelDir) const {
    static const double negNinetyDeg = -PI/2;
    worldState->player.viewDir = worldState->player.viewDir.Rotate(swivelIncrement * inputsBuffer->mouseAbsXrel * static_cast<int>(swivelDir));
    worldState->player.eastDir = worldState->player.viewDir.Rotate(negNinetyDeg);
}

void PlayerManager::MovePlayerIfValid(const Point2& proposedLoc) const {
    Tile* proposed_tile = worldState->map.GetTile(proposedLoc);
    if (!proposed_tile->PlayerTileHit())
        worldState->player.location = proposedLoc;
}

Ray PlayerManager::GetPlayerViewDirRay() const {
    return Ray(worldState->player.location, worldState->player.viewDir);
}

void PlayerManager::OpenDoor() const {
    RayHitMarker rayCursor(GetPlayerViewDirRay());

    while (worldState->map.WithinMap(rayCursor.hitTile)) {
        rayCursor.GoToNextHit();
        if (rayCursor.GetDistToHitPoint() > 4.0)
            break;
        else {
            Tile* currentTile = worldState->map.GetTile(rayCursor.hitTile);
            switch (currentTile->type) {
                case tileType_t::EMPTY:
                case tileType_t::SPRITE:
                    continue;
                case tileType_t::DOOR:
                {
                    DoorTile* currentDoor = static_cast<DoorTile*>(currentTile);
                    switch (currentDoor->doorStatus) {
                        case doorStatus_t::CLOSED:
                            worldState->map.AddActiveDoor(currentDoor);
                        case doorStatus_t::CLOSING: currentDoor->doorStatus = doorStatus_t::OPENING;
                    }
                }
                case tileType_t::WALL:
                    goto exit_loop;
            }
        }
    }
    exit_loop: {}
}