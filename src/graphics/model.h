#ifndef MODEL_H
#define MODEL_H

#include "engine/content/content.h"

namespace engine
{
	struct Vertex
	{
		glm::vec3 m_position;
		glm::vec3 m_normal;
		glm::vec2 m_texcoord0;
		//glm::vec2 m_texcoord1;

		//glm::vec3 m_tangent;
		//glm::vec3 m_bitangent;
	};

	class ModelBase;
	class GrVertexBuffer;
	class GrIndexBuffer;
	class Material;

	class SubMesh
	{
		friend class ModelBase;
	public:
		SubMesh();
		SubMesh(eastl::vector<Vertex>& vertices, eastl::vector<uint32_t>& indecies, const glm::mat4& position, const char* materialname);
		void load(eastl::vector<Vertex>& vertices, eastl::vector<uint32_t>& indecies, const glm::mat4& position, const char* materialname);

		~SubMesh();

		void destroy();

		void render();

		GrVertexBuffer* getVertexBuffer() { return m_vertexBuffer; }
		GrIndexBuffer* getIndexBuffer() { return m_indexBuffer; }

		uint32_t getVerticesCount() { return m_verticesCount; }
		uint32_t getIndeciesCount() { return m_indeciesCount; }

	private:
		Material* m_material;

		GrVertexBuffer* m_vertexBuffer;
		GrIndexBuffer* m_indexBuffer;

		glm::mat4 m_transform;

		uint32_t m_verticesCount;
		uint32_t m_indeciesCount;
	};

	class ModelBase : public Content
	{
	public:
		virtual void load(const eastl::shared_ptr<DataStream>& dataStream);

		virtual void load(const eastl::string& filename);
		virtual void destroy();

		virtual void renderObjects();

		const eastl::string& getFileName() { return m_filename; }

	private:
		eastl::vector<SubMesh*> m_subMeshes;
		eastl::string m_filename;
	};
}

#endif // !MODEL_H
