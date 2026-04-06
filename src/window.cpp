#include <window.hpp>

#include <camera.hpp>
#include <inventory.hpp>
#include <world.hpp>

#include <iostream>

#ifdef __EMSCRIPTEN__
#include <emscripten/emscripten.h>
#include <emscripten/html5.h>
#endif

namespace Window
{
	#ifdef __EMSCRIPTEN__
	static EM_BOOL key_cb(int eventType, const EmscriptenKeyboardEvent *e, void *userData)
	{
		(void)eventType;
		auto *window = static_cast<WindowData*>(userData);
		if (!window) return EM_FALSE;

		const bool down = (eventType == EMSCRIPTEN_EVENT_KEYDOWN);

		// Map common keys by "code"
		std::string code = e->code ? e->code : "";
		auto setKey = [&](int idx, bool v) {
			if (idx >= 0 && idx < (int)(sizeof(window->keys)/sizeof(window->keys[0]))) window->keys[idx] = v;
		};

		if (code == "KeyW") setKey('W', down);
		else if (code == "KeyA") setKey('A', down);
		else if (code == "KeyS") setKey('S', down);
		else if (code == "KeyD") setKey('D', down);
		else if (code == "Space") setKey(' ', down);
		else if (code == "Escape" || code == "KeyQ") window->should_close = down;
		else if (code.rfind("Digit", 0) == 0 && code.size() == 6) {
			char d = code[5];
			if (d >= '1' && d <= '9') setKey(d, down);
		}

		return EM_TRUE;
	}

	static EM_BOOL mousedown_cb(int eventType, const EmscriptenMouseEvent *e, void *userData)
	{
		auto *window = static_cast<WindowData*>(userData);
		if (!window) return EM_FALSE;
		bool down = (eventType == EMSCRIPTEN_EVENT_MOUSEDOWN);
		if (e->button >= 0 && e->button < 8) window->mouseButtons[e->button] = down;

		// Request pointer lock on first click so we can read relative mouse movement.
		if (down && !window->pointer_locked) {
			emscripten_request_pointerlock("#canvas", true);
		}

		return EM_TRUE;
	}

	struct MouseMoveCtx { WindowData *window; CameraData *camera; };
	static EM_BOOL mousemove_cb(int eventType, const EmscriptenMouseEvent *e, void *userData)
	{
		(void)eventType;
		auto *ctx = static_cast<MouseMoveCtx*>(userData);
		if (!ctx || !ctx->window || !ctx->camera) return EM_FALSE;

		// When pointer-locked, use movementX/Y (relative) instead of absolute canvas coords.
		if (ctx->window->pointer_locked) {
			Camera::move_cam(*ctx->camera, (double)e->movementX, (double)e->movementY);
			return EM_TRUE;
		}

		on_mouse_move(*ctx->window, e->canvasX, e->canvasY, *ctx->camera);
		return EM_TRUE;
	}

	static EM_BOOL pointerlockchange_cb(int eventType, const EmscriptenPointerlockChangeEvent *e, void *userData)
	{
		(void)eventType;
		auto *window = static_cast<WindowData*>(userData);
		if (!window) return EM_FALSE;
		window->pointer_locked = e->isActive;
		window->first_mouse_flag = true; // reset to avoid jumps when unlocking/relocking
		return EM_TRUE;
	}

	static EM_BOOL resize_cb(int eventType, const EmscriptenUiEvent *e, void *userData)
	{
		(void)eventType;
		(void)e;
		auto *window = static_cast<WindowData*>(userData);
		if (!window) return EM_FALSE;

		int w, h;
		emscripten_get_canvas_element_size("#canvas", &w, &h);
		on_resize(*window, w, h);
		return EM_TRUE;
	}
	#endif

