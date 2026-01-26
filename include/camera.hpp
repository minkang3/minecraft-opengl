#pragma once

#include <cmath>

#include <shader.h>
#include <world.hpp>
#include <collision.hpp>
#include <iostream>

const float DEFAULT_SPEED = 5.0f;
const float DEFAULT_PLAYER_WIDTH = 0.7f;
const float DEFAULT_PLAYER_HEIGHT = 1.9f;
const float DEFAULT_PLAYER_HEIGHT_OFFSET = 0.4f;

enum CameraDir {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT,
	NONE
};

class Camera
{
	//private:
public:
	glm::mat4 view;
	glm::vec3 cameraPos;
	glm::vec3 cameraDir;
	glm::vec3 cameraUp;

	glm::vec3 moveVector;

	float fallSpeed;
	float fallAccel;

	float playerWidth;
	float playerHeight;
	float playerHeightOffset;

	bool should_jump;


public:
	float yaw;
	float pitch;
	float speed;
	Camera()
	{
		view = glm::mat4(1.0f);
		cameraPos = glm::vec3(0.0f, 5.0f, 0.0f);
		cameraDir = glm::vec3(0.0f, 0.0f, -1.0f);
		cameraUp  = glm::vec3(0.0f, 1.0f, 0.0f);
		view = glm::lookAt(cameraPos, cameraDir, cameraUp);

		speed = DEFAULT_SPEED;
		fallSpeed = 0.0f;
		fallAccel = 20.0f;
		yaw = -90.0f;
		pitch = 0.0f;

		playerWidth = DEFAULT_PLAYER_WIDTH;
		playerHeight = DEFAULT_PLAYER_HEIGHT;
		playerHeightOffset = DEFAULT_PLAYER_HEIGHT_OFFSET;
	}

	glm::mat4 get_view()
	{
		return view;
	}

	glm::vec3 get_pos()
	{
		return cameraPos;
	}

	void update()
	{
		view = glm::lookAt(cameraPos, cameraPos + cameraDir, cameraUp);
		glm::vec3 dir;
		dir.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		dir.y = sin(glm::radians(pitch));
		dir.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
		cameraDir = dir;
	}

	void reset_move()
	{
		moveVector = glm::vec3(0.0f, 0.0f, 0.0f);
	}

	void move(float deltaTime, World world)
	{
		moveVector.x = moveVector.x * speed;
		moveVector.z = moveVector.z * speed;

		// cameraPos += moveVector * deltaTime;

		// std::cout << "precalc:\n";
		// std::cout << "x: " << cameraPos.x << "\n";
		// std::cout << "y: " << cameraPos.y << "\n";
		// std::cout << "z: " << cameraPos.z << std::endl;


		// if (norms_and_blocks.size() > 0) {
		// 	std::cout << "collision detected" << std::endl;
		// 	std::cout << "size: " << norms_and_blocks.size() << std::endl;
		// }

		fallSpeed += deltaTime * fallAccel; // TODO: move this somewhere nicer

		cameraPos.y += moveVector.y * deltaTime;
		std::vector<std::pair<glm::vec3, BlockCoords>> norms_and_blocks = get_all_collision_norms(world);

		for (const auto &[norm, block] : norms_and_blocks) {
			// block is +y and moving +y
			if (norm.y > 0.01f && moveVector.y > 0.01f) {
				//moveVector.y = -0.0f;
				fallSpeed = 0;
			}
			// block is -y and moving -y
			else if (norm.y < -0.01f && moveVector.y < -0.01f) {
				//moveVector.y = 0.0f;
				cameraPos.y = (float)block.y + (BLOCK_WIDTH / 2.0f + playerHeight - playerHeightOffset);
				if (!(fallSpeed < 0))
					fallSpeed = 0;
				if (should_jump) {
					fallSpeed = -8.0f;
				}
			}
		}

		cameraPos.x += moveVector.x * deltaTime;
		norms_and_blocks = get_all_collision_norms(world);

		for (const auto &[norm, block] : norms_and_blocks) {
			// block is -x and moving -x
			if (norm.x < -0.01f && moveVector.x < -0.01f) {
				//moveVector.x = 0.0f;
				cameraPos.x = (float)block.x + (BLOCK_WIDTH / 2.0f + playerWidth / 2.0f);
			}
			// block is +x and moving +x
			else if (norm.x > 0.01f && moveVector.x > 0.01f) {
				//moveVector.x = -0.0f;
				cameraPos.x = (float)block.x - (BLOCK_WIDTH / 2.0f + playerWidth / 2.0f);
			}
		}

		cameraPos.z += moveVector.z * deltaTime;
		norms_and_blocks = get_all_collision_norms(world);

		for (const auto &[norm, block] : norms_and_blocks) {
			// block is -z and moving -z
			if (norm.z < -0.01f && moveVector.z < -0.01f) {
				//moveVector.z = 0;
				cameraPos.z = (float)block.z + (BLOCK_WIDTH / 2.0f + playerWidth / 2.0f);
			}
			// block is +z and moving +z
			else if (norm.z > 0.01f && moveVector.z > 0.01f) {
				//moveVector.z = 0;
				cameraPos.z = (float)block.z - (BLOCK_WIDTH / 2.0f + playerWidth / 2.0f);
			}
		}
		should_jump = false;
		// std::cout << "postcalc:\n";
		// std::cout << "x: " << cameraPos.x << "\n";
		// std::cout << "y: " << cameraPos.y << "\n";
		// std::cout << "z: " << cameraPos.z << std::endl;
	}

