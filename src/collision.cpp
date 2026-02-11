#include <collision.hpp>



namespace Collision
{
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

	// TODO: rename to aabb_is_colliding
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

	std::vector<std::pair<glm::vec3, BlockCoords>> get_all_collision_norms(CameraData &camera, WorldData &world)
	{
		// p[xyz] -> player [xyz]
		int px = std::round(camera.pos.x);
		int py = std::round(camera.pos.y);
		int pz = std::round(camera.pos.z);
		
		std::vector<std::pair<glm::vec3, BlockCoords>> res;
		AABB player_box = Collision::make_player_aabb(camera.pos, camera.playerWidth, camera.playerHeight, camera.playerHeightOffset);
		// loop over three by three area around player
		for (int bz = pz - 1; bz <= pz + 1; ++bz) {
			for (int by = py - 3; by <= py + 1; ++by) { // TODO: make 3 dependent on player height
				for (int bx = px - 1; bx <= px + 1; ++bx) {
					if (World::at(world, bx, by, bz) == BlockID::NONE)
						continue;
					AABB block_box  = Collision::make_block_aabb(bx, by, bz);
					if (Collision::is_colliding(player_box, block_box)) {
						glm::vec3 collision_norm = Collision::get_collision_normal(player_box, block_box);
						BlockCoords coords = { bx, by, bz };
						//std::pair<glm::vec3, BlockID> norm_and_coords = { collision_norm, {bx, by, bz} };
						res.emplace_back(collision_norm, coords);
					}
				}
			}
		}

		return res;
	}

	RayFace draw_ray_to_block(glm::vec3 origin, glm::vec3 ray, AABB aabb)
	{
		float x_low  = aabb.min.x;
		float x_high = aabb.max.x;
		float y_low  = aabb.min.y;
		float y_high = aabb.max.y;
		float z_low  = aabb.min.z;
		float z_high = aabb.max.z;

		float o_x = origin.x;
		float o_y = origin.y;
		float o_z = origin.z;

		float r_x = ray.x;
		float r_y = ray.y;
		float r_z = ray.z;

		float t_x_low  = (x_low  - o_x) / r_x;
		float t_x_high = (x_high - o_x) / r_x;

		float t_y_low  = (y_low  - o_y) / r_y;
		float t_y_high = (y_high - o_y) / r_y;

		float t_z_low  = (z_low  - o_z) / r_z;
		float t_z_high = (z_high - o_z) / r_z;

		float t_x_close, t_x_far;
		Face t_x_close_sel;

		if (t_x_low < t_x_high) {
			t_x_close = t_x_low;
			t_x_far   = t_x_high;
			t_x_close_sel = Face::LOW;
		} else {
			t_x_close = t_x_high;
			t_x_far   = t_x_low;
			t_x_close_sel = Face::HIGH;
		}

		float t_y_close, t_y_far;
		Face t_y_close_sel;

		if (t_y_low < t_y_high) {
			t_y_close = t_y_low;
			t_y_far   = t_y_high;
			t_y_close_sel = Face::LOW;
		} else {
			t_y_close = t_y_high;
			t_y_far   = t_y_low;
			t_y_close_sel = Face::HIGH;
		}

		float t_z_close, t_z_far;
		Face t_z_close_sel;

		if (t_z_low < t_z_high) {
			t_z_close = t_z_low;
			t_z_far   = t_z_high;
			t_z_close_sel = Face::LOW;
		} else {
			t_z_close = t_z_high;
			t_z_far   = t_z_low;
			t_z_close_sel = Face::HIGH;
		}

		//float t_close = std::max(t_x_close, std::max(t_y_close, t_z_close));
		float t_close;
		Axis t_close_axis;
		Face t_close_sel;
		if (t_x_close > t_y_close && t_x_close > t_z_close) {
			t_close = t_x_close;
			t_close_axis = Axis::X_AXIS;
			t_close_sel = t_x_close_sel;
		} else if (t_y_close > t_z_close) {
			t_close = t_y_close;
			t_close_axis = Axis::Y_AXIS;
			t_close_sel = t_y_close_sel;
		} else {
			t_close = t_z_close;
			t_close_axis = Axis::Z_AXIS;
			t_close_sel = t_z_close_sel;
		}
		float t_far   = std::min(t_x_far, std::min(t_y_far, t_z_far));

		if (t_close > 0 && t_close < t_far) {
			return { t_close_axis, t_close_sel, t_close, block_aabb_to_coords(aabb) };
		}
		else {
			return { Axis::INVALID, Face::INVALID, 0.0f, { 0, 0, 0 } };
		}
	}

