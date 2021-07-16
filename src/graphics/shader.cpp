#include "pch.h"
#include "graphics/shader.h"
#include "graphics/gl/glad/glad.h"

#include "file/filedevice.h"

#include <fstream>

namespace engine
{
	GLuint CreateShader(GLenum shaderType, const std::string& filename)
	{
		std::ifstream ifs(filename);

		if (!ifs.is_open()) {
			spdlog::error("CreateShader: failed to open file {}", filename.c_str());
			std::terminate();
		}
			
		std::string content((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));
		const char* contentCStr = content.c_str();

		GLuint shader = glCreateShader(shaderType);
		glShaderSource(shader, 1, &contentCStr, NULL);
		glCompileShader(shader);

		int  success;
		char infoLog[512];
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

		if (!success)
		{
			glGetShaderInfoLog(shader, 512, NULL, infoLog);
			spdlog::error("Failed to compile shader {}\n{}", filename.c_str(), infoLog);
		}

		spdlog::info("created shader from file {}", filename.c_str());
		return shader;
	}

	Shader::Shader(const eastl::string& name, const eastl::string& vertexPath, const eastl::string& fragmentPath)
	{
		create(name, vertexPath, fragmentPath);
	}

	Shader::Shader()
	{

	}

	Shader::Shader(const eastl::string& name, const eastl::string& vertexPath, const eastl::string& fragmentPath, const eastl::vector<VertexDeclaration>& vertexDecls)
	{
		assert(!vertexDecls.empty());
		m_vertexDeclarations = vertexDecls;

		m_allVxDeclsSize = 0;
		for (int i = 0, size = m_vertexDeclarations.size(); i < size; i++)
			m_allVxDeclsSize += m_vertexDeclarations[i].size;

		create(name, vertexPath, fragmentPath);
	}

	Shader::~Shader()
	{
		//glDeleteProgram(m_Program);
	}

	bool Shader::create(const eastl::string& name, const eastl::string& vertexPath, const eastl::string& fragmentPath)
	{
		m_Name = name;
		m_VSName = vertexPath;
		m_FSName = fragmentPath;

		GLuint vertexShader = CreateShader(GL_VERTEX_SHADER, vertexPath.c_str());
		GLuint fragmentShader = CreateShader(GL_FRAGMENT_SHADER, fragmentPath.c_str());

		if (vertexShader == 0 || fragmentShader == 0)
			return false;

		m_Program = glCreateProgram();
		glAttachShader(m_Program, vertexShader);
		glAttachShader(m_Program, fragmentShader);

		glLinkProgram(m_Program);

		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

		int  success;
		char infoLog[512];
		glGetProgramiv(m_Program, GL_LINK_STATUS, &success);

		if (!success)
		{
			glGetProgramInfoLog(m_Program, 512, NULL, infoLog);
			spdlog::error("Failed to link program {}", infoLog);
			return false;
		}

		return true;
	}

	void Shader::destroy()
	{
		glDeleteProgram(m_Program);
	}

	void Shader::bind()
	{
		glUseProgram(m_Program);

		size_t appliedOffset = 0;
		for (int i = 0, size = m_vertexDeclarations.size(); i < size; i++)
		{
			VertexDeclaration& vxdecl = m_vertexDeclarations[i];

			glVertexAttribPointer(i, vxdecl.size, GL_FLOAT, GL_FALSE, m_allVxDeclsSize * sizeof(float), (void*)(appliedOffset * sizeof(float)));
			glEnableVertexAttribArray(i);

			//glVertexAttribPointer(i, vxdecl.size, GL_FLOAT, GL_FALSE, m_allVxDeclsSize * sizeof(float), (void*)(appliedOffset * sizeof(float)));
			//glEnableVertexAttribArray(i);

			appliedOffset += vxdecl.size;
		}
	}

	void Shader::setInteger(const char* name, int value)
	{
		GLint uniformPosition = glGetUniformLocation(m_Program, name);
		glUniform1i(uniformPosition, value);
	}

	void Shader::setBoolean(const char* name, bool value)
	{
		GLint uniformPosition = glGetUniformLocation(m_Program, name);
		glUniform1i(uniformPosition, value);
	}

	void Shader::setFloat(const char* name, float value)
	{
		GLint uniformPosition = glGetUniformLocation(m_Program, name);
		glUniform1f(uniformPosition, value);
	}

	void Shader::setVec2(const char* name, const glm::vec2& vec)
	{
		GLint uniformPosition = glGetUniformLocation(m_Program, name);
		glUniform2fv(uniformPosition, 1, glm::value_ptr(vec));
	}

	void Shader::setVec3(const char* name, const glm::vec3& vec)
	{
		GLint uniformPosition = glGetUniformLocation(m_Program, name);
		glUniform3fv(uniformPosition, 1, glm::value_ptr(vec));
	}

	void Shader::setVec4(const char* name, const glm::vec4& vec)
	{
		GLint uniformPosition = glGetUniformLocation(m_Program, name);
		glUniform4fv(uniformPosition, 1, glm::value_ptr(vec));
	}

	void Shader::setMatrix(const char* name, const glm::mat4& matrix)
	{
		GLint uniformPosition = glGetUniformLocation(m_Program, name);
		glUniformMatrix4fv(uniformPosition, 1, GL_FALSE, &matrix[0][0]);
	}

	void Shader::hotReload()
	{
		spdlog::info("--- HOT RELOAD %s ---", m_Name.c_str());
		destroy();
		create(m_Name, m_VSName, m_FSName);
	}

}
