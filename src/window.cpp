#include <window.hpp>

#include <iostream>

extern float lastTime;
extern float deltaTime;

extern bool firstMouse;
extern double lastX;
extern double lastY;

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
		window.handle = glfwCreateWindow(800, 600, "opengl recreation", NULL, NULL);
		if (!window.handle) {
			std::cout << "Failed to create window\n";
			return -1;
		}
		glfwMakeContextCurrent(window.handle);
		glfwSetFramebufferSizeCallback(window.handle, framebuffer_size_cb);
		glfwSetCursorPosCallback(window.handle, mouse_cb);
		glfwSetInputMode(window.handle, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		if (glfwRawMouseMotionSupported()) {
			std::cout << "setting raw mouse" << std::endl;
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
		//std::cout << "in mouse_cb: (" << x << ", " << y << ")" << std::endl;
		CameraData *camera = static_cast<CameraData*>(glfwGetWindowUserPointer(window));
		if (firstMouse) {
			lastX = x;
			lastY = y;
			firstMouse = false;
			return;
		}

		Camera::move_cam(*camera, x - lastX, y - lastY);

		lastX = x;
		lastY = y;
	}

	void processInput(GLFWwindow *window, CameraData &camera, WorldData &world)
	{
		static bool rightClickFirst = true;
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS ||
			glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);

		if (rightClickFirst && glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
			std::cout << "right click detected" << std::endl;
			Camera::place_block(camera, world);
			rightClickFirst = false;
		} else if (!rightClickFirst && glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE) {
			rightClickFirst = true;
		}
	 
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			Camera::move_horz(camera, FORWARD, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			Camera::move_horz(camera, BACKWARD, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			Camera::move_horz(camera, LEFT, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			Camera::move_horz(camera, RIGHT, deltaTime);
		if (!(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) &&
			!(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) &&
			!(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) &&
			!(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS))
			Camera::move_horz(camera, NONE, deltaTime);
		 
		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
			camera.shouldJump = true;
		// if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		// 	  Camera::move_vert(camera, -1, deltaTime);
	 
		if (glfwGetKey(window, GLFW_KEY_EQUAL) == GLFW_PRESS) {
			camera.speed += 0.1f;
			std::cout << "add pressed\n" << std::endl;
		}
		if (glfwGetKey(window, GLFW_KEY_MINUS) == GLFW_PRESS) {
			camera.speed -= 0.1f;
			std::cout << "minus pressed\n" << std::endl;
		}
	}
}
