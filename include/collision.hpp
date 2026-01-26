#pragma once

#include <glm/glm.hpp>
#include <world.hpp>

#define BLOCK_WIDTH (1.0f)

struct AABB
{
	glm::vec3 min;
	glm::vec3 max;
};

AABB make_player_aabb(const glm::vec3 &camPos, float width, float height, float height_offset);
AABB make_block_aabb(const Block &block);
bool is_colliding(const AABB &a, const AABB &b);
glm::vec3 get_collision_normal(const AABB &a, const AABB &b);
