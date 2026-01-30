#pragma once

#include <block_renderer.hpp>
#include <block.hpp>
#include <cstdint>
#include <vector>
#include <string>
#include <iostream>


class World
{
private:
	std::vector<BlockID> blocks;
public:
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
		// 		(*this)(x, 0, z) = BlockID::GRASS;
		// 	}
		// }

		// (*this)(3, 1, 3) = BlockID::GRASS;
		// (*this)(3, 2, 3) = BlockID::GRASS;
		// (*this)(3, 3, 3) = BlockID::GRASS;
	}

	BlockID& operator()(int x, int y, int z)
	{
		return blocks[ (x - xmin) + (y - ymin) * xsize + (z - zmin) * xsize * ysize ];
	}

	void draw(BlockRenderer &renderer)
	{
		for (int z = zmin; z < zmin + zsize; ++z) {
			for (int y = ymin; y < ymin + ysize; ++y) {
				for (int x = xmin; x < xmin + xsize; ++x) {
					if ((*this)(x, y, z) == BlockID::GRASS) {
						renderer.set_texture(BlockID::GRASS);
						renderer.draw(x, y, z);
					} else if ((*this)(x, y, z) == BlockID::STONE) {
						renderer.set_texture(BlockID::STONE);
						renderer.draw(x, y, z);
					} else if ((*this)(x, y, z) == BlockID::DIRT) {
						renderer.set_texture(BlockID::DIRT);
						renderer.draw(x, y, z);
					}
				}
			}
		}
	}

	void fill(BlockID block_type, int xmin, int xmax, int ymin, int ymax, int zmin, int zmax)
	{
		for (int z = zmin; z <= zmax; ++z) {
			for (int y = ymin; y <= ymax; ++y) {
				for (int x = xmin; x <= xmax; ++x) {
					(*this)(x, y, z) = block_type;
				}
			}
		}
	}

	void place(BlockID block_type, int x, int y, int z)
	{
		(*this)(x, y, z) = block_type;
	}
	
	void load_blocks(std::string file_name)
	{
		// TODO:
	}
};

