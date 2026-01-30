#include <camera.hpp>

namespace Camera
{
	int init(CameraData &camera)
	{
		camera.cameraPos = glm::vec3(0.0f, 5.0f, 0.0f);
		camera.cameraDir = glm::vec3(0.0f, 0.0f, -1.0f);
		camera.cameraUp  = glm::vec3(0.0f, 1.0f, 0.0f);

		camera.speed = DEFAULT_SPEED;
		camera.fallSpeed = 0.0f;
		camera.fallAccel = 20.0f;
		camera.yaw = -90.0f;
		camera.pitch = 0.0f;

		camera.playerWidth = DEFAULT_PLAYER_WIDTH;
		camera.playerHeight = DEFAULT_PLAYER_HEIGHT;
		camera.playerHeightOffset = DEFAULT_PLAYER_HEIGHT_OFFSET;

		return 0;
	}

	void update(CameraData &camera, WorldData &world, float deltaTime)
	{
		glm::vec3 dir;
		dir.x = cos(glm::radians(camera.yaw)) * cos(glm::radians(camera.pitch));
		dir.y = sin(glm::radians(camera.pitch));
		dir.z = sin(glm::radians(camera.yaw)) * cos(glm::radians(camera.pitch));
		camera.cameraDir = dir;

		update_pos(camera, world, deltaTime);
	}

	void move_cam(CameraData &camera, double dx, double dy)
	{
		camera.yaw += (float) dx * 0.01f; // TODO: make 0.01f a sensitivity variale
		camera.pitch -= (float) dy * 0.01f;

		if (camera.pitch > 89.0f)
			camera.pitch = 89.0f;
		if (camera.pitch < -89.0f)
			camera.pitch = -89.0f;
	}

	void queue_horz_move(CameraData &camera, CameraDir dir)
	{
		switch (dir) {
		case FORWARD:
			camera.move_bitmap |= (1 << FORWARD);
			break;
		case BACKWARD:
			camera.move_bitmap |= (1 << BACKWARD);
			break;
		case LEFT:
			camera.move_bitmap |= (1 << LEFT);
			break;
		case RIGHT:
			camera.move_bitmap |= (1 << RIGHT);
			break;
		}
	}

	glm::vec3 calc_horz_move_vector(CameraData &camera)
	{
		glm::vec3 dir = camera.cameraDir;
		dir.y = 0;
		dir = glm::normalize(dir);

		glm::vec3 move(0.0f, 0.0f, 0.0f);

		if (camera.move_bitmap & (1 << FORWARD)) {
			move += dir;
		}
		if (camera.move_bitmap & (1 << BACKWARD)) {
			move -= dir;
		}
		if (camera.move_bitmap & (1 << LEFT)) {
			move -= glm::cross(dir, camera.cameraUp);
		}
		if (camera.move_bitmap & (1 << RIGHT)) {
			move += glm::cross(dir, camera.cameraUp);
		}

		camera.move_bitmap = 0;
		
		return glm::length(move) > 0.01 ? glm::normalize(move) : move;
	}

	void update_pos(CameraData &camera, WorldData &world, float deltaTime)
	{
		glm::vec3 moveVector = calc_horz_move_vector(camera);
		moveVector.x = moveVector.x * camera.speed;
		moveVector.z = moveVector.z * camera.speed;
		moveVector.y = -camera.fallSpeed;

		camera.fallSpeed += deltaTime * camera.fallAccel; // TODO: move this somewhere nicer

		camera.cameraPos.y += moveVector.y * deltaTime;
		std::vector<std::pair<glm::vec3, BlockCoords>> norms_and_blocks = get_all_collision_norms(camera, world);

		for (const auto &[norm, block] : norms_and_blocks) {
			// block is +y and moving +y
			if (norm.y > 0.01f && moveVector.y > 0.01f) {
				//moveVector.y = -0.0f;
				camera.fallSpeed = 0;
			}
			// block is -y and moving -y
			else if (norm.y < -0.01f && moveVector.y < -0.01f) {
				//moveVector.y = 0.0f;
				camera.cameraPos.y = (float)block.y + (BLOCK_WIDTH / 2.0f + camera.playerHeight - camera.playerHeightOffset);
				if (!(camera.fallSpeed < 0))
					camera.fallSpeed = 0;
				if (camera.shouldJump) {
					camera.fallSpeed = -8.0f;
				}
			}
		}

		camera.cameraPos.x += moveVector.x * deltaTime;
		norms_and_blocks = get_all_collision_norms(camera, world);

		for (const auto &[norm, block] : norms_and_blocks) {
			// block is -x and moving -x
			if (norm.x < -0.01f && moveVector.x < -0.01f) {
				//moveVector.x = 0.0f;
				camera.cameraPos.x = (float)block.x + (BLOCK_WIDTH / 2.0f + camera.playerWidth / 2.0f);
			}
			// block is +x and moving +x
			else if (norm.x > 0.01f && moveVector.x > 0.01f) {
				//moveVector.x = -0.0f;
				camera.cameraPos.x = (float)block.x - (BLOCK_WIDTH / 2.0f + camera.playerWidth / 2.0f);
			}
		}

		camera.cameraPos.z += moveVector.z * deltaTime;
		norms_and_blocks = get_all_collision_norms(camera, world);

		for (const auto &[norm, block] : norms_and_blocks) {
			// block is -z and moving -z
			if (norm.z < -0.01f && moveVector.z < -0.01f) {
				//moveVector.z = 0;
				camera.cameraPos.z = (float)block.z + (BLOCK_WIDTH / 2.0f + camera.playerWidth / 2.0f);
			}
			// block is +z and moving +z
			else if (norm.z > 0.01f && moveVector.z > 0.01f) {
				//moveVector.z = 0;
				camera.cameraPos.z = (float)block.z - (BLOCK_WIDTH / 2.0f + camera.playerWidth / 2.0f);
			}
		}
		camera.shouldJump = false;
		// std::cout << "postcalc:\n";
		// std::cout << "x: " << camera.cameraPos.x << "\n";
		// std::cout << "y: " << camera.cameraPos.y << "\n";
		// std::cout << "z: " << camera.cameraPos.z << std::endl;
	}

