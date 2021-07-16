#ifndef MODEL_H
#define MODEL_H

namespace engine
{
	struct Vertex
	{
		glm::vec3 m_position;
		glm::vec3 m_normal;
		glm::vec2 m_texcoord0;
		glm::vec2 m_texcoord1;

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
		SubMesh(std::vector<Vertex>& vertices, std::vector<uint32_t>& indecies, const glm::mat4& position, const char* materialname);
		void Load(std::vector<Vertex>& vertices, std::vector<uint32_t>& indecies, const glm::mat4& position, const char* materialname);

		~SubMesh();

		void Destroy();

		void Render();

		GrVertexBuffer* GetVertexBuffer() { return m_vertexBuffer; }
		GrIndexBuffer* GetIndexBuffer() { return m_indexBuffer; }

		uint32_t GetVerticesCount() { return m_verticesCount; }
		uint32_t GetIndeciesCount() { return m_indeciesCount; }

	private:
		Material* m_material;

		GrVertexBuffer* m_vertexBuffer;
		GrIndexBuffer* m_indexBuffer;

		glm::mat4 m_transform;

		uint32_t m_verticesCount;
		uint32_t m_indeciesCount;
	};

	class ModelBase
	{
	public:
		virtual void Load(const std::string& filename);
		virtual void Destroy();

		virtual void RenderObjects();

		const std::string& GetFileName() { return m_filename; }

	private:
		std::vector<SubMesh*> m_subMeshes;
		std::string m_filename;
	};
}

#endif // !MODEL_H
