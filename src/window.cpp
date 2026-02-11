#include <window.hpp>
#include <game_engine.hpp>
#include <iostream>


namespace Window
{
	int init(WindowData &window)
	{
		// init and set version
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHintString(GLFW_WAYLAND_APP_ID, "opengl_window");

		// create window
		window.handle = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "opengl recreation", NULL, NULL);
		if (!window.handle) {
			std::cout << "Failed to create window\n";
			return -1;
		}
		glfwMakeContextCurrent(window.handle);
		glfwSetFramebufferSizeCallback(window.handle, framebuffer_size_cb);
		glfwSetCursorPosCallback(window.handle, mouse_cb);
		glfwSetInputMode(window.handle, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		if (glfwRawMouseMotionSupported()) {
			glfwSetInputMode(window.handle, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
		}

		// init glad
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
			std::cout << "Failed to initialize glad\n";
			return -1;
		}
		glEnable(GL_DEPTH_TEST);

		return 0;
	}

	void framebuffer_size_cb(GLFWwindow *window, int width, int height)
	{
		glViewport(0, 0, width, height);
	}

	void mouse_cb(GLFWwindow *window, double x, double y)
	{
		EngineState *state = static_cast<EngineState*>(glfwGetWindowUserPointer(window));

		if (state->window->first_mouse_flag) {
			state->window->mouse_prev_xpos = x;
			state->window->mouse_prev_ypos = y;
			state->window->first_mouse_flag = false;
			return;
		}

		Camera::move_cam(*(state->camera), x - state->window->mouse_prev_xpos, y - state->window->mouse_prev_ypos);

		state->window->mouse_prev_xpos = x;
		state->window->mouse_prev_ypos = y;
	}

	void process_input(GLFWwindow *window, CameraData &camera, WorldData &world)
	{
		static bool rightClickFirst = true;
		static bool leftClickFirst  = true;

		if (rightClickFirst && glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
			Camera::place_block(camera, world);
			rightClickFirst = false;
		} else if (!rightClickFirst && glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE) {
			rightClickFirst = true;
		}

		if (leftClickFirst && glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
			Camera::break_block(camera, world);
			leftClickFirst = false;
		} else if (!leftClickFirst && glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE) {
			leftClickFirst = true;
		}
	 
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			Camera::queue_horz_move(camera, FORWARD);
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			Camera::queue_horz_move(camera, BACKWARD);
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			Camera::queue_horz_move(camera, LEFT);
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			Camera::queue_horz_move(camera, RIGHT);
		 
		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
			camera.shouldJump = true;
		// if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		// 	  Camera::move_vert(camera, -1, deltaTime);

		if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
			camera.selected_block = static_cast<BlockID>(1);
		if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
			camera.selected_block = static_cast<BlockID>(2);
		if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
			camera.selected_block = static_cast<BlockID>(3);
		if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
			camera.selected_block = static_cast<BlockID>(4);
		if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS)
			camera.selected_block = static_cast<BlockID>(5);
		if (glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS)
			camera.selected_block = static_cast<BlockID>(6);
		if (glfwGetKey(window, GLFW_KEY_7) == GLFW_PRESS)
			camera.selected_block = static_cast<BlockID>(7);
		if (glfwGetKey(window, GLFW_KEY_8) == GLFW_PRESS)
			camera.selected_block = static_cast<BlockID>(8);
		if (glfwGetKey(window, GLFW_KEY_9) == GLFW_PRESS)
			camera.selected_block = static_cast<BlockID>(9);

		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS ||
			glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);
	}
}
