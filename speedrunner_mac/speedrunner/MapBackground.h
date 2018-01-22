#pragma once

#include <string>

class MapBackground
{
public:
	MapBackground();
	~MapBackground();

	void Load(const std::string& backgroundFilePath);
};

