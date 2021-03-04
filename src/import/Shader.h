#pragma once
#include <GLFW/glfw3.h>
#include <string>
#include <glm.hpp>

class Shader
{
public:
    unsigned int ID;

    Shader(std::string vertexPath, std::string fragmentPath);

    void use() const;

    void setBool(const std::string& name, bool value) const;
    void setVec4(const std::string& name, const glm::vec4& value) const;
    void setMat4(const std::string& name, const glm::mat4& mat) const;

private:
    void checkCompileErrors(GLuint shader, std::string type);
};