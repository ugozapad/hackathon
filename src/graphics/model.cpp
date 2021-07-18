#include "pch.h"
#include "model.h"

#include "file/filedevice.h"

#include "graphics/shader.h"
#include "graphics/material.h"

#include "graphics/graphicsdevice.h"

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

#ifdef _MSC_VER
#	ifdef _DEBUG
#		pragma comment(lib, "assimp-vc142-mtd.lib")
#	else
#		pragma comment(lib, "assimp-vc142-mt.lib")
#	endif
#endif // _MSC_VER

namespace engine
{
	inline static glm::mat4 Assimp2Glm(const aiMatrix4x4& from)
	{
		return glm::mat4(
			(double)from.a1, (double)from.b1, (double)from.c1, (double)from.d1,
			(double)from.a2, (double)from.b2, (double)from.c2, (double)from.d2,
			(double)from.a3, (double)from.b3, (double)from.c3, (double)from.d3,
			(double)from.a4, (double)from.b4, (double)from.c4, (double)from.d4
		);
	}

	SubMesh* ProccessSubMesh(aiMesh* mesh, aiNode* node, const aiScene* scene)
	{
		eastl::vector<Vertex> vertices;
		eastl::vector<uint32_t> indecies;

		for (uint32_t i = 0; i < mesh->mNumVertices; i++)
		{
			Vertex vertex;
			vertex.m_position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
			vertex.m_normal = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);

			if (mesh->mTextureCoords[0])
				vertex.m_texcoord0 = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
			else
				vertex.m_texcoord0 = glm::vec2(0.0f, 0.0f);

			if (mesh->mTextureCoords[1])
				vertex.m_texcoord1 = glm::vec2(mesh->mTextureCoords[1][i].x, mesh->mTextureCoords[1][i].y);
			else
				vertex.m_texcoord1 = glm::vec2(0.0f, 0.0f);

			//if (mesh->mTangents)
			//	vertex.m_tangent = glm::vec3(mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z);

			//if (mesh->mBitangents)
			//	vertex.m_bitangent = glm::vec3(mesh->mBitangents[i].x, mesh->mBitangents[i].y, mesh->mBitangents[i].z);

			vertices.push_back(vertex);
		}

		for (uint32_t i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace face = mesh->mFaces[i];

			for (uint32_t j = 0; j < face.mNumIndices; j++)
				indecies.push_back(face.mIndices[j]);
		}

		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

		char buffer[256];
		sprintf(buffer, "%s/data/materials/%s.material", FileDevice::getInstance()->getDefaultPath().c_str(), material->GetName().C_Str());

		if (!GetFileAttributesA(buffer))
		{
			aiString diffusePath;
			material->GetTexture(aiTextureType_DIFFUSE, 0, &diffusePath);

			aiString normalPath;
			material->GetTexture(aiTextureType_NORMALS, 0, &normalPath);

			Material::CreateMaterialFromImport(material->GetName().C_Str(), diffusePath.C_Str(), normalPath.C_Str());
		}

		aiMatrix4x4 nodePosition = node->mTransformation;
		glm::mat4 transform = glm::mat4(1.0f);
		transform = Assimp2Glm(nodePosition);

		return mem_new<SubMesh>(*g_sysAllocator, vertices, indecies, transform, material->GetName().C_Str());
	}

	void ProccessNode(eastl::vector<SubMesh*>& submeshes, aiNode* node, const aiScene* scene)
	{
		for (uint32_t i = 0; i < node->mNumMeshes; i++)
		{
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			SubMesh* submesh = ProccessSubMesh(mesh, node, scene);
			submeshes.push_back(submesh);
		}

		for (uint32_t i = 0; i < node->mNumChildren; i++)
		{
			ProccessNode(submeshes, node->mChildren[i], scene);
		}
	}

	void ModelBase::load(const eastl::string& filename)
	{
		m_filename = filename;
		spdlog::info("loading model {}", filename.c_str());

		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(filename.c_str(), aiProcess_Triangulate /*| aiProcess_TransformUVCoords | aiProcess_FlipUVs*/);

		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			spdlog::error("Failed to load model. {}", importer.GetErrorString());
			std::terminate();
		}

		assert(scene);
		ProccessNode(m_subMeshes, scene->mRootNode, scene);
	}

	void ModelBase::destroy()
	{
		for (int i = 0; i < m_subMeshes.size(); i++)
		{
			m_subMeshes[i]->destroy();
			mem_delete(*g_sysAllocator, m_subMeshes[i]);
		}

		m_subMeshes.clear();
	}

	void ModelBase::renderObjects()
	{
		for (int i = 0; i < m_subMeshes.size(); i++)
			m_subMeshes[i]->render();
	}

	SubMesh::SubMesh(eastl::vector<Vertex>& vertices, eastl::vector<uint32_t>& indecies, const glm::mat4& position, const char* materialname) :
		m_vertexBuffer(0),
		m_indexBuffer(0)
	{
		load(vertices, indecies, position, materialname);
	}

	SubMesh::SubMesh() :
		m_vertexBuffer(0),
		m_indexBuffer(0)
	{

	}

	void SubMesh::load(eastl::vector<Vertex>& vertices, eastl::vector<uint32_t>& indecies, const glm::mat4& position, const char* materialname)
	{
		m_transform = position;

		BufferCreationDesc vertexBufferDesc = { 0 };
		vertexBufferDesc.m_data = vertices.data();
		vertexBufferDesc.m_dataSize = vertices.size() * sizeof(Vertex);
		vertexBufferDesc.m_access = BufferAccess::Static;
		m_vertexBuffer = GraphicsDevice::getInstance()->createVertexBuffer(vertexBufferDesc);

		BufferCreationDesc indexBufferDesc = { 0 };
		indexBufferDesc.m_data = indecies.data();
		indexBufferDesc.m_dataSize = indecies.size() * sizeof(uint32_t);
		indexBufferDesc.m_access = BufferAccess::Static;
		m_indexBuffer = GraphicsDevice::getInstance()->createIndexBuffer(indexBufferDesc);

		m_verticesCount = vertices.size();
		m_indeciesCount = indecies.size();

		//m_material = g_materialSystem.CreateMaterial(materialname);
	}

	SubMesh::~SubMesh()
	{
		//	Destroy();
	}

	void SubMesh::destroy()
	{
		if (m_indexBuffer)
			GraphicsDevice::getInstance()->deleteIndexBuffer(m_indexBuffer);

		if (m_vertexBuffer)
			GraphicsDevice::getInstance()->deleteVertexBuffer(m_vertexBuffer);
	}

	void SubMesh::render()
	{
		// create saved render ctx as previous model.
		//RenderContext savedCtx = RenderContext::GetContext();

		//// create local copy of render context
		//RenderContext localCtx = RenderContext::GetContext();

		//// and overwrite model matrix
		//localCtx.model = savedCtx.model * m_transform;

		//// set our local render ctx
		//RenderContext::SetContext(localCtx);

		//g_renderDevice->SetVertexBuffer(m_vertexBuffer);
		//g_renderDevice->SetIndexBuffer(m_indexBuffer);

		//m_material->Bind();

		//// render
		//g_renderDevice->DrawElement(PM_TRIANGLES, m_indeciesCount);

		//// return what have been
		//RenderContext::SetContext(savedCtx);
	}

}