	int init(WindowData &window)
	{
		// On web, we must ensure a WebGL context exists *before* calling any GL function.
#ifdef __EMSCRIPTEN__
		EmscriptenWebGLContextAttributes attr;
		emscripten_webgl_init_context_attributes(&attr);
		attr.alpha = 1;
		attr.depth = 1;
		attr.stencil = 0;
		attr.antialias = 0;
		attr.premultipliedAlpha = 0;
		attr.preserveDrawingBuffer = 0;
		attr.enableExtensionsByDefault = 1;
		attr.majorVersion = 2;
		attr.minorVersion = 0;

		EMSCRIPTEN_WEBGL_CONTEXT_HANDLE ctx = emscripten_webgl_create_context("#canvas", &attr);
		if (ctx <= 0) {
			std::cerr << "Failed to create WebGL2 context\n";
			return -1;
		}
		emscripten_webgl_make_context_current(ctx);

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// make canvas match our constants initially
		emscripten_set_canvas_element_size("#canvas", WINDOW_WIDTH, WINDOW_HEIGHT);
		int w, h;
		emscripten_get_canvas_element_size("#canvas", &w, &h);
		on_resize(window, w, h);

		emscripten_set_keydown_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, &window, true, key_cb);
		emscripten_set_keyup_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, &window, true, key_cb);
		emscripten_set_mousedown_callback("#canvas", &window, true, mousedown_cb);
		emscripten_set_mouseup_callback("#canvas", &window, true, mousedown_cb);
		emscripten_set_pointerlockchange_callback(EMSCRIPTEN_EVENT_TARGET_DOCUMENT, &window, true, pointerlockchange_cb);
		// mouse move callback will be registered from main once we have camera/state
		emscripten_set_resize_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, &window, true, resize_cb);
#else
		std::cerr << "This project was refactored for emscripten; native window creation is not implemented.\n";
#endif
		return 0;
	}

	void poll_events(WindowData &window)
	{
		(void)window;
		// Web events are delivered via callbacks.
	}

	void swap_buffers(WindowData &window)
	{
		(void)window;
		// Emscripten swaps automatically when the frame ends.
	}

	bool should_close(const WindowData &window)
	{
		return window.should_close;
	}

	void request_close(WindowData &window)
	{
		window.should_close = true;
	}

	void on_resize(WindowData &window, int width, int height)
	{
		window.width = width;
		window.height = height;
		glViewport(0, 0, width, height);
	}

	void on_mouse_move(WindowData &window, double x, double y, CameraData &camera)
	{
		if (window.first_mouse_flag) {
			window.mouse_prev_xpos = x;
			window.mouse_prev_ypos = y;
			window.first_mouse_flag = false;
			return;
		}

		Camera::move_cam(camera, x - window.mouse_prev_xpos, y - window.mouse_prev_ypos);
		window.mouse_prev_xpos = x;
		window.mouse_prev_ypos = y;
	}

	void process_input(WindowData &window, CameraData &camera, WorldData &world, InventoryData &inventory)
	{
		static bool rightClickFirst = true;
		static bool leftClickFirst  = true;

		// Mouse buttons: JS button mapping: 0 left, 2 right
		bool rightDown = window.mouseButtons[2];
		bool leftDown  = window.mouseButtons[0];

		if (rightClickFirst && rightDown) {
			Inventory::place_block(inventory, camera, world);
			rightClickFirst = false;
		} else if (!rightClickFirst && !rightDown) {
			rightClickFirst = true;
		}

		if (leftClickFirst && leftDown) {
			Camera::break_block(camera, world);
			leftClickFirst = false;
		} else if (!leftClickFirst && !leftDown) {
			leftClickFirst = true;
		}

		if (window.keys['W']) Camera::queue_horz_move(camera, FORWARD);
		if (window.keys['S']) Camera::queue_horz_move(camera, BACKWARD);
		if (window.keys['A']) Camera::queue_horz_move(camera, LEFT);
		if (window.keys['D']) Camera::queue_horz_move(camera, RIGHT);

		if (window.keys[' ']) camera.shouldJump = true;

		for (char d = '1'; d <= '9'; d++) {
			if (window.keys[(int)d]) Inventory::hotbar_select_slot(inventory, (unsigned int)(d - '0'));
		}

		if (window.should_close) request_close(window);
	}

#ifdef __EMSCRIPTEN__
	void register_mousemove(WindowData &window, CameraData &camera)
	{
		static MouseMoveCtx ctx;
		ctx.window = &window;
		ctx.camera = &camera;
		emscripten_set_mousemove_callback("#canvas", &ctx, true, mousemove_cb);
	}
#endif
}
