#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <camera.hpp>
#include <world.hpp>
#include <inventory.hpp>

#define WINDOW_WIDTH  (800)
#define WINDOW_HEIGHT (600)

struct WindowData
{
	GLFWwindow *handle;
	bool first_mouse_flag = true;
	double mouse_prev_xpos;
	double mouse_prev_ypos;
};

namespace Window
{
	int init(WindowData &window);
	void framebuffer_size_cb(GLFWwindow *window, int width, int height);
	void mouse_cb(GLFWwindow *window, double x, double y);
	void process_input(GLFWwindow *window, CameraData &camera, WorldData &world, InventoryData &inventory);
}
