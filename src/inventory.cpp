#include <inventory.hpp>
#include <cassert>

namespace Inventory
{
	void init(InventoryData &inventory)
	{
		inventory.items.assign(34, {BlockID::NONE, 0});

		inventory.items[0] = {BlockID::GRASS, 1};
		inventory.items[1] = {BlockID::DIRT,  1};
		inventory.items[2] = {BlockID::STONE, 1};
		inventory.items[3] = {BlockID::STONE, 1};
	}

	void hotbar_select_slot(InventoryData &inventory, unsigned int slot_num)
	{
		assert(1 <= slot_num && slot_num <= 9 && "hotbar slot num must be between 1 and 9");
		inventory.hotbar_slot = slot_num;
	}

	void place_block(InventoryData &inventory, CameraData &camera, WorldData &world)
	{
		if (inventory.items[inventory.hotbar_slot - 1].id == BlockID::NONE)
			return;

		Camera::place_block(camera, world, inventory.items[inventory.hotbar_slot - 1].id);
	}

	void draw_hotbar_items(InventoryData &inventory, Renderer &render)
	{
		for (int i = 0; i < 9; ++i)
		{
			if (inventory.items[i].id != BlockID::NONE)
			{
				Render::draw_3d_hotbar_item(render, inventory.items[i].id, i + 1);
			}
		}
	}
}
