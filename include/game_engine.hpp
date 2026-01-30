#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <window.hpp>
#include <camera.hpp>

struct EngineState
{
	WindowData *window;
	CameraData *camera;
};
