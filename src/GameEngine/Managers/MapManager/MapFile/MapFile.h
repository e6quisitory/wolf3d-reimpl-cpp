#pragma once

#include "../../../Multimedia/Multimedia.h"

/*
=========================================================
    Relevant type definitions
=========================================================
*/

struct parsedTileInfo_t {
    parsedTileInfo_t(const textureType_t _textureType, const int _textureID);

    iPoint2        tileCoord;
    textureType_t  textureType;
    int            textureID;
};

typedef  std::optional              <parsedTileInfo_t>            parsedTileInfo_o;
typedef  std::pair                  <textureType_t, int>          parsedTextureInfo_t;
typedef  std::optional              <parsedTextureInfo_t>         parsedTextureInfo_o;
typedef  std::vector                <parsedTileInfo_o>            parsedTileInfo1DArray_t;
typedef  std::pair                  <parsedTileInfo_o, iPoint2>   tileInfoAndCoord_t;
typedef  std::vector<std::vector    <tileInfoAndCoord_t>>         tileInfoAndCoord2DGrid_t;

/*
=========================================================
    MapFile
=========================================================
*/

class MapFile {
public:
    tileInfoAndCoord2DGrid_t parsedTiles;
    int numColumns;
    int numRows;

public:
    MapFile(const std::string& mapFileName);

private:
    void                 ParseCSV       (const std::string& mapFileName);
    void                 PushTileInfo   (parsedTileInfo1DArray_t& parsedTileInfo1DArray, std::string &cellString)  const;
    parsedTextureInfo_o  ParseTileCode  (const std::string& textureCode)                                           const;
};