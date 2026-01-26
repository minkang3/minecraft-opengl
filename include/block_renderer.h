#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <stb_image.h>

#include <shader.h>

inline float cube_verticies[] = {
	// left face
	-0.5f,  0.5f, -0.5f, 0.0f / 4.0f, 2.0f / 3.0f,
	-0.5f, -0.5f, -0.5f, 0.0f / 4.0f, 1.0f / 3.0f,
	-0.5f, -0.5f,  0.5f, 1.0f / 4.0f, 1.0f / 3.0f,
	-0.5f, -0.5f,  0.5f, 1.0f / 4.0f, 1.0f / 3.0f,
	-0.5f,  0.5f,  0.5f, 1.0f / 4.0f, 2.0f / 3.0f,
	-0.5f,  0.5f, -0.5f, 0.0f / 4.0f, 2.0f / 3.0f,
		  
	// front face
	-0.5f, -0.5f, 0.5f, 1.0f / 4.0f, 1.0f / 3.0f,
	0.5f, -0.5f, 0.5f, 2.0f / 4.0f, 1.0f / 3.0f,
	-0.5f,  0.5f, 0.5f, 1.0f / 4.0f, 2.0f / 3.0f,
	0.5f, -0.5f, 0.5f, 2.0f / 4.0f, 1.0f / 3.0f,
	-0.5f,  0.5f, 0.5f, 1.0f / 4.0f, 2.0f / 3.0f,
	0.5f,  0.5f, 0.5f, 2.0f / 4.0f, 2.0f / 3.0f,

	// right face
	0.5f,  0.5f, -0.5f, 3.0f / 4.0f, 2.0f / 3.0f, // top right
	0.5f, -0.5f, -0.5f, 3.0f / 4.0f, 1.0f / 3.0f, // bot right
	0.5f, -0.5f,  0.5f, 2.0f / 4.0f, 1.0f / 3.0f, // bot left
	0.5f, -0.5f,  0.5f, 2.0f / 4.0f, 1.0f / 3.0f, // bot left
	0.5f,  0.5f,  0.5f, 2.0f / 4.0f, 2.0f / 3.0f, // top left
	0.5f,  0.5f, -0.5f, 3.0f / 4.0f, 2.0f / 3.0f, // top right

	// back face
	-0.5f, -0.5f, -0.5f, 4.0f / 4.0f, 1.0f / 3.0f,
	0.5f, -0.5f, -0.5f, 3.0f / 4.0f, 1.0f / 3.0f,
	-0.5f,  0.5f, -0.5f, 4.0f / 4.0f, 2.0f / 3.0f,
	0.5f, -0.5f, -0.5f, 3.0f / 4.0f, 1.0f / 3.0f,
	-0.5f,  0.5f, -0.5f, 4.0f / 4.0f, 2.0f / 3.0f,
	0.5f,  0.5f, -0.5f, 3.0f / 4.0f, 2.0f / 3.0f,

	// top face
	-0.5f, 0.5f, -0.5f, 1.0f / 4.0f, 3.0f / 3.0f, // top left
	-0.5f, 0.5f, 0.5f,  1.0f / 4.0f, 2.0f / 3.0f, // bot left
	0.5f, 0.5f, 0.5f,   2.0f / 4.0f, 2.0f / 3.0f, // bot right
	0.5f, 0.5f, 0.5f,   2.0f / 4.0f, 2.0f / 3.0f, // bot right
	0.5f, 0.5f, -0.5f,  2.0f / 4.0f, 3.0f / 3.0f, // top right
	-0.5f, 0.5f, -0.5f, 1.0f / 4.0f, 3.0f / 3.0f, // top left

	// bot face
	-0.5f, -0.5f, -0.5f, 1.0f / 4.0f, 1.0f / 3.0f, // top left 
	-0.5f, -0.5f, 0.5f,  1.0f / 4.0f, 0.0f / 3.0f, // bot left 
	0.5f,  -0.5f, 0.5f,  2.0f / 4.0f, 0.0f / 3.0f, // bot right
	0.5f,  -0.5f, 0.5f,  2.0f / 4.0f, 0.0f / 3.0f, // bot right
	0.5f,  -0.5f, -0.5f, 2.0f / 4.0f, 1.0f / 3.0f, // top right
	-0.5f, -0.5f, -0.5f, 1.0f / 4.0f, 1.0f / 3.0f, // top left 
};

class BlockRenderer
{
private:
	unsigned int VAO, VBO;
	Shader &shader;
	glm::mat4 model;

	std::unordered_map<std::string, unsigned int> texture_map;
public:
	BlockRenderer(Shader &shader)
		: shader(shader)
	{
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);

		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(cube_verticies), cube_verticies, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
	}

	void init_texture(std::string key, std::string file_path)
	{
		unsigned int texture;
		// texture
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);

		// set the texture wrapping parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		// set texture filtering parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		int width, height, nrChannels;
		stbi_set_flip_vertically_on_load(true);
		unsigned char *data = stbi_load(file_path.c_str(), &width, &height, &nrChannels, 0);

		if (data) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
		} else {
			std::cout << "Failed to load image\n";
		}
		stbi_image_free(data);

		std::cout << "Setting " << key << " to " << texture << std::endl;
		texture_map[key] = texture;

		shader.setInt("texture1", texture);
	}

	bool set_texture(std::string key)
	{
		if (texture_map.find(key) == texture_map.end())
			return false;

		//std::cout << "texture_map[" << key << "]: " << texture_map[key] << std::endl;

		//shader.setInt("texture1", texture_map[key]);
		glBindTexture(GL_TEXTURE_2D, texture_map[key]);
		
		return true;
	}

	void draw(int posX, int posY, int posZ)
	{
		shader.use();
		model = glm::mat4(1.0f);
		glm::vec3 posVec = glm::vec3((float)posX, (float)posY, (float)posZ);
		model = glm::translate(model, posVec);
		//model = glm::rotate(model, glm::radians(50.0f * (float)glfwGetTime()), glm::vec3(0.0f, 1.0f, 0.0f));
		shader.setMat4("model", model);

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}
};
