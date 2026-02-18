#pragma once

#include <shader.hpp>
#include <world.hpp>
#include <block.hpp>

#include <iostream>
#include <cmath>

const float DEFAULT_RANGE = 5.0f;
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

struct CameraData
{
	glm::vec3 pos;
	glm::vec3 dir;
	glm::vec3 up;

	float yaw;
	float pitch;

	float range;

	float speed;
	float fallSpeed;
	float fallAccel;
	bool shouldJump;
	unsigned char move_bitmap;

	float playerWidth;
	float playerHeight;
	float playerHeightOffset;

	BlockID selected_block;
};

namespace Camera
{
	int init(CameraData &camera);
	void update(CameraData &camera, WorldData &world, float deltaTime);
	void move_cam(CameraData &camera, double dx, double dy);
	void queue_horz_move(CameraData &camera, CameraDir dir);
	glm::vec3 move_bitmap_to_move_v3(CameraData &camera);
	void update_pos(CameraData &camera, WorldData &world, float deltaTime);
	void place_block(CameraData &camera, WorldData &world);
	void break_block(CameraData &camera, WorldData &world);
	void draw_wire(CameraData &camera, WorldData &world, Renderer &render);
}
