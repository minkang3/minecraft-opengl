#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stb_image.h>

#include <game_engine.hpp>

#include <block.hpp>
#include <shader.hpp>
#include "cube_verticies.hpp"

typedef unsigned int textureID;

struct BlockRender
{
	ShaderID shaderID;
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
