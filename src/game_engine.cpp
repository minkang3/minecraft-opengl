#include <game_engine.hpp>

#include <iostream>

namespace Engine
{
	void update_timedata(EngineState &state)
	{
		state.timestamp_delta = (float)glfwGetTime() - state.timestamp_prev;
		state.timestamp_prev  = (float)glfwGetTime();
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
