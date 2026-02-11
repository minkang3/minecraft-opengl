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
		RayFace rayface = Collision::draw_ray_through_world(camera.pos, camera.dir, world, 5.0f); // TODO: make 5.0f into variable place_range

		if (rayface.axis == Axis::INVALID && rayface.face == Face::INVALID) {
			return;
		}

		BlockCoords new_coords = rayface.coords;
		int delta = 1;
		if (rayface.face == Face::LOW)
			delta *= -1;

		switch (rayface.axis) {
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
	}

	void draw_grid(CameraData &camera, WorldData &world, BlockRender &render, unsigned int VAO)
	{
		RayFace rayface = Collision::draw_ray_through_world(camera.pos, camera.dir, world, 5.0f); // TODO: make 5.0f into variable place_range

		if (rayface.axis == Axis::INVALID && rayface.face == Face::INVALID) {
			return;
		}

		const float SCALE_AMT = 0.0001f;
		glm::mat4 model = glm::mat4(1.0f);
		glm::vec3 posVec = glm::vec3((float)rayface.coords.x,
									 (float)rayface.coords.y,
									 (float)rayface.coords.z);

		model = glm::translate(model, posVec);
		model = glm::scale(model, glm::vec3(1.0f + SCALE_AMT));

		Shader::use(render.shaderID);
		Shader::setMat4(render.shaderID, "model", model);

		glBindVertexArray(VAO);
		glLineWidth(8.0f);
		glDrawArrays(GL_TRIANGLES, 0, 6*24);
	}
}
