#pragma once

#include <cstdint>

enum class BlockID : std::uint8_t {
	NONE  = 0,
	GRASS = 1,
	DIRT  = 2,
	STONE = 3,
};

struct BlockCoords
{
	int x, y, z;
};
