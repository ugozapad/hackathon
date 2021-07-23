#ifndef SHADER_H
#define SHADER_H

namespace engine
{
	struct VertexDeclaration
	{
		enum
		{
			Float,
			Integer
		};

		std::string name;
		uint32_t type;
		uint32_t size;
		uint32_t offset;
	};

	class Shader
	{
	public:
		Shader();
		Shader(const std::string& name, const std::string& vertexPath, const std::string& fragmentPath);
		~Shader();
		bool create(const std::string& name, const std::string& vertexPath, const std::string& fragmentPath);
		void destroy();
		void bind();

		void setInteger(const char* name, int value);
		void setBoolean(const char* name, bool value);
		void setFloat(const char* name, float value);
		void setVec2(const char* name, const glm::vec2& vec);
		void setVec3(const char* name, const glm::vec3& vec);
		void setVec4(const char* name, const glm::vec4& vec);
		void setMatrix(const char* name, const glm::mat4& matrix);

		std::string& getName() { return m_name; }

		void hotReload();

		void addVertexDeclaration(const VertexDeclaration& vecdecl);

	private:
		std::vector<VertexDeclaration> m_vertexDeclarations;

		std::string m_name;
		std::string m_vsName;
		std::string m_fsName;
		uint32_t m_program;

		size_t m_allVxDeclsSize;
	};

}

#endif // !SHADER_H
