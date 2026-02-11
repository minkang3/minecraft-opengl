#pragma once

#include <glm/glm.hpp>
#include <world.hpp>
#include <camera.hpp>
#include <block.hpp>

#define BLOCK_WIDTH (1.0f)

typedef BlockCoords ivec3;

struct AABB
{
	glm::vec3 min;
	glm::vec3 max;
};

enum class Face {
	INVALID,
	LOW,
	HIGH
};

enum class Axis {
	INVALID,
	X_AXIS,
	Y_AXIS,
	Z_AXIS
};

struct RayFace
{
	Axis axis;
	Face face;
	float t;
	BlockCoords coords;
};

namespace Collision
{
	AABB make_player_aabb(const glm::vec3 &camPos, float width, float height, float height_offset);
	AABB make_block_aabb(int x, int y, int z);
	bool is_colliding(const AABB &a, const AABB &b);
	glm::vec3 get_collision_normal(const AABB &a, const AABB &b);
	std::vector<std::pair<glm::vec3, BlockCoords>> get_all_collision_norms(CameraData &camera, WorldData &world);
	RayFace draw_ray_to_block(glm::vec3 origin, glm::vec3 ray, AABB aabb);
	RayFace draw_ray_through_world(glm::vec3 o, glm::vec3 r, WorldData &world, float place_range);
	BlockCoords block_aabb_to_coords(AABB aabb);
}

std::ostream& operator<<(std::ostream& os, Face f);
std::ostream& operator<<(std::ostream& os, Axis a);
