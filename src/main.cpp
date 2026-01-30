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


float lastTime;
float deltaTime;

bool firstMouse = true;
double lastX;
double lastY;


int main()
{
	 EngineState state  = { };
	 WindowData  window = { };
	 BlockRender render = { };
	 CameraData  camera = { };
	 WorldData   world  = { };

	 Window::init(window);
	 Shader::init(render.shaderID);
	 Render::init(render);
	 World ::init(world, -20, 20, -5, 5, -20, 20);
	 Camera::init(camera);

	 glfwSetWindowUserPointer(window.handle, &camera);

	 while (!glfwWindowShouldClose(window.handle)) {
		  deltaTime = (float)glfwGetTime() - lastTime;
		  lastTime = (float)glfwGetTime();

		  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		  glClear(GL_COLOR_BUFFER_BIT);

		  World::draw(world, render);

		  Camera::reset_move(camera);
		  Window::processInput(window.handle, camera, world);
		  Camera::fall(camera, deltaTime);
		  Camera::move(camera, world, deltaTime);
		  Camera::update(camera);
		  Shader::setMat4(render.shaderID, "view", camera.view);

		  glfwSwapBuffers(window.handle);
		  glfwPollEvents();
	 }

	 glfwTerminate();
	 return 0;
}