	BlockCoords vec3_to_coords(glm::vec3 v)
	{
		return {
			static_cast<int>(std::round(v.x)),
			static_cast<int>(std::round(v.y)),
			static_cast<int>(std::round(v.z)),
		};
	}

	

	RayFace draw_ray_through_world(glm::vec3 o, glm::vec3 r, WorldData &world, float place_range)
	{
		// float place_range = 5.0f;

		// glm::vec3 o = camera.pos;
		// glm::vec3 r = camera.dir;
		float t = 0.0f;

		// printf("origin: (%f, %f, %f)\n", o.x, o.y, o.z);
		// printf("ray: (%f, %f, %f)\n", r.x, r.y, r.z);

		BlockCoords c = vec3_to_coords(o);

		ivec3 step = { r.x >= 0.0f ? 1 : -1,
					   r.y >= 0.0f ? 1 : -1,
					   r.z >= 0.0f ? 1 : -1 };

		// printf("step: (%d, %d, %d)\n", step.x, step.y, step.z);

		while (t <= place_range) {
			// printf("in loop t: %f\n", t);
			// printf("checking c: (%d, %d, %d)\n", c.x, c.y, c.z);
			if (World::at(world, c.x, c.y, c.z) != BlockID::NONE) {
				RayFace rayface = Collision::draw_ray_to_block(o, r, Collision::make_block_aabb(c.x, c.y, c.z));

				if (rayface.axis != Axis::INVALID && rayface.face != Face::INVALID) {
					// TODO:
					printf("Found: x: %d, y: %d, z: %d\n", c.x, c.y, c.z);
					return rayface;
				}
			}

			float target_x = c.x + (BLOCK_WIDTH / 2) * step.x;
			float target_y = c.y + (BLOCK_WIDTH / 2) * step.y;
			float target_z = c.z + (BLOCK_WIDTH / 2) * step.z;

			// printf("target_x: %f\n", target_x);
			// printf("target_y: %f\n", target_y);
			// printf("target_z: %f\n", target_z);

			float t_x = (target_x  - o.x) / r.x;
			float t_y = (target_y  - o.y) / r.y;
			float t_z = (target_z  - o.z) / r.z;

			// printf("t_x: %f\n", t_x);
			// printf("t_y: %f\n", t_y);
			// printf("t_z: %f\n", t_z);
			
			if (t_x < t_y && t_x < t_z) {
				c.x += step.x;
				t = t_x;
			} else if (t_y < t_z) {
				c.y += step.y;
				t = t_y;
			} else {
				c.z += step.z;
				t = t_z;
			}
			if (t > place_range) {
				printf("out of range\n");
				return {};
			}
		}

		return { Axis::INVALID, Face::INVALID, 0.0f, { 0, 0, 0 } };

	}

	BlockCoords block_aabb_to_coords(AABB aabb)
	{
		int x = std::round((aabb.min.x + aabb.max.x) / 2);
		int y = std::round((aabb.min.y + aabb.max.y) / 2);
		int z = std::round((aabb.min.z + aabb.max.z) / 2);

		return { x, y, z };
	}
}


std::ostream& operator<<(std::ostream& os, Face f) {
    switch (f) {
	case Face::INVALID:   return os << "INVALID";
	case Face::LOW: return os << "LOW";
	case Face::HIGH:  return os << "HIGH";
        default:           return os << "Unknown";
    }
}

std::ostream& operator<<(std::ostream& os, Axis a) {
    switch (a) {
	case Axis::INVALID:   return os << "INVALID";
	case Axis::X_AXIS: return os << "X_AXIS";
	case Axis::Y_AXIS: return os << "Y_AXIS";
	case Axis::Z_AXIS: return os << "Z_AXIS";

        default:           return os << "Unknown";
    }
}
