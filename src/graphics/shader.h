#ifndef SHADER_H
#define SHADER_H

namespace engine
{
	struct VertexDeclaration
	{
		enum
		{
			VD_FLOAT,
			VD_INTEGER
		};

		eastl::string name;
		uint32_t type;
		uint32_t size;
		uint32_t offset;
	};

	class Shader
	{
	public:
		Shader();
		Shader(const eastl::string& name, const eastl::string& vertexPath, const eastl::string& fragmentPath);
		Shader(const eastl::string& name, const eastl::string& vertexPath, const eastl::string& fragmentPath, const eastl::vector<VertexDeclaration>& vertexDecls);
		~Shader();
		bool create(const eastl::string& name, const eastl::string& vertexPath, const eastl::string& fragmentPath);
		void destroy();
		void bind();

		void setInteger(const char* name, int value);
		void setBoolean(const char* name, bool value);
		void setFloat(const char* name, float value);
		void setVec2(const char* name, const glm::vec2& vec);
		void setVec3(const char* name, const glm::vec3& vec);
		void setVec4(const char* name, const glm::vec4& vec);
		void setMatrix(const char* name, const glm::mat4& matrix);

		eastl::string& getName() { return m_name; }

		void hotReload();

	private:
		eastl::vector<VertexDeclaration> m_vertexDeclarations;

		eastl::string m_name;
		eastl::string m_vsName;
		eastl::string m_fsName;
		uint32_t m_program;

		size_t m_allVxDeclsSize;
	};

}

#endif // !SHADER_H
