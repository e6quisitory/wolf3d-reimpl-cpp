#include "MapFile.h"

/*
=========================================================
    Relevant type definitions
=========================================================
*/

parsedTileInfo_t::parsedTileInfo_t(const textureType_t _textureType, const std::optional<int> _textureID):
    textureType(_textureType), textureID(_textureID) {}

/*
=========================================================
    Public Methods
=========================================================
*/

MapFile::MapFile(const std::string& mapFileName): numRows(0), numColumns(0) {
    ParseCSV(mapFileName);
}

void MapFile::ParseCSV(const std::string& mapFileName) {
    std::ifstream mapFile(mapFileName);
    parsedTileInfo1DArray_t parsedTiles1DArray;
    std::string currCellString;
    bool cellReadInProgress = false;

    while (mapFile.is_open()) {
        char currentChar = mapFile.get();
        bool isComma   = currentChar == ',';
        bool isNewline = currentChar == '\r' || currentChar == '\n';
        bool isEOF     = currentChar == EOF;

        if (isComma) {
            goto PushCurrentTileInfo;
        } else if (isNewline) {
            if (cellReadInProgress == true) {
                ++numRows;
                goto PushCurrentTileInfo;
            } else
                continue;
        } else if (isEOF) {
            ++numRows;
            mapFile.close();
            goto PushCurrentTileInfo;
        } else if (cellReadInProgress == true) {
            currCellString.push_back(currentChar);
        } else {
            cellReadInProgress = true;
            currCellString.push_back(currentChar);
            if (numRows == 0)
                ++numColumns;
        }

        goto Exit;

        PushCurrentTileInfo: {
            cellReadInProgress = false;
            PushTileInfo(parsedTiles1DArray, currCellString);
        }

        Exit: {}
    }

    /*** Transform tiles 1D array into 2D array **/
    parsedTiles.resize(numColumns);
    for (auto& column : parsedTiles)
        column.resize(numRows);

    // Fill 2D array from 1D array
    for (int tileIndex = 0; tileIndex < parsedTiles1DArray.size(); ++tileIndex) {
        auto tileCoord = iPoint2(tileIndex%numColumns, (numRows-1) - tileIndex/numColumns);  // subtracted from numRows due to flipping in next step after this loop
        parsedTiles[tileCoord.x()][tileCoord.y()] = std::pair(parsedTiles1DArray[tileIndex], tileCoord);
    }

    // Flip 2D array (top-most row goes to bottom, as map origin is at bottom-left corner
    for (auto& column : parsedTiles)
        std::reverse(column.begin(), column.end());
};

void MapFile::PushTileInfo(parsedTileInfo1DArray_t& parsedTileInfo1DArray, std::string &cellString) const {
    if (cellString.empty())
        parsedTileInfo1DArray.emplace_back(std::nullopt);
    else {
        auto parsedTextureInfo = ParseTileCode(cellString);
        if (parsedTextureInfo.has_value()) {
            auto& [textureType, textureID] = parsedTextureInfo.value();
            auto parsedTileInfo = parsedTileInfo_t(textureType, textureID);
            parsedTileInfo1DArray.emplace_back(parsedTileInfo);
        } else
            parsedTileInfo1DArray.emplace_back(std::nullopt);  // invalid format ; default to empty tile

        cellString.clear();
    }
}

parsedTextureInfo_o MapFile::ParseTileCode(const std::string& textureCode) const {
    // If enemy tile, then no texture ID code
    if (textureCode == "GU")
        return std::pair(textureType_t::GUARD, std::nullopt);
    else if (textureCode == "OF")
        return std::pair(textureType_t::OFFICER, std::nullopt);
    else if (textureCode == "SS")
        return std::pair(textureType_t::SS, std::nullopt);

    // Wall / object tile
    std::string  textureTypeCode = textureCode.substr(0, textureCode.find("-"));
    int          textureIdCode   = std::stoi(textureCode.substr(textureCode.find("-") + 1));
    if (textureTypeCode == "W")
        return std::pair(textureType_t::WALLS, textureIdCode);
    else if (textureTypeCode == "O")
        return std::pair(textureType_t::OBJECTS, textureIdCode);
    else
        return std::nullopt;  // invalid format ; default to empty tile
}