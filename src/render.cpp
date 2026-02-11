#include <render.hpp>
#include <cassert>

#include <shader.hpp>

namespace Render
{
	int init(BlockRender &render)
	{
		assert(render.shaderID != SHADER_ID_UNINIT && "Shader must be initialized before renderer");

		init_block_render(render);
		init_wire_render(render);
		init_textures(render);

		return 0;
	}

	int init_block_render(BlockRender &render)
	{
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

		render.blockVAO = VAO;

		return 0;
	}

	int init_wire_render(BlockRender &render)
	{
		unsigned int VAO, VBO;
		const float LINE_THICKNESS = 0.005f;

		float verticies[] = {
			// top front
			0.5f, 0.5f, 0.5f,
			-0.5f, 0.5f, 0.5f,
			-0.5f, 0.5f, 0.5f - LINE_THICKNESS,
			-0.5f, 0.5f, 0.5f - LINE_THICKNESS,
			0.5f, 0.5f, 0.5f - LINE_THICKNESS,
			0.5f, 0.5f, 0.5f,
		  
			// top left
			-0.5f, 0.5f, 0.5f,
			-0.5f, 0.5f, -0.5f,
			-0.5f + LINE_THICKNESS, 0.5f, -0.5f,
			-0.5f + LINE_THICKNESS, 0.5f, -0.5f,
			-0.5f + LINE_THICKNESS, 0.5f, 0.5f,
			-0.5f, 0.5f, 0.5f,

			// top back
			-0.5f, 0.5f, -0.5f,
			0.5f, 0.5f, -0.5f,
			0.5f, 0.5f, -0.5 + LINE_THICKNESS,
			0.5f, 0.5f, -0.5 + LINE_THICKNESS,
			-0.5f, 0.5f, -0.5 + LINE_THICKNESS,
			-0.5f, 0.5f, -0.5f,

			// top right
			0.5f, 0.5f, -0.5f,
			0.5f, 0.5f, 0.5f,
			0.5f - LINE_THICKNESS, 0.5f, 0.5f,
			0.5f - LINE_THICKNESS, 0.5f, 0.5f,
			0.5f - LINE_THICKNESS, 0.5f, -0.5f,
			0.5f, 0.5f, -0.5f,

			// front top
			0.5f, 0.5f, 0.5f,
			-0.5f, 0.5f, 0.5f,
			-0.5f, 0.5f - LINE_THICKNESS, 0.5f,
			-0.5f, 0.5f - LINE_THICKNESS, 0.5f,
			0.5f, 0.5f - LINE_THICKNESS, 0.5f,
			0.5f, 0.5f, 0.5f,

			// front right
			0.5f, 0.5f, 0.5f,
			0.5f, -0.5f, 0.5f,
			0.5f - LINE_THICKNESS, -0.5f, 0.5f,
			0.5f - LINE_THICKNESS, -0.5f, 0.5f,
			0.5f - LINE_THICKNESS, 0.5f, 0.5f,
			0.5f, 0.5f, 0.5f,

			// front bot
			0.5f, -0.5f, 0.5f,
			-0.5f, -0.5f, 0.5f,
			-0.5f, -0.5f + LINE_THICKNESS, 0.5f,
			-0.5f, -0.5f + LINE_THICKNESS, 0.5f,
			0.5f, -0.5f + LINE_THICKNESS, 0.5f,
			0.5f, -0.5f, 0.5f,

			// front left
			-0.5f, -0.5f, 0.5f,
			-0.5f, 0.5f, 0.5f,
			-0.5f + LINE_THICKNESS, 0.5f, 0.5f,
			-0.5f + LINE_THICKNESS, 0.5f, 0.5f,
			-0.5f + LINE_THICKNESS, -0.5f, 0.5f,
			-0.5f, -0.5f, 0.5f,

			// bot forward
			0.5f, -0.5f, 0.5f,
			-0.5f, -0.5f, 0.5f,
			-0.5f, -0.5f, 0.5f - LINE_THICKNESS,
			-0.5f, -0.5f, 0.5f - LINE_THICKNESS,
			0.5f, -0.5f, 0.5f - LINE_THICKNESS,
			0.5f, -0.5f, 0.5f,

			// bot left
			-0.5f, -0.5f, 0.5f,
			-0.5f, -0.5f, -0.5f,
			-0.5f + LINE_THICKNESS, -0.5f, -0.5f,
			-0.5f + LINE_THICKNESS, -0.5f, -0.5f,
			-0.5f + LINE_THICKNESS, -0.5f, 0.5f,
			-0.5f, -0.5f, 0.5f,

			// bot back
			-0.5f, -0.5f, -0.5f,
			0.5f, -0.5f, -0.5f,
			0.5f, -0.5f, -0.5f - LINE_THICKNESS,
			0.5f, -0.5f, -0.5f - LINE_THICKNESS,
			-0.5f, -0.5f, -0.5f - LINE_THICKNESS,
			-0.5f, -0.5f, -0.5f,

			// bot right
			0.5f, -0.5f, -0.5f,
			0.5f, -0.5f, 0.5f,
			0.5f - LINE_THICKNESS, -0.5f, 0.5f,
			0.5f - LINE_THICKNESS, -0.5f, 0.5f,
			0.5f - LINE_THICKNESS, -0.5f, -0.5f,
			0.5f, -0.5f, -0.5f,

			// back top
			-0.5f, 0.5f, -0.5f,
			0.5f, 0.5f, -0.5f,
			0.5f, 0.5f - LINE_THICKNESS, -0.5,
			0.5f, 0.5f - LINE_THICKNESS, -0.5,
			-0.5f, 0.5f - LINE_THICKNESS, -0.5,
			-0.5f, 0.5f, -0.5f,

			// back left
			-0.5f, 0.5f, -0.5f,
			-0.5f, -0.5f, -0.5f,
			-0.5f + LINE_THICKNESS, -0.5f, -0.5f,
			-0.5f + LINE_THICKNESS, -0.5f, -0.5f,
			-0.5f + LINE_THICKNESS, 0.5f, -0.5f,
			-0.5f, 0.5f, -0.5f,

			// back bot
			-0.5f, -0.5f, -0.5f,
			0.5f, -0.5f, -0.5f,
			0.5f, -0.5f + LINE_THICKNESS, -0.5f,
			0.5f, -0.5f + LINE_THICKNESS, -0.5f,
			-0.5f, -0.5f + LINE_THICKNESS, -0.5f,
			-0.5f, -0.5f, -0.5f,
		  
			// back right
			0.5f, -0.5f, -0.5f,
			0.5f, 0.5f, -0.5f,
			0.5f - LINE_THICKNESS, 0.5f, -0.5f,
			0.5f - LINE_THICKNESS, 0.5f, -0.5f,
			0.5f - LINE_THICKNESS, -0.5f, -0.5f,
			0.5f, -0.5f, -0.5f,

			// left top
			-0.5f, 0.5f, 0.5f,
			-0.5f, 0.5f, -0.5f,
			-0.5f, 0.5f - LINE_THICKNESS, -0.5f,
			-0.5f, 0.5f - LINE_THICKNESS, -0.5f,
			-0.5f, 0.5f - LINE_THICKNESS, 0.5f,
			-0.5f, 0.5f, 0.5f,

			// left bot
			-0.5f, -0.5f, 0.5f,
			-0.5f, -0.5f, -0.5f,
			-0.5f, -0.5f + LINE_THICKNESS, -0.5f,
			-0.5f, -0.5f + LINE_THICKNESS, -0.5f,
			-0.5f, -0.5f + LINE_THICKNESS, 0.5f,
			-0.5f, -0.5f, 0.5f,

			// left front
			-0.5f, 0.5f, 0.5f,
			-0.5f, -0.5f, 0.5f,
			-0.5f, -0.5f, 0.5f - LINE_THICKNESS,
			-0.5f, -0.5f, 0.5f - LINE_THICKNESS,
			-0.5f, 0.5f, 0.5f - LINE_THICKNESS,
			-0.5f, 0.5f, 0.5f,

			// left back
			-0.5f, 0.5f,  -0.5f,
			-0.5f, -0.5f, -0.5f,
			-0.5f, -0.5f, -0.5f + LINE_THICKNESS,
			-0.5f, -0.5f, -0.5f + LINE_THICKNESS,
			-0.5f, 0.5f,  -0.5f + LINE_THICKNESS,
			-0.5f, 0.5f,  -0.5f,

			// right top
			0.5f, 0.5f, 0.5f,
			0.5f, 0.5f, -0.5f,
			0.5f, 0.5f - LINE_THICKNESS, -0.5f,
			0.5f, 0.5f - LINE_THICKNESS, -0.5f,
			0.5f, 0.5f - LINE_THICKNESS, 0.5f,
			0.5f, 0.5f, 0.5f,

			// right bot
			0.5f, -0.5f, 0.5f,
			0.5f, -0.5f, -0.5f,
			0.5f, -0.5f + LINE_THICKNESS, -0.5f,
			0.5f, -0.5f + LINE_THICKNESS, -0.5f,
			0.5f, -0.5f + LINE_THICKNESS, 0.5f,
			0.5f, -0.5f, 0.5f,

			// right front
			0.5f, 0.5f, 0.5f,
			0.5f, -0.5f, 0.5f,
			0.5f, -0.5f, 0.5f - LINE_THICKNESS,
			0.5f, -0.5f, 0.5f - LINE_THICKNESS,
			0.5f, 0.5f, 0.5f - LINE_THICKNESS,
			0.5f, 0.5f, 0.5f,

			// right back
			0.5f, 0.5f,  -0.5f,
			0.5f, -0.5f, -0.5f,
			0.5f, -0.5f, -0.5f + LINE_THICKNESS,
			0.5f, -0.5f, -0.5f + LINE_THICKNESS,
			0.5f, 0.5f,  -0.5f + LINE_THICKNESS,
			0.5f, 0.5f,  -0.5f,


		};

		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);

		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(verticies), verticies, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		render.wireVAO = VAO;
		return 0;
	}

	void init_textures(BlockRender &render)
	{
		init_texture(render, BlockID::GRASS, "assets/grass.png");
		init_texture(render, BlockID::STONE, "assets/stone.png");
		init_texture(render, BlockID::DIRT, "assets/dirt.png");

		Shader::setInt(render.shaderID, "texture1", 0);
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
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
		} else {
			std::cout << "Failed to load image\n";
		}
		stbi_image_free(data);

		render.texture_map[block_id] = texture;
	}

	bool set_texture(BlockRender &render, BlockID block_id)
	{
		if (render.texture_map.find(block_id) == render.texture_map.end())
			return false;

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, render.texture_map[block_id]);
		return true;
	}

	void draw_block(BlockRender &render, int posX, int posY, int posZ)
	{
		glm::mat4 model = glm::mat4(1.0f);
		glm::vec3 posVec = glm::vec3((float)posX, (float)posY, (float)posZ);

		model = glm::translate(model, posVec);

		Shader::use(render.shaderID);
		Shader::setMat4(render.shaderID, "model", model);

		glBindVertexArray(render.blockVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}

	void draw_wire(BlockRender &render, int posX, int posY, int posZ)
	{
		const float SCALE_AMT = 0.01f;

		glm::mat4 model = glm::mat4(1.0f);
		glm::vec3 posVec = glm::vec3((float)posX,
									 (float)posY,
									 (float)posZ);

		model = glm::translate(model, posVec);
		model = glm::scale(model, glm::vec3(1.0f + SCALE_AMT));

		Shader::use(render.shaderID);
		Shader::setMat4(render.shaderID, "model", model);

		glBindVertexArray(render.wireVAO);
		glLineWidth(8.0f);
		glDrawArrays(GL_TRIANGLES, 0, 6*24);
	}
}
