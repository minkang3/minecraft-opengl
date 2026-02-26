#include <render.hpp>
#include <cassert>

#include <wire_verticies.hpp>
#include <crosshair_verticies.hpp>
#include <hotbar_verticies.hpp>

#include <shader.hpp>


namespace Render
{
    // * Init
	int init(Renderer &render)
	{
		assert(render.shaderID != SHADER_ID_UNINIT && "Shader must be initialized before renderer");

		init_block_render(render);
		init_wire_render(render);
		init_crosshair(render);
		init_hotbar(render);
		init_hotbar_selector(render);
		init_3d_hotbar_item(render);

		init_textures(render);
		init_hotbar_texture(render, "assets/hotbar.png");
		init_hotbar_selector_texture(render, "assets/hotbar_selector.png");

		return 0;
	}

    // * Block
	int init_block_render(Renderer &render)
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


	void init_texture(Renderer &render, BlockID block_id, std::string file_path)
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

	void init_textures(Renderer &render)
	{
		init_texture(render, BlockID::GRASS, "assets/grass.png");
		init_texture(render, BlockID::STONE, "assets/stone.png");
		init_texture(render, BlockID::DIRT, "assets/dirt.png");

		Shader::setInt(render.shaderID, "texture1", 0);
	}

	bool set_texture(Renderer &render, BlockID block_id)
	{
		if (render.texture_map.find(block_id) == render.texture_map.end())
			return false;

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, render.texture_map[block_id]);
		return true;
	}

	void draw_block(Renderer &render, int posX, int posY, int posZ)
	{
		glm::mat4 model = glm::mat4(1.0f);
		glm::vec3 posVec = glm::vec3((float)posX, (float)posY, (float)posZ);

		model = glm::translate(model, posVec);

		Shader::use(render.shaderID);
		Shader::setMat4(render.shaderID, "model", model);

		glBindVertexArray(render.blockVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}

	// * Wire
	int init_wire_render(Renderer &render)
	{
		unsigned int VAO, VBO;

		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);

		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(wire_verticies), wire_verticies, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		render.wireVAO = VAO;
		return 0;
	}

	void draw_wire(Renderer &render, int posX, int posY, int posZ)
	{
		const float SCALE_AMT = 0.0001f;

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

	// * Crosshair
	void init_crosshair(Renderer &render)
	{
		unsigned int VAO, VBO;
		glGenVertexArrays(1, &VAO);

		glGenBuffers(1, &VBO);
		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(crosshair_verticies), crosshair_verticies, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		render.crosshairVAO = VAO;
	}

	void draw_crosshair(Renderer &render)
	{
		Shader::use(render.crosshairShaderID);
		glBindVertexArray(render.crosshairVAO);
		glDrawArrays(GL_TRIANGLES, 0, 12);
	}

	// * Hotbar
	void init_hotbar(Renderer &render)
	{
		unsigned int VAO, VBO;
		glGenVertexArrays(1, &VAO);

		glGenBuffers(1, &VBO);
		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(hotbar_verticies), hotbar_verticies, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		render.hotbarVAO = VAO;
	}

	void init_hotbar_selector(Renderer &render)
	{
		unsigned int VAO, VBO;
		glGenVertexArrays(1, &VAO);

		glGenBuffers(1, &VBO);
		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(hotbar_selector_verticies), hotbar_selector_verticies, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		render.hotbarSelectorVAO = VAO;
	}

	void init_hotbar_texture(Renderer &render, std::string file_path)
	{
		Shader::use(render.hotbarShaderID);

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

		Shader::setInt(render.shaderID, "texture1", 0);
		render.hotbarTextureID = texture;
	}

	void init_hotbar_selector_texture(Renderer &render, std::string file_path)
	{
		Shader::use(render.hotbarShaderID);

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

		Shader::setInt(render.shaderID, "texture1", 0);
		render.hotbarSelectorTextureID = texture;
	}

	void draw_hotbar(Renderer &render)
	{
		Shader::use(render.hotbarShaderID);

		glm::mat4 model = glm::mat4(1.0f);
		Shader::setMat4(render.hotbarShaderID, "model", model);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, render.hotbarTextureID);
		glBindVertexArray(render.hotbarVAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}

	void draw_hotbar_selector(Renderer &render, unsigned int slot)
	{
		Shader::use(render.hotbarShaderID);

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3((slot - 1) * hotbar_slot_spacing, 0.0f, 0.0f));
		Shader::setMat4(render.hotbarShaderID, "model", model);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, render.hotbarSelectorTextureID);
		glBindVertexArray(render.hotbarSelectorVAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}

	// 3d items
	void init_3d_hotbar_item(Renderer &render)
	{
		Shader::use(render.item3dShaderID);
		Shader::set_projection_mat(render.item3dShaderID);

		glm::mat4 model = glm::mat4(1.0f);

		model = glm::scale(model, glm::vec3(0.2f));
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -5.0f));
		model = glm::rotate(model, glm::radians(30.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));

		glm::vec3 pos(0.0f, 0.0f, 5.0f);

		glm::mat4 view = glm::lookAt(pos,
									 pos + glm::vec3(0.0f, 0.0f, -1.0f),
									 glm::vec3(0.0f, 1.0f, 0.0f));

		Shader::setMat4(render.item3dShaderID, "model", model);
		Shader::setMat4(render.item3dShaderID, "view", view);
	}

	void draw_3d_hotbar_item(Renderer &render, BlockID block_id, unsigned int slot)
	{
		Shader::use(render.item3dShaderID);

		glm::mat4 translate = glm::mat4(1.0f);
		translate = glm::translate(translate,
								   glm::vec3(-(hotbar_slot_spacing * 4) + ((slot - 1) * hotbar_slot_spacing),
											 -1.0 + hotbar_height / 2,
											 -1.5f));

		Shader::setMat4(render.item3dShaderID, "translate", translate);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, render.texture_map[block_id]);

		glBindVertexArray(render.blockVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}
}
