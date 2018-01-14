#pragma once

#include <map>
#include <memory>
#include <vector>

#include "rapidxml.hpp"

using namespace rapidxml;

struct TileSheetData
{
	std::string m_path;
	int m_width;
	int m_height;
	int m_columns;
	int m_rows;
	int m_tileWidth;
	int m_tileHeight;
	int m_tileCount;
};

struct TileMapSprite
{
	std::shared_ptr<TileSheetData> m_data;
	int m_tileSheetID;
};


class TilemapParser
{
public:
	TilemapParser();
	~TilemapParser();

	void Parse(const std::string& fileLocation);

private:
	std::shared_ptr<std::vector<TileMapSprite>> BuildSprites();
};

