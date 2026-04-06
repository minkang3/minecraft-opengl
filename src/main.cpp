#ifdef __EMSCRIPTEN__
#include <GLES3/gl3.h>
#else
#include <glad/glad.h>
#endif

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
#include <inventory.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#ifdef __EMSCRIPTEN__
#include <emscripten/emscripten.h>
#endif

struct App
{
	EngineState state = {};
	WindowData window = {};
	Renderer render = {};
	CameraData camera = {};
	WorldData world = {};
	InventoryData inventory = {};
};

static void frame(void *userData)
{
	auto *app = static_cast<App*>(userData);
	EngineState &state = app->state;
	WindowData &window = app->window;
	Renderer &render = app->render;
	CameraData &camera = app->camera;
	WorldData &world = app->world;
	InventoryData &inventory = app->inventory;

	Engine::update_timedata(state);

	Shader::use(render.shaderID);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	Window::poll_events(window);
	Window::process_input(window, camera, world, inventory);
	Camera::update(camera, world, state.timestamp_delta);
	Engine::update_view_matrix(state);

	World::draw(world, render);

	Render::draw_hotbar(render);
	Inventory::draw_hotbar_items(inventory, render);
	Render::draw_hotbar_selector(render, inventory.hotbar_slot);
	Camera::draw_wire(camera, world, render);
	Render::draw_crosshair(render);

	Window::swap_buffers(window);

	if (Window::should_close(window)) {
#ifdef __EMSCRIPTEN__
		emscripten_cancel_main_loop();
#endif
	}
}

int main()
{
	static App app;

	app.state.window = &app.window;
	app.state.camera = &app.camera;
	app.state.render = &app.render;

	Window::init(app.window);

	// IMPORTANT for WebGL/Emscripten:
	// Do NOT call desktop GL loader (gladLoadGLLoader). WebGL provides symbols directly.

	Shader::init(app.render.shaderID, "shaders/shader.vs", "shaders/shader.fs");
	Shader::init(app.render.crosshairShaderID, "shaders/crosshair.vs", "shaders/crosshair.fs");
	Shader::init(app.render.hotbarShaderID, "shaders/hotbar.vs", "shaders/hotbar.fs");
	Shader::init(app.render.item3dShaderID, "shaders/item3d.vs", "shaders/item3d.fs");
	Render::init(app.render);
	World::init(app.world, -20, 20, -5, 10, -20, 20);
	Camera::init(app.camera);
	Inventory::init(app.inventory);


#ifdef __EMSCRIPTEN__
	Window::register_mousemove(app.window, app.camera);
	// Use browser animation frame.
	emscripten_set_main_loop_arg(frame, &app, 0, true);
	return 0;
#else
	// Non-web build stub loop: call frame until closed.
	while (!Window::should_close(app.window)) {
		frame(&app);
	}
	return 0;
#endif
}