	void move_horz(CameraDir dir, float deltaTime)
	{
		glm::mat3 stripY = glm::mat3
			(1.0f, 0.0f, 0.0f,
			 0.0f, 0.0f, 0.0f,
			 0.0f, 0.0f, 1.0f);

		glm::vec3 moveDir = glm::normalize(stripY * cameraDir);

		switch (dir) {
		case FORWARD:
			break;
		case BACKWARD:
			moveDir *= -1;
			break;
		case LEFT:
			moveDir = -glm::cross(moveDir, cameraUp);
			break;
		case RIGHT:
			moveDir =  glm::cross(moveDir, cameraUp);
			break;
		case NONE:
			moveVector = glm::vec3(0.0f, 0.0f, 0.0f);
			return;
		}

		moveVector += moveDir;
		moveVector = glm::normalize(moveVector);
	}

	void fall(World &world, float deltaTime)
	{
		moveVector.y = -fallSpeed;
	}

	void jump()
	{
		should_jump = true;
	}

	void move_cam(double dx, double dy)
	{
		yaw += (float) dx * 0.01f;
		pitch -= (float) dy * 0.01f;

		if (pitch > 89.0f)
			pitch = 89.0f;
		if (pitch < -89.0f)
			pitch = -89.0f;
	}

	// bool is_above_block(World &world)
	// {
	// 	if (fallSpeed < 0)
	// 		return false;

	// 	int x = std::round(cameraPos.x);
	// 	int y = std::round(cameraPos.y);
	// 	int z = std::round(cameraPos.z);

	// 	for (const BlockID &b : world.blocks) {
	// 		if (x == b.x && z == b.z && cameraPos.y <= (float)(b.y + 2) && cameraPos.y >= (float)(b.y + 1.9)) {
	// 			fallSpeed = 0.0f;
	// 			return true;
	// 		}
	// 	}
	// 	return false;
	// }

	// TODO: change name to something more fitting "check_around_player" "get_collision_norms_around_player"
	std::vector<std::pair<glm::vec3, BlockCoords>> get_all_collision_norms(World &world)
	{
		int px = std::round(cameraPos.x);
		int py = std::round(cameraPos.y);
		int pz = std::round(cameraPos.z);
		
		std::vector<std::pair<glm::vec3, BlockCoords>> res;
		AABB player_box = make_player_aabb(cameraPos, playerWidth, playerHeight, playerHeightOffset);
		// loop over three by three area around player
		for (int bz = pz - 1; bz <= pz + 1; ++bz) {
			for (int by = py - 3; by <= py + 1; ++by) { // TODO: make 3 dependent on player height
				for (int bx = px - 1; bx <= px + 1; ++bx) {
					if (world(bx, by, bz) == BlockID::NONE)
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
};
