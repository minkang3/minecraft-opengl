#pragma once

#ifdef __EMSCRIPTEN__
#include <GLES3/gl3.h>
#else
#include <glad/glad.h>
#endif

#include <window.hpp>
#include <camera.hpp>

struct EngineState
{
	WindowData *window;
	CameraData *camera;
	Renderer *render;

	float timestamp_prev = 0.0f;
	float timestamp_delta = 0.0f;
};

namespace Engine
{
	void update_timedata(EngineState &state);
	void update_view_matrix(EngineState &state);
}
