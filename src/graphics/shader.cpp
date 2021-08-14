#include "pch.h"
#include "graphics/shader.h"
#include "graphics/gl/glad/glad.h"

#include "file/filedevice.h"

#include <fstream>

namespace engine
{
	GLuint createShader(GLenum shaderType, const std::string& filename)
	{
		File* file = FileDevice::instance()->openFile(filename.c_str(), FileAccess::Read);

		if (!file->isValid()) {
			Core::error("createShader: failed to open file %s", filename.c_str());
		}

		std::string content;
		file->readString(content);
	
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
			Core::error("Failed to compile shader %s\n%s", filename.c_str(), infoLog);
		}

		spdlog::info("created shader from file {}", filename.c_str());
		return shader;
	}

	Shader::Shader(const std::string& name, const std::string& vertexPath, const std::string& fragmentPath)
	{
		m_allVxDeclsSize = 0;
		create(name, vertexPath, fragmentPath);
	}

	Shader::Shader()
	{
		m_allVxDeclsSize = 0;
	}

	Shader::~Shader()
	{
		//glDeleteProgram(m_Program);
	}

	bool Shader::create(const std::string& name, const std::string& vertexPath, const std::string& fragmentPath)
	{
		if (m_allVxDeclsSize == 0)
			m_allVxDeclsSize = 0;

		m_name = name;
		m_vsName = vertexPath;
		m_fsName = fragmentPath;

		GLuint vertexShader = createShader(GL_VERTEX_SHADER, vertexPath.c_str());
		GLuint fragmentShader = createShader(GL_FRAGMENT_SHADER, fragmentPath.c_str());

		if (vertexShader == 0 || fragmentShader == 0)
			return false;

		m_program = glCreateProgram();
		glAttachShader(m_program, vertexShader);
		glAttachShader(m_program, fragmentShader);

		glLinkProgram(m_program);

		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

		int  success;
		char infoLog[512];
		glGetProgramiv(m_program, GL_LINK_STATUS, &success);

		if (!success)
		{
			glGetProgramInfoLog(m_program, 512, NULL, infoLog);
			Core::error("Failed to link program %s", infoLog);
			return false;
		}

		return true;
	}

	void Shader::destroy()
	{
		glDeleteProgram(m_program);
	}

	void Shader::bind()
	{
		glUseProgram(m_program);

		size_t appliedOffset = 0;
		for (int i = 0, size = m_vertexDeclarations.size(); i < size; i++)
		{
			VertexDeclaration& vxdecl = m_vertexDeclarations[i];
			glVertexAttribPointer(i, vxdecl.size, GL_FLOAT, GL_FALSE, m_allVxDeclsSize * sizeof(float), (void*)(appliedOffset * sizeof(float)));
			glEnableVertexAttribArray(i);

			appliedOffset += vxdecl.size;
		}
	}

	void Shader::setInteger(const char* name, int value)
	{
		GLint uniformPosition = glGetUniformLocation(m_program, name);
		glUniform1i(uniformPosition, value);
	}

	void Shader::setBoolean(const char* name, bool value)
	{
		GLint uniformPosition = glGetUniformLocation(m_program, name);
		glUniform1i(uniformPosition, value);
	}

	void Shader::setFloat(const char* name, float value)
	{
		GLint uniformPosition = glGetUniformLocation(m_program, name);
		glUniform1f(uniformPosition, value);
	}

	void Shader::setVec2(const char* name, const glm::vec2& vec)
	{
		GLint uniformPosition = glGetUniformLocation(m_program, name);
		glUniform2fv(uniformPosition, 1, glm::value_ptr(vec));
	}

	void Shader::setVec3(const char* name, const glm::vec3& vec)
	{
		GLint uniformPosition = glGetUniformLocation(m_program, name);
		glUniform3fv(uniformPosition, 1, glm::value_ptr(vec));
	}

	void Shader::setVec4(const char* name, const glm::vec4& vec)
	{
		GLint uniformPosition = glGetUniformLocation(m_program, name);
		glUniform4fv(uniformPosition, 1, glm::value_ptr(vec));
	}

	void Shader::setMatrix(const char* name, const glm::mat4& matrix)
	{
		GLint uniformPosition = glGetUniformLocation(m_program, name);
		glUniformMatrix4fv(uniformPosition, 1, GL_FALSE, &matrix[0][0]);
	}

	void Shader::hotReload()
	{
		spdlog::info("--- RELOAD {} ---", m_name.c_str());
		destroy();
		create(m_name, m_vsName, m_fsName);
	}

	void Shader::addVertexDeclaration(const VertexDeclaration& vecdecl)
	{
		m_allVxDeclsSize += vecdecl.size;
		m_vertexDeclarations.push_back(vecdecl);
	}

}
