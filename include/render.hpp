#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stb_image.h>

#include <block.hpp>
#include "cube_verticies.hpp"

#include <string>


#define SHADER_ID_UNINIT 0xdeadbeef

typedef unsigned int ShaderID;
typedef unsigned int textureID;

struct Renderer
{
	ShaderID shaderID          = SHADER_ID_UNINIT;
	ShaderID crosshairShaderID = SHADER_ID_UNINIT;
	ShaderID hotbarShaderID    = SHADER_ID_UNINIT;
	unsigned int blockVAO;
	unsigned int wireVAO;
	unsigned int crosshairVAO;
	unsigned int hotbarVAO;
	unsigned int hotbarTextureID;
	std::unordered_map<BlockID, textureID> texture_map;
};

namespace Render
{
	int init(Renderer &render);
	int init_block_render(Renderer &render);
	int init_wire_render(Renderer &render);
	void init_crosshair(Renderer &render);
	void init_hotbar(Renderer &render);
	void init_textures(Renderer &render);
	void init_texture(Renderer &render, BlockID block_id, std::string file_path);
	void init_hotbar_texture(Renderer &render, std::string file_path);
	bool set_texture(Renderer &render, BlockID block_id);
	void draw_block(Renderer &render, int posX, int posY, int posZ);
	void draw_wire(Renderer &render, int posX, int posY, int posZ);
	void draw_crosshair(Renderer &render);
	void draw_hotbar(Renderer &render);
}
