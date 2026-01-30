#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <window.hpp>
#include <camera.hpp>

struct EngineState
{
	WindowData *window;
	CameraData *camera;
	BlockRender *render;

	float timestamp_prev;
	float timestamp_delta;
};

namespace Engine
{
	void update_timedata(EngineState &state);
	void update_view_matrix(EngineState &state);
}
