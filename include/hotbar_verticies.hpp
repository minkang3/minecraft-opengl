const float hotbar_width  = 0.6f;
const float hotbar_height = 0.2f;

float hotbar_verticies[] = {
	-hotbar_width,                 -1.0f, 0.0f, 0.0f, 0.0f,
	 hotbar_width,                 -1.0f, 0.0f, 1.0f, 0.0f,
	 hotbar_width, -1.0f + hotbar_height, 0.0f, 1.0f, 1.0f,
	 hotbar_width, -1.0f + hotbar_height, 0.0f, 1.0f, 1.0f,
	-hotbar_width, -1.0f + hotbar_height, 0.0f, 0.0f, 1.0f,
	-hotbar_width,                 -1.0f, 0.0f, 0.0f, 0.0f,
};
