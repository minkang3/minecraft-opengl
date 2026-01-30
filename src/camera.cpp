#include <camera.hpp>

#include <collision.hpp>

namespace Camera
{
	int init(CameraData &camera)
	{
		camera.pos = glm::vec3(0.0f, 5.0f, 0.0f);
		camera.dir = glm::vec3(0.0f, 0.0f, -1.0f);
		camera.up  = glm::vec3(0.0f, 1.0f, 0.0f);

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
		camera.dir = dir;

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

	glm::vec3 move_bitmap_to_move_v3(CameraData &camera)
	{
		glm::vec3 dir = camera.dir;
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
			move -= glm::cross(dir, camera.up);
		}
		if (camera.move_bitmap & (1 << RIGHT)) {
			move += glm::cross(dir, camera.up);
		}

		camera.move_bitmap = 0;
		
		return glm::length(move) > 0.01 ? glm::normalize(move) : move;
	}

	void update_pos(CameraData &camera, WorldData &world, float deltaTime)
	{
		glm::vec3 moveVector = move_bitmap_to_move_v3(camera);
		moveVector.x = moveVector.x * camera.speed;
		moveVector.z = moveVector.z * camera.speed;
		moveVector.y = -camera.fallSpeed;

		camera.fallSpeed += deltaTime * camera.fallAccel; // TODO: move this somewhere nicer

		camera.pos.y += moveVector.y * deltaTime;
		std::vector<std::pair<glm::vec3, BlockCoords>> norms_and_blocks = Collision::get_all_collision_norms(camera, world);

		for (const auto &[norm, block] : norms_and_blocks) {
			// block is +y and moving +y
			if (norm.y > 0.01f && moveVector.y > 0.01f) {
				//moveVector.y = -0.0f;
				camera.fallSpeed = 0;
			}
			// block is -y and moving -y
			else if (norm.y < -0.01f && moveVector.y < -0.01f) {
				//moveVector.y = 0.0f;
				camera.pos.y = (float)block.y + (BLOCK_WIDTH / 2.0f + camera.playerHeight - camera.playerHeightOffset);
				if (!(camera.fallSpeed < 0))
					camera.fallSpeed = 0;
				if (camera.shouldJump) {
					camera.fallSpeed = -8.0f;
				}
			}
		}

		camera.pos.x += moveVector.x * deltaTime;
		norms_and_blocks = Collision::get_all_collision_norms(camera, world);

		for (const auto &[norm, block] : norms_and_blocks) {
			// block is -x and moving -x
			if (norm.x < -0.01f && moveVector.x < -0.01f) {
				//moveVector.x = 0.0f;
				camera.pos.x = (float)block.x + (BLOCK_WIDTH / 2.0f + camera.playerWidth / 2.0f);
			}
			// block is +x and moving +x
			else if (norm.x > 0.01f && moveVector.x > 0.01f) {
				//moveVector.x = -0.0f;
				camera.pos.x = (float)block.x - (BLOCK_WIDTH / 2.0f + camera.playerWidth / 2.0f);
			}
		}

		camera.pos.z += moveVector.z * deltaTime;
		norms_and_blocks = Collision::get_all_collision_norms(camera, world);

		for (const auto &[norm, block] : norms_and_blocks) {
			// block is -z and moving -z
			if (norm.z < -0.01f && moveVector.z < -0.01f) {
				//moveVector.z = 0;
				camera.pos.z = (float)block.z + (BLOCK_WIDTH / 2.0f + camera.playerWidth / 2.0f);
			}
			// block is +z and moving +z
			else if (norm.z > 0.01f && moveVector.z > 0.01f) {
				//moveVector.z = 0;
				camera.pos.z = (float)block.z - (BLOCK_WIDTH / 2.0f + camera.playerWidth / 2.0f);
			}
		}
		camera.shouldJump = false;
		// std::cout << "postcalc:\n";
		// std::cout << "x: " << camera.pos.x << "\n";
		// std::cout << "y: " << camera.pos.y << "\n";
		// std::cout << "z: " << camera.pos.z << std::endl;
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
					AABB aabb = Collision::make_block_aabb(x, y, z);
					RayFace rayface = Collision::draw_ray_to_block(camera.pos, camera.dir, aabb);
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
}
