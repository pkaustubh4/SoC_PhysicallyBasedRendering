#pragma once

#include <string>
#include <unordered_map>

#include "glm/glm.hpp"

struct ShaderProgramSource {

	std::string VertexSource;
	std::string FragmentSource;
};

class Shader
{
private:
	std::string m_filepath;
	unsigned int m_rendererID;
	// Caching for uniforms

	std::unordered_map<std::string, int> m_uniformLocationCache;

public:
	Shader(const std::string& filepath);
	~Shader();

	void Bind() const;
	void Unbind() const;

	// Setting uniforms
	void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
	void SetUniformMat4f(const std::string& name, const glm::mat4& matrix);
	
private:
	unsigned int CompileShader(unsigned int type, const std::string& source);
	unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
	ShaderProgramSource ParseShader(const std::string& filepath);
	unsigned int GetUniformLocation(const std::string& name);
};