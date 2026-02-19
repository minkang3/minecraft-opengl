const float hotbar_width  = 1.2f;
const float hotbar_height = 0.2f;

float hotbar_verticies[] = {
	-hotbar_width / 2,                -1.0f, 0.0f, 0.0f, 0.0f,
	 hotbar_width / 2,                -1.0f, 0.0f, 1.0f, 0.0f,
	 hotbar_width / 2, hotbar_height - 1.0f, 0.0f, 1.0f, 1.0f,
	 hotbar_width / 2, hotbar_height - 1.0f, 0.0f, 1.0f, 1.0f,
	-hotbar_width / 2, hotbar_height - 1.0f, 0.0f, 0.0f, 1.0f,
	-hotbar_width / 2,                -1.0f, 0.0f, 0.0f, 0.0f,
};

const float hotbar_selector_width = hotbar_width / 9;
const float hotbar_selector_height = hotbar_height;
const float hotbar_selector_offset = 0.01f;

float hotbar_selector_verticies[] = {
	-hotbar_selector_width / 2 - hotbar_selector_offset - hotbar_selector_width * 4,                        -1.0f - hotbar_selector_offset, -0.1f, 0.0f, 0.0f,
	 hotbar_selector_width / 2 + hotbar_selector_offset - hotbar_selector_width * 4,                        -1.0f - hotbar_selector_offset, -0.1f, 1.0f, 0.0f,
	 hotbar_selector_width / 2 + hotbar_selector_offset - hotbar_selector_width * 4, hotbar_selector_height -1.0f + hotbar_selector_offset, -0.1f, 1.0f, 1.0f,
	 hotbar_selector_width / 2 + hotbar_selector_offset - hotbar_selector_width * 4, hotbar_selector_height -1.0f + hotbar_selector_offset, -0.1f, 1.0f, 1.0f,
	-hotbar_selector_width / 2 - hotbar_selector_offset - hotbar_selector_width * 4, hotbar_selector_height -1.0f + hotbar_selector_offset, -0.1f, 0.0f, 1.0f,
	-hotbar_selector_width / 2 - hotbar_selector_offset - hotbar_selector_width * 4,                        -1.0f - hotbar_selector_offset, -0.1f, 0.0f, 0.0f,
};
