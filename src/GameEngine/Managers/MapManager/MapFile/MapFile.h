#pragma once

#include "../../../Multimedia/Multimedia.h"

/*
=========================================================
    Relevant type definitions
=========================================================
*/

struct parsedTileInfo_t {
    parsedTileInfo_t(const int _tileIndex, const textureType_t _textureType, const int _textureID);

    int            tileIndex;
    iPoint2        tileCoord;
    textureType_t  textureType;
    int            textureID;
};

typedef  std::optional  <parsedTileInfo_t>      parsedTileInfo_o;
typedef  std::pair      <textureType_t, int>    parsedTextureInfo_t;
typedef  std::optional  <parsedTextureInfo_t>   parsedTextureInfo_o;

/*
=========================================================
    MapFile
=========================================================
*/

class MapFile {
public:
    std::vector<parsedTileInfo_o> tiles;
    int columns;
    int rows;
    int numCells;

public:
    MapFile(const std::string& mapFileName);

private:
    void                 ParseCSV      (const std::string& mapFileName);
    void                 PushTileInfo  (int& tileIndex, std::string& cellString);
    parsedTextureInfo_o  ParseTileCode (const std::string& textureCode)             const;
};