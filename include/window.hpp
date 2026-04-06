#pragma once

#ifdef __EMSCRIPTEN__
#include <GLES3/gl3.h>
#else
#include <glad/glad.h>
#endif

#include <camera.hpp>
#include <world.hpp>
#include <inventory.hpp>

#define WINDOW_WIDTH  (800)
#define WINDOW_HEIGHT (600)

struct WindowData
{
	// For native builds you might have a platform window handle.
	// For emscripten/web builds we render into a canvas, so no handle is needed.
	void *handle = nullptr;

	bool first_mouse_flag = true;
	double mouse_prev_xpos = 0.0;
	double mouse_prev_ypos = 0.0;

	// Input state (filled by native or web event handlers)
	bool keys[512] = {false};
	bool mouseButtons[8] = {false};

	bool pointer_locked = false;

	int width = WINDOW_WIDTH;
	int height = WINDOW_HEIGHT;

	bool should_close = false;
};

namespace Window
{
	int init(WindowData &window);
	void poll_events(WindowData &window);
	void swap_buffers(WindowData &window);
	bool should_close(const WindowData &window);
	void request_close(WindowData &window);

	// Called by the platform event system
	void on_resize(WindowData &window, int width, int height);
	void on_mouse_move(WindowData &window, double x, double y, CameraData &camera);

	// Uses stored input state to drive gameplay actions
	void process_input(WindowData &window, CameraData &camera, WorldData &world, InventoryData &inventory);

#ifdef __EMSCRIPTEN__
	// Emscripten-specific helper to bind mousemove to a camera.
	void register_mousemove(WindowData &window, CameraData &camera);
#endif
}
