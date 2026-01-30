#include <render.hpp>

namespace Render
{
	int init(BlockRender &render)
	{
		Shader::init(render.shaderID);

		unsigned int VAO, VBO;
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);

		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(cube_verticies), cube_verticies, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		render.VAO = VAO;
		render.VBO = VBO;

		init_textures(render);

		return 0;
	}

	void init_textures(BlockRender &render)
	{
		init_texture(render, BlockID::GRASS, "assets/grass.png");
		init_texture(render, BlockID::STONE, "assets/stone.png");
		init_texture(render, BlockID::DIRT, "assets/dirt.png");
	}

	void init_texture(BlockRender &render, BlockID block_id, std::string file_path)
	{
		unsigned int texture;
		// texture
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);

		// set the texture wrapping parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		// set texture filtering parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		int width, height, nrChannels;
		stbi_set_flip_vertically_on_load(true);
		unsigned char *data = stbi_load(file_path.c_str(), &width, &height, &nrChannels, 0);

		if (data) {
			std::cout << "loaded\n";
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
		} else {
			std::cout << "Failed to load image\n";
		}
		stbi_image_free(data);

		render.texture_map[block_id] = texture;

		Shader::setInt(render.shaderID, "texture1", texture);
	}

	bool set_texture(BlockRender &render, BlockID block_id)
	{
		if (render.texture_map.find(block_id) == render.texture_map.end())
			return false;

		glBindTexture(GL_TEXTURE_2D, render.texture_map[block_id]);
		return true;
	}

	void draw(BlockRender &render, int posX, int posY, int posZ)
	{
		glm::mat4 model = glm::mat4(1.0f);
		glm::vec3 posVec = glm::vec3((float)posX, (float)posY, (float)posZ);

		model = glm::translate(model, posVec);

		Shader::use(render.shaderID);
		Shader::setMat4(render.shaderID, "model", model);

		glBindVertexArray(render.VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}
}
