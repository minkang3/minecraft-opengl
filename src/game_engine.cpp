#include <game_engine.hpp>

#include <iostream>
#include <chrono>

#ifdef __EMSCRIPTEN__
#include <emscripten/emscripten.h>
#endif

namespace Engine
{
	static float now_seconds()
	{
#ifdef __EMSCRIPTEN__
		return (float)(emscripten_get_now() / 1000.0);
#else
		// Fallback for non-web builds without glfw.
		using clock = std::chrono::steady_clock;
		static const auto start = clock::now();
		auto t = clock::now() - start;
		return std::chrono::duration_cast<std::chrono::duration<float>>(t).count();
#endif
	}

	void update_timedata(EngineState &state)
	{
		float now = now_seconds();
		state.timestamp_delta = now - state.timestamp_prev;
		state.timestamp_prev = now;
	}

	void update_view_matrix(EngineState &state)
	{
		CameraData *camera = state.camera;

		glm::mat4 view = glm::lookAt(camera->pos,
									 camera->pos + camera->dir,
									 camera->up);

		Shader::setMat4(state.render->shaderID, "view", view);
	}
}
