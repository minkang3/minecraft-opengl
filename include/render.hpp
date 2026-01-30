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

struct BlockRender
{
	ShaderID shaderID = SHADER_ID_UNINIT;
	unsigned int VAO, VBO;
	std::unordered_map<BlockID, textureID> texture_map;
};

namespace Render
{
	int init(BlockRender &render);
	void init_textures(BlockRender &render);
	void init_texture(BlockRender &render, BlockID block_id, std::string file_path);
	bool set_texture(BlockRender &render, BlockID block_id);
	void draw(BlockRender &render, int posX, int posY, int posZ);
}
