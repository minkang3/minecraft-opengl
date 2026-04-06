#pragma once

#ifdef __EMSCRIPTEN__
#include <GLES3/gl3.h>
#else
#include <glad/glad.h>
#endif
#include <glm/glm.hpp>

#include <render.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#define VERTEX_PATH "shaders/shader.vs"
#define FRAGMENT_PATH "shaders/shader.fs"

namespace Shader
{
    int init(ShaderID &shaderID, std::string vertex_path, std::string fragment_path);
    int set_projection_mat(ShaderID ID);
    void set_projection_mat_to_identity(ShaderID ID);
    void use(ShaderID ID);
    void setBool(ShaderID ID, const std::string &name, bool value);
    void setInt(ShaderID ID, const std::string &name, int value);
    void setFloat(ShaderID ID, const std::string &name, float value);
    void setVec2(ShaderID ID, const std::string &name, const glm::vec2 &value);
    void setVec2(ShaderID ID, const std::string &name, float x, float y);
    void setVec3(ShaderID ID, const std::string &name, const glm::vec3 &value);
    void setVec3(ShaderID ID, const std::string &name, float x, float y, float z);
    void setVec4(ShaderID ID, const std::string &name, const glm::vec4 &value);
    void setVec4(ShaderID ID, const std::string &name, float x, float y, float z, float w);
    void setMat2(ShaderID ID, const std::string &name, const glm::mat2 &mat);
    void setMat3(ShaderID ID, const std::string &name, const glm::mat3 &mat);
    void setMat4(ShaderID ID, const std::string &name, const glm::mat4 &mat);
    void checkCompileErrors(GLuint shader, std::string type);    
}
