#include <game_engine.hpp>

#include <iostream>

namespace Engine
{
	void update_timedata(EngineState &state)
	{
		state.timestamp_delta = (float)glfwGetTime() - state.timestamp_prev;
		state.timestamp_prev  = (float)glfwGetTime();
	}
}
