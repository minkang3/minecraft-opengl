#include <world.hpp>

namespace World
{
	int init(WorldData &world, int xmin, int xmax, int ymin, int ymax, int zmin, int zmax)
	{
		world.xsize = xmax - xmin + 1;
		world.zsize = zmax - zmin + 1;
		world.ysize = ymax - ymin + 1;

		world.xmin = xmin;
		world.ymin = ymin;
		world.zmin = zmin;

		world.blocks.resize(world.xsize * world.ysize * world.zsize);

		// TEMP
		fill(world, BlockID::GRASS, -5, 5, 0, 0, -5, 5);

		place(world, BlockID::STONE, 1, 1, 1);
		place(world, BlockID::DIRT, 2, 1, 1);
		place(world, BlockID::DIRT, 0, 5, 0);

		place(world, BlockID::STONE, -5, 1, 0);

		return 0;
	}

	BlockID& at(WorldData &world, int x, int y, int z)
	{
		return world.blocks[ (x - world.xmin) +
							 (y - world.ymin) * world.xsize +
							 (z - world.zmin) * world.xsize * world.ysize ];
	}

	void draw(WorldData &world, Renderer &render)
	{
		for (int z = world.zmin; z < world.zmin + world.zsize; ++z) {
			for (int y = world.ymin; y < world.ymin + world.ysize; ++y) {
				for (int x = world.xmin; x < world.xmin + world.xsize; ++x) {
					if (World::at(world, x, y, z) == BlockID::GRASS) {
						Render::set_texture(render, BlockID::GRASS);
						Render::draw_block(render, x, y, z);
					} else if (World::at(world, x, y, z) == BlockID::STONE) {
						Render::set_texture(render, BlockID::STONE);
						Render::draw_block(render, x, y, z);
					} else if (World::at(world, x, y, z) == BlockID::DIRT) {
						Render::set_texture(render, BlockID::DIRT);
						Render::draw_block(render, x, y, z);
					}
				}
			}
		}
	}

	void fill(WorldData &world, BlockID block_type, int xmin, int xmax, int ymin, int ymax, int zmin, int zmax)
	{
		for (int z = zmin; z <= zmax; ++z) {
			for (int y = ymin; y <= ymax; ++y) {
				for (int x = xmin; x <= xmax; ++x) {
					World::at(world, x, y, z) = block_type;
				}
			}
		}
	}

	void place(WorldData &world, BlockID block_type, int x, int y, int z)
	{
		World::at(world, x, y, z) = block_type;
	}
}
