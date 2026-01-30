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
void processInput(GLFWwindow *window);

Camera camera;

float lastTime;
float deltaTime;

bool firstMouse = true;
double lastX;
double lastY;

int main()
{
	 EngineState state  = { 0 };
	 BlockRender render = { 0 };

	 Window::init(state);
	 Render::init(render);

	 World world(-20, 20, -5, 5, -20, 20);

	 camera.set_world(&world);

	 world.fill(BlockID::GRASS, -5, 5, 0, 0, -5, 5);

	 world.place(BlockID::STONE, 1, 1, 1);
	 world.place(BlockID::DIRT, 2, 1, 1);
	 world.place(BlockID::DIRT, 0, 5, 0);

	 world.place(BlockID::STONE, -5, 1, 0);

	 Shader::use(render.shaderID);
	 
	 glm::mat4 projection = glm::mat4(1.0f);
	 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

	 Shader::setMat4(render.shaderID, "projection", projection);

	 while (!glfwWindowShouldClose(state.window)) {
		  deltaTime = (float)glfwGetTime() - lastTime;
		  lastTime = (float)glfwGetTime();

		  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		  glClear(GL_COLOR_BUFFER_BIT);

		  world.draw(render);

		  camera.reset_move();
		  processInput(state.window);
		  camera.fall(world, deltaTime);
		  camera.move(deltaTime, world);
		  camera.update();
		  Shader::setMat4(render.shaderID, "view", camera.get_view());

		  glfwSwapBuffers(state.window);
		  glfwPollEvents();
	 }

	 glfwTerminate();
	 return 0;
}


void processInput(GLFWwindow *window)
{
	 static bool rightClickFirst = true;
	 if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS ||
		 glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		  glfwSetWindowShouldClose(window, true);

	 if (rightClickFirst && glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
		  std::cout << "right click detected" << std::endl;
		  camera.place_block();
		  rightClickFirst = false;
	 } else if (!rightClickFirst && glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE) {
		  rightClickFirst = true;
	 }
	 
	 if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		  camera.move_horz(FORWARD, deltaTime);
	 if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	  	  camera.move_horz(BACKWARD, deltaTime);
	 if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	  	  camera.move_horz(LEFT, deltaTime);
	 if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	  	  camera.move_horz(RIGHT, deltaTime);
	 if (!(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) &&
		 !(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) &&
		 !(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) &&
		 !(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS))
		  camera.move_horz(NONE, deltaTime);
		 
	 if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		  camera.jump();
	 // if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
	 // 	  camera.move_vert(-1, deltaTime);
	 
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
	 if (firstMouse) {
		  lastX = x;
		  lastY = y;
		  firstMouse = false;
		  return;
	 }

	 camera.move_cam(x - lastX, y - lastY);

	 lastX = x;
	 lastY = y;
}
