#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#define GLM_ENABLE_EXPERIMENTAL // to be able to print matricies/vectors to stdout
#include <glm/gtx/string_cast.hpp>

#include <iostream>

#include <game_engine.hpp>
#include <window.hpp>
#include <shader.hpp>
#include <render.hpp>
#include <camera.hpp>
#include <world.hpp>
#include <collision.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

void framebuffer_size_cb(GLFWwindow *window, int width, int height);
void mouse_cb(GLFWwindow *window, double x, double y);
void processInput(GLFWwindow *window, CameraData &camera, WorldData &world);

//Camera camera;

float lastTime;
float deltaTime;

bool firstMouse = true;
double lastX;
double lastY;


int main()
{
	 EngineState state  = { };
	 BlockRender render = { };
	 CameraData  camera = { };
	 WorldData   world  = { };

	 Window::init(state);
	 Shader::init(render.shaderID);
	 Render::init(render);
	 World ::init(world, -20, 20, -5, 5, -20, 20);
	 Camera::init(camera);

	 glfwSetWindowUserPointer(state.window, &camera);

	 while (!glfwWindowShouldClose(state.window)) {
		  deltaTime = (float)glfwGetTime() - lastTime;
		  lastTime = (float)glfwGetTime();

		  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		  glClear(GL_COLOR_BUFFER_BIT);

		  World::draw(world, render);

		  Camera::reset_move(camera);
		  processInput(state.window, camera, world);
		  Camera::fall(camera, deltaTime);
		  Camera::move(camera, world, deltaTime);
		  Camera::update(camera);
		  Shader::setMat4(render.shaderID, "view", camera.view);

		  glfwSwapBuffers(state.window);
		  glfwPollEvents();
	 }

	 glfwTerminate();
	 return 0;
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
