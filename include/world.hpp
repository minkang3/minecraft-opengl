#pragma once

#include <block_renderer.h>
#include <vector>
#include <string>
#include <iostream>

struct Block
{
	int x, y, z;
	std::string type;

	Block(int x, int y, int z, std::string type)
		: x(x), y(y), z(z), type(type) {}

	void draw(BlockRenderer &renderer)
	{
		bool ret = renderer.set_texture(type);
		renderer.draw(x, y, z);
	}
};

class World
{
private:
public:
	std::vector<Block> blocks;

	World()
	{
		for (int x = 0; x < 10; ++x) {
			for (int z = 0; z < 10; ++z) {
				blocks.emplace_back(x, 0, z, "grass_block");
			}
		}

		blocks.emplace_back(3, 1, 3, "grass_block");
		blocks.emplace_back(4, 1, 4, "grass_block");
		blocks.emplace_back(8, 1, 8, "grass_block");
		blocks.emplace_back(8, 2, 8, "grass_block");
		blocks.emplace_back(8, 3, 8, "grass_block");
		blocks.emplace_back(7, 3, 8, "grass_block");
	}

	void draw(BlockRenderer &renderer)
	{
		for (Block b : blocks) {
			bool ret = renderer.set_texture(b.type);
			//std::cout << "in draw, ret: " << ret << std::endl;
			renderer.draw(b.x, b.y, b.z);
		}
	}
	
	void load_blocks(std::string file_name)
	{
		// TODO:
	}
};

