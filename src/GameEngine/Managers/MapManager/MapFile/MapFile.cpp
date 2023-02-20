#include "MapFile.h"

/*
=========================================================
    Relevant type definitions
=========================================================
*/

parsedTileInfo_t::parsedTileInfo_t(const int _tileIndex, const textureType_t _textureType, const int _textureID):
    tileIndex(_tileIndex), textureType(_textureType), textureID(_textureID) {}

/*
=========================================================
    Public Methods
=========================================================
*/

MapFile::MapFile(const std::string& mapFileName): rows(0), columns(0), numCells(0) {
    ParseCSV(mapFileName);
}

void MapFile::ParseCSV(const std::string& mapFileName) {
    std::ifstream mapFile(mapFileName);

    std::string currCellString;
    bool cellReadInProgress = false;

    int cellIndex = 0;
    while (mapFile.is_open()) {
        char currentChar = mapFile.get();
        bool isComma   = currentChar == ',';
        bool isNewline = currentChar == '\r' || currentChar == '\n';
        bool isEOF     = currentChar == EOF;

        if (isComma) {
            goto PushCurrentTileInfo;
        } else if (isNewline) {
            if (cellReadInProgress == true) {
                ++rows;
                goto PushCurrentTileInfo;
            } else
                continue;
        } else if (isEOF) {
            ++rows;
            mapFile.close();
            goto PushCurrentTileInfo;
        } else if (cellReadInProgress == true) {
            currCellString.push_back(currentChar);
        } else {
            cellReadInProgress = true;
            currCellString.push_back(currentChar);
            if (rows == 0)
                ++columns;
        }

        goto Exit;

        PushCurrentTileInfo: {
            cellReadInProgress = false;
            PushTileInfo(cellIndex, currCellString);
        }

        Exit: {}
    }
    numCells = rows * columns;

    // Fill in tile coordinates
    for (auto& tileInfo : tiles)
        tileInfo->tileCoord = iPoint2(tileInfo->tileIndex % columns, tileInfo->tileIndex / columns);
};

void MapFile::PushTileInfo(int& tileIndex, std::string& cellString) {
    if (cellString.empty())
        tiles.emplace_back(std::nullopt);
    else {
        auto parsedTextureInfo = ParseTileCode(cellString);

        if (parsedTextureInfo.has_value()) {
            auto [textureType, textureID] = parsedTextureInfo.value();
            auto parsedTileInfo = parsedTileInfo_t(tileIndex, textureType, textureID);
            tiles.emplace_back(parsedTileInfo);
        } else
            tiles.emplace_back(std::nullopt);  // invalid format ; default to empty tile

        cellString.clear();
    }
    ++tileIndex;
}

parsedTextureInfo_o MapFile::ParseTileCode(const std::string& textureCode) const {
    std::string  textureTypeCode = textureCode.substr(0, textureCode.find("-"));
    int          textureIdCode   = std::stoi(textureCode.substr(textureCode.find("-") + 1));

    if (textureTypeCode == "W")
        return std::pair(textureType_t::WALLS, textureIdCode);
    else if (textureTypeCode == "O")
        return std::pair(textureType_t::OBJECTS, textureIdCode);
    else if (textureTypeCode == "G1")
        return std::pair(textureType_t::GUARD, textureIdCode);
    else
        return std::nullopt;  // invalid format ; default to empty tile
}