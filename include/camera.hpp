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

class Camera
{
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

	bool shouldJump;
	World *world;

public:
	float yaw;
	float pitch;
	float speed;
	Camera()
		: world(world)
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

	void set_world(World *world)
	{
		this->world = world;
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
				if (shouldJump) {
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
		shouldJump = false;
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
		shouldJump = true;
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

	void place_block()
	{
		std::vector<std::pair<RayFace, BlockCoords>> rayfaces;
		// need to draw a ray
		// need to find which block it hits and what face
		for (int z = world->zmin; z < world->zmin + world->zsize; ++z) {
			for (int y = world->ymin; y < world->ymin + world->ysize; ++y) {
				for (int x = world->xmin; x < world->xmin + world->xsize; ++x) {
					if ((*world)(x, y, z) == BlockID::NONE)
						continue;
					AABB aabb = make_block_aabb(x, y, z);
					RayFace rayface = draw_ray_to_block(aabb);
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

		(*world)(new_coords.x, new_coords.y, new_coords.z) = BlockID::STONE;

		std::cout << "new coords:" << std::endl;
		std::cout << "(" << new_coords.x << ", " << new_coords.y << ", " << new_coords.z << ")" << std::endl;
	}

	RayFace draw_ray_to_block(AABB aabb)
	{
		float x_low  = aabb.min.x;
		float x_high = aabb.max.x;
		float y_low  = aabb.min.y;
		float y_high = aabb.max.y;
		float z_low  = aabb.min.z;
		float z_high = aabb.max.z;

		float o_x = cameraPos.x;
		float o_y = cameraPos.y;
		float o_z = cameraPos.z;

		float r_x = cameraDir.x;
		float r_y = cameraDir.y;
		float r_z = cameraDir.z;

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
};