	std::vector<std::pair<glm::vec3, BlockCoords>> get_all_collision_norms(CameraData &camera, WorldData &world)
	{
		// p[xyz] -> player [xyz]
		int px = std::round(camera.cameraPos.x);
		int py = std::round(camera.cameraPos.y);
		int pz = std::round(camera.cameraPos.z);
		
		std::vector<std::pair<glm::vec3, BlockCoords>> res;
		AABB player_box = make_player_aabb(camera.cameraPos, camera.playerWidth, camera.playerHeight, camera.playerHeightOffset);
		// loop over three by three area around player
		for (int bz = pz - 1; bz <= pz + 1; ++bz) {
			for (int by = py - 3; by <= py + 1; ++by) { // TODO: make 3 dependent on player height
				for (int bx = px - 1; bx <= px + 1; ++bx) {
					if (World::at(world, bx, by, bz) == BlockID::NONE)
						continue;
					AABB block_box  = make_block_aabb(bx, by, bz);
					if (is_colliding(player_box, block_box)) {
						glm::vec3 collision_norm = get_collision_normal(player_box, block_box);
						BlockCoords coords = { bx, by, bz };
						//std::pair<glm::vec3, BlockID> norm_and_coords = { collision_norm, {bx, by, bz} };
						res.emplace_back(collision_norm, coords);
					}
				}
			}
		}

		return res;
	}

	void place_block(CameraData &camera, WorldData &world)
	{
		std::vector<std::pair<RayFace, BlockCoords>> rayfaces;
		// need to draw a ray
		// need to find which block it hits and what face
		for (int z = world.zmin; z < world.zmin + world.zsize; ++z) {
			for (int y = world.ymin; y < world.ymin + world.ysize; ++y) {
				for (int x = world.xmin; x < world.xmin + world.xsize; ++x) {
					if (World::at(world, x, y, z) == BlockID::NONE)
						continue;
					AABB aabb = make_block_aabb(x, y, z);
					RayFace rayface = draw_ray_to_block(camera, aabb);
					if (rayface.axis == Axis::INVALID && rayface.face == Face::INVALID) {
						//std::cout << "invalid" << std::endl;
						continue;
					}
					BlockCoords coords = { x, y, z };
					rayfaces.emplace_back(rayface, coords);
				}
			}
		}

		if (rayfaces.size() <= 0)
			return;

		RayFace closest_rayface;
		BlockCoords closest_coords;
		closest_rayface.t = 1.0f / 0.0f; // infinity
		for (auto &[rayface, coords] : rayfaces) {
			if (rayface.t <  closest_rayface.t) {
				closest_rayface = rayface;
				closest_coords = coords;
			}
		}
		std::cout << "(" << closest_coords.x << ", " << closest_coords.y << ", " << closest_coords.z << ")" << std::endl;
		std::cout << "axis: " << closest_rayface.axis << ", face: " << closest_rayface.face << "\n" << std::endl;

		BlockCoords new_coords = closest_coords;
		int delta = 1;
		if (closest_rayface.face == Face::LOW)
			delta *= -1;

		switch (closest_rayface.axis) {
		case Axis::X_AXIS:
			new_coords.x += delta;
			break;
		case Axis::Z_AXIS:
			new_coords.z += delta;
			break;
		case Axis::Y_AXIS:
			new_coords.y += delta;
			break;
		}

		World::at(world, new_coords.x, new_coords.y, new_coords.z) = BlockID::STONE;

		std::cout << "new coords:" << std::endl;
		std::cout << "(" << new_coords.x << ", " << new_coords.y << ", " << new_coords.z << ")" << std::endl;
	}

	RayFace draw_ray_to_block(CameraData &camera, AABB aabb)
	{
		float x_low  = aabb.min.x;
		float x_high = aabb.max.x;
		float y_low  = aabb.min.y;
		float y_high = aabb.max.y;
		float z_low  = aabb.min.z;
		float z_high = aabb.max.z;

		float o_x = camera.cameraPos.x;
		float o_y = camera.cameraPos.y;
		float o_z = camera.cameraPos.z;

		float r_x = camera.cameraDir.x;
		float r_y = camera.cameraDir.y;
		float r_z = camera.cameraDir.z;

		float t_x_low  = (x_low  - o_x) / r_x;
		float t_x_high = (x_high - o_x) / r_x;

		float t_y_low  = (y_low  - o_y) / r_y;
		float t_y_high = (y_high - o_y) / r_y;

		float t_z_low  = (z_low  - o_z) / r_z;
		float t_z_high = (z_high - o_z) / r_z;

		// float t_x_close = std::min(t_x_low, t_x_high);
		// float t_x_far   = std::max(t_x_low, t_x_high);

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

		//std::cout << "t_close: " << t_close << ", t_far: " << t_far << std::endl;
		if (t_close > 0 && t_close < t_far) {
			//std::cout << "branch 1" << std::endl;
			return { t_close_axis, t_close_sel, t_close };
		}
		else {
			//std::cout << "branch 2" << std::endl;
			return { Axis::INVALID, Face::INVALID, 0.0f };
		}
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
