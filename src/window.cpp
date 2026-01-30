#include <window.hpp>

#include <iostream>

void framebuffer_size_cb(GLFWwindow *window, int width, int height);
void mouse_cb(GLFWwindow *window, double x, double y);

namespace Window
{
	int init(EngineState &state)
	{
		// init and set version
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHintString(GLFW_WAYLAND_APP_ID, "opengl_window");

		// create window
		state.window = glfwCreateWindow(800, 600, "opengl recreation", NULL, NULL);
		if (!state.window) {
			std::cout << "Failed to create window\n";
			return -1;
		}
		glfwMakeContextCurrent(state.window);
		glfwSetFramebufferSizeCallback(state.window, framebuffer_size_cb);
		glfwSetCursorPosCallback(state.window, mouse_cb);
		glfwSetInputMode(state.window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		if (glfwRawMouseMotionSupported()) {
			std::cout << "setting raw mouse" << std::endl;
			glfwSetInputMode(state.window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
		}

		// init glad
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
			std::cout << "Failed to initialize glad\n";
			return -1;
		}
		glEnable(GL_DEPTH_TEST);

		return 0;
	}
}
