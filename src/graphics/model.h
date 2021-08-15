#ifndef MODEL_H
#define MODEL_H

#include "engine/content/content.h"

#include "graphics/shader.h"

namespace engine
{
	struct Vertex
	{
		glm::vec3 m_position;
		glm::vec3 m_normal;
		glm::vec2 m_texcoord0;
		//glm::vec2 m_texcoord1;

		glm::vec3 m_tangent;
		glm::vec3 m_bitangent;
	};

	class ModelBase;
	class GrVertexBuffer;
	class GrIndexBuffer;
	class Material;

	GrVertexBuffer* createVBFromModel(VertexDeclaration* meshDecs, int countOfVertexDecls,const std::string& filename, int* verticesCount);

	class SubMesh
	{
		friend class ModelBase;
	public:
		SubMesh();
		SubMesh(std::vector<Vertex>& vertices, std::vector<uint32_t>& indecies, const glm::mat4& position, const char* materialname);
		void load(std::vector<Vertex>& vertices, std::vector<uint32_t>& indecies, const glm::mat4& position, const char* materialname);

		~SubMesh();

		void destroy();

		void render();

		GrVertexBuffer* getVertexBuffer() { return m_vertexBuffer; }
		GrIndexBuffer* getIndexBuffer() { return m_indexBuffer; }

		uint32_t getVerticesCount() { return m_verticesCount; }
		uint32_t getIndeciesCount() { return m_indeciesCount; }

	private:
		std::shared_ptr<Material> m_material;
		std::string m_materialName;

		GrVertexBuffer* m_vertexBuffer;
		GrIndexBuffer* m_indexBuffer;

		std::vector<Vertex> m_vertices;
		std::vector<uint32_t> m_indecies;

		glm::mat4 m_transform;

		uint32_t m_verticesCount;
		uint32_t m_indeciesCount;
	};

	class ModelBase : public Content
	{
	public:
		ModelBase(const std::string& name);
		~ModelBase();

		virtual void load(const std::shared_ptr<DataStream>& dataStream);

		virtual void load(const std::string& filename);
		virtual void destroy();

		virtual void createHwShit();

		virtual void renderObjects();

		const std::string& getFileName() { return m_filename; }

		

	private:
		std::vector<SubMesh*> m_subMeshes;
	};
}

#endif // !MODEL_H
