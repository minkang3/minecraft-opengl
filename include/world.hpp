#pragma once

#include <block_renderer.h>
#include <cstdint>
#include <vector>
#include <string>
#include <iostream>

enum class Block : std::uint8_t {
	NONE  = 0,
	GRASS = 1,
	DIRT  = 2,
	STONE = 3,
};

struct BlockCoords
{
	int x, y, z;
};

class World
{
private:
	std::vector<Block> blocks;
	int xmin, ymin, zmin;
	int xsize, ysize, zsize;
public:

	World(int xmin, int xmax, int ymin, int ymax, int zmin, int zmax)
		: xmin(xmin), ymin(ymin), zmin(zmin)
	{
		xsize = xmax - xmin + 1;
		zsize = zmax - zmin + 1;
		ysize = ymax - ymin + 1;

		blocks.resize(xsize * ysize * zsize);

		// for (int z = -3; z < 3; ++z) {
		// 	for (int x = -3; x < 3; ++x) {
		// 		(*this)(x, 0, z) = Block::GRASS;
		// 	}
		// }

		// (*this)(3, 1, 3) = Block::GRASS;
		// (*this)(3, 2, 3) = Block::GRASS;
		// (*this)(3, 3, 3) = Block::GRASS;
	}

	Block& operator()(int x, int y, int z)
	{
		return blocks[ (x - xmin) + (y - ymin) * xsize + (z - zmin) * xsize * ysize ];
	}

	void draw(BlockRenderer &renderer)
	{
		for (int z = zmin; z < zmin + zsize; ++z) {
			for (int y = ymin; y < ymin + ysize; ++y) {
				for (int x = xmin; x < xmin + xsize; ++x) {
					if ((*this)(x, y, z) == Block::GRASS) {
						renderer.set_texture("grass");
						renderer.draw(x, y, z);
					} else if ((*this)(x, y, z) == Block::STONE) {
						renderer.set_texture("stone");
						renderer.draw(x, y, z);
					} else if ((*this)(x, y, z) == Block::DIRT) {
						renderer.set_texture("dirt");
						renderer.draw(x, y, z);
					}
				}
			}
		}
	}

	void fill(Block block_type, int xmin, int xmax, int ymin, int ymax, int zmin, int zmax)
	{
		for (int z = zmin; z <= zmax; ++z) {
			for (int y = ymin; y <= ymax; ++y) {
				for (int x = xmin; x <= xmax; ++x) {
					(*this)(x, y, z) = block_type;
				}
			}
		}
	}

	void place(Block block_type, int x, int y, int z)
	{
		(*this)(x, y, z) = block_type;
	}
	
	void load_blocks(std::string file_name)
	{
		// TODO:
	}
};

