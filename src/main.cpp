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


int main()
{
	 EngineState state  = { };
	 WindowData  window = { };
	 Renderer    render = { };
	 CameraData  camera = { };
	 WorldData   world  = { };

	 state.window = &window;
	 state.camera = &camera;
	 state.render = &render;

	 Window::init(window);
	 Shader::init(render.shaderID, "shaders/shader.vs", "shaders/shader.fs");
	 Shader::init(render.crosshairShaderID, "shaders/crosshair.vs", "shaders/crosshair.fs");
	 Shader::init(render.hotbarShaderID, "shaders/hotbar.vs", "shaders/hotbar.fs");
	 Render::init(render);
	 World ::init(world, -20, 20, -5, 10, -20, 20);
	 Camera::init(camera);

	 glfwSetWindowUserPointer(window.handle, &state);

	 glEnable(GL_BLEND);
	 glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	 while (!glfwWindowShouldClose(window.handle)) {
		  Engine::update_timedata(state);

		  Shader::use(render.shaderID);

		  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		  glClear(GL_COLOR_BUFFER_BIT);

		  Window::process_input(window.handle, camera, world);
		  Camera::update(camera, world, state.timestamp_delta);
		  Engine::update_view_matrix(state);

		  World::draw(world, render);

		  Render::draw_hotbar(render);
		  Camera::draw_wire(camera, world, render);
		  Render::draw_crosshair(render);

		  glfwSwapBuffers(window.handle);
		  glfwPollEvents();
	 }

	 glfwTerminate();
	 return 0;
}


