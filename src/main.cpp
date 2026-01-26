#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#define GLM_ENABLE_EXPERIMENTAL // to be able to print matricies/vectors to stdout
#include <glm/gtx/string_cast.hpp>

#include <shader.h>
#include <block_renderer.h>
#include <camera.hpp>
#include <world.hpp>
#include <collision.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

GLFWwindow *init();
void framebuffer_size_cb(GLFWwindow *window, int width, int height);
void mouse_cb(GLFWwindow *window, double x, double y);
void processInput(GLFWwindow *window);

Camera camera;
World world;

float lastTime;
float deltaTime;

double lastX = -1.0f;
double lastY = -1.0f;

int main()
{
	 GLFWwindow *window = init();
	 Shader myShader("shaders/shader.vs", "shaders/shader.fs");

	 BlockRenderer block_renderer(myShader);
	 block_renderer.init_texture("grass_block", "assets/grass_block_texture.jpg");
	 //block_renderer.set_texture("grass_block");

	 //Block my_block(3, 1, 3, "grass_block");

	 myShader.use();
	 
	 glm::mat4 projection = glm::mat4(1.0f);
	 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

	 myShader.setMat4("projection", projection);

	 while (!glfwWindowShouldClose(window)) {
		  deltaTime = (float)glfwGetTime() - lastTime;
		  lastTime = (float)glfwGetTime();

		  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		  glClear(GL_COLOR_BUFFER_BIT);

		  world.draw(block_renderer);

		  camera.reset_move();
		  processInput(window);
		  camera.fall(world, deltaTime);
		  camera.move(deltaTime, world);
		  camera.update();
		  myShader.setMat4("view", camera.get_view());

		  //std::cout << "feet y: " << camera.cameraPos.y + camera.playerHeightOffset - camera.playerHeight << std::endl;

		  glfwSwapBuffers(window);
		  glfwPollEvents();
	 }

	 glfwTerminate();
	 return 0;
}

GLFWwindow *init()
{
	  // init and set version
	 glfwInit();
	 glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	 glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	 glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	 glfwWindowHintString(GLFW_WAYLAND_APP_ID, "opengl_window");

	 // create window
	 GLFWwindow *window = glfwCreateWindow(800, 600, "opengl recreation", NULL, NULL);
	 if (!window) {
		  std::cout << "Failed to create window\n";
		  return NULL;
	 }
	 glfwMakeContextCurrent(window);
	 glfwSetFramebufferSizeCallback(window, framebuffer_size_cb);
	 glfwSetCursorPosCallback(window, mouse_cb);
	 glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	 if (glfwRawMouseMotionSupported()) {
		  std::cout << "setting raw mouse" << std::endl;
		  glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
	 }

	 // init glad
	 if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		  std::cout << "Failed to initialize glad\n";
		  return NULL;
	 }
	 glEnable(GL_DEPTH_TEST);

	 return window;
}

void processInput(GLFWwindow *window)
{
	 if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS ||
		 glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		  glfwSetWindowShouldClose(window, true);
	 
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
	 if (lastX < 0 && lastY < 0) {
		  lastX = x;
		  lastY = y;
		  return;
	 }

	 camera.move_cam(x - lastX, y - lastY);

	 lastX = x;
	 lastY = y;
}
