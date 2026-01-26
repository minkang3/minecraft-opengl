#include <collision.hpp>



AABB make_player_aabb(const glm::vec3 &camPos, float width, float height, float height_offset)
{
	glm::vec3 min, max;

	min.x = camPos.x - width / 2.0f;
	min.z = camPos.z - width / 2.0f;
	max.x = camPos.x + width / 2.0f;
	max.z = camPos.z + width / 2.0f;

	min.y = camPos.y - (height - height_offset);
	max.y = camPos.y + height_offset;

	return { min, max };
}

AABB make_block_aabb(int x, int y, int z)
{
	glm::vec3 min, max;

	min.x = x - BLOCK_WIDTH / 2.0f;
	min.y = y - BLOCK_WIDTH / 2.0f;
	min.z = z - BLOCK_WIDTH / 2.0f;

	max.x = x + BLOCK_WIDTH / 2.0f;
	max.y = y + BLOCK_WIDTH / 2.0f;
	max.z = z + BLOCK_WIDTH / 2.0f;

	return { min, max };
}

bool is_colliding(const AABB &a, const AABB &b)
{
	// x overlap, y overlap, z overlap
	bool is_x_ol = !(a.max.x < b.min.x || b.max.x < a.min.x);
	bool is_y_ol = !(a.max.y < b.min.y || b.max.y < a.min.y);
	bool is_z_ol = !(a.max.z < b.min.z || b.max.z < a.min.z);

	return is_x_ol && is_y_ol && is_z_ol;
}

glm::vec3 get_collision_normal(const AABB &a, const AABB &b)
{
	float x_ol = std::min(a.max.x, b.max.x) - std::max(a.min.x, b.min.x);
	float y_ol = std::min(a.max.y, b.max.y) - std::max(a.min.y, b.min.y);
	float z_ol = std::min(a.max.z, b.max.z) - std::max(a.min.z, b.min.z);

	float y_bias = 0.000f;

	if (y_ol < z_ol + y_bias && y_ol < x_ol + y_bias) {
		return (a.min.y < b.min.y) ? glm::vec3(0.0f, 1.0f, 0.0f) : glm::vec3(0.0f, -1.0f, 0.0f);
	}
	if (x_ol < z_ol) {
		return (a.min.x < b.min.x) ? glm::vec3(1.0f, 0.0f, 0.0f) : glm::vec3(-1.0f, 0.0f, 0.0f);
	}
	else {
		return (a.min.z < b.min.z) ? glm::vec3(0.0f, 0.0f, 1.0f) : glm::vec3(0.0f, 0.0f, -1.0f);
	}
}
