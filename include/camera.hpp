#pragma once

#include <shader.hpp>
#include <world.hpp>
#include <collision.hpp>

#include <iostream>
#include <cmath>

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
};

std::ostream& operator<<(std::ostream& os, Face f);
std::ostream& operator<<(std::ostream& os, Axis a);

struct CameraData
{
	glm::vec3 pos;
	glm::vec3 dir;
	glm::vec3 up;

	float yaw;
	float pitch;

	float speed;
	float fallSpeed;
	float fallAccel;
	bool shouldJump;
	unsigned char move_bitmap;

	float playerWidth;
	float playerHeight;
	float playerHeightOffset;
};

namespace Camera
{
	int init(CameraData &camera);
	void update(CameraData &camera, WorldData &world, float deltaTime);
	void move_cam(CameraData &camera, double dx, double dy);
	void queue_horz_move(CameraData &camera, CameraDir dir);
	glm::vec3 move_bitmap_to_move_v3(CameraData &camera);
	void update_pos(CameraData &camera, WorldData &world, float deltaTime);
	std::vector<std::pair<glm::vec3, BlockCoords>> get_all_collision_norms(CameraData &camera, WorldData &world);
	void place_block(CameraData &camera, WorldData &world);
	RayFace draw_ray_to_block(CameraData &camera, AABB aabb);
}
