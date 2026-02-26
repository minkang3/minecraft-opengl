#pragma once

#include <block.hpp>
#include <render.hpp>
#include <camera.hpp>
#include <cstdint>
#include <vector>

struct InventoryItem
{
	BlockID id;
	uint8_t quantity;
};

struct InventoryData
{
	unsigned int hotbar_slot = 1;
	std::vector<InventoryItem> items;
};

namespace Inventory
{
	void init(InventoryData &inventory);
	void hotbar_select_slot(InventoryData &inventory, unsigned int slot_num);
	void place_block(InventoryData &inventory, CameraData &camera, WorldData &world);
	void draw_hotbar_items(InventoryData &inventory, Renderer &render);
}
