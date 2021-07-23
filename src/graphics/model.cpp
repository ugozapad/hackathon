#include "pch.h"
#include "model.h"

#include "file/filedevice.h"

#include "engine/content/contentmanager.h"

#include "graphics/shader.h"
#include "graphics/material.h"

#include "graphics/graphicsdevice.h"
#include "graphics/rendercontext.h"

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include <assimp/IOStream.hpp>
#include <assimp/IOSystem.hpp>

#ifdef _MSC_VER
#	ifdef _DEBUG
#		pragma comment(lib, "assimp-vc142-mtd.lib")
#	else
#		pragma comment(lib, "assimp-vc142-mt.lib")
#	endif
#endif // _MSC_VER

namespace engine
{
	class AssImpStream : public Assimp::IOStream
	{
	private:
		std::shared_ptr<DataStream> m_dataStream;
	public:
		AssImpStream(const std::shared_ptr<DataStream>& dataStream)
		{
			m_dataStream = dataStream;
		}

		size_t Read(void* pvBuffer, size_t pSize, size_t pCount) override
		{
			size_t beginReadSize = m_dataStream->tell();
			m_dataStream->read(pvBuffer, pCount);
			size_t endReadSize = m_dataStream->tell();

			return endReadSize - beginReadSize;
		}

		size_t Write(const void* pvBuffer, size_t pSize, size_t pCount) override
		{
			size_t beginWriteSize = m_dataStream->tell();
			m_dataStream->write(const_cast<void*>(pvBuffer), pCount);
			size_t endWriteSize = m_dataStream->tell();

			return endWriteSize - beginWriteSize;
		}

		aiReturn Seek(size_t pOffset, aiOrigin pOrigin) override
		{
			if (m_dataStream->eof())
				return aiReturn_FAILURE;

			FileSeek seekDirection;

			switch (pOrigin)
			{
			case aiOrigin_SET:
				seekDirection = FileSeek::Begin;
				break;
			case aiOrigin_CUR:
				seekDirection = FileSeek::Current;
				break;
			case aiOrigin_END:
				seekDirection = FileSeek::End;
				break;
			default:
				break;
			}

			m_dataStream->seek(seekDirection, pOffset);

			return aiReturn_SUCCESS;
		}

		size_t Tell() const override
		{
			return m_dataStream->tell();
		}

		size_t FileSize() const override
		{
			size_t currentPosition = m_dataStream->tell();

			m_dataStream->seek(FileSeek::End, 0);
			size_t filesize = m_dataStream->tell();
			m_dataStream->seek(FileSeek::Begin, currentPosition);

			return filesize;
		}

		void Flush() override
		{
			return m_dataStream->flush();
		}

	};

	class AssImpIOSystem : public Assimp::IOSystem
	{
	private:
		std::shared_ptr<DataStream> m_dataStream;
	public:
		AssImpIOSystem(const std::shared_ptr<DataStream>& dataStream)
		{
			m_dataStream = dataStream;
		}

		bool Exists(const char* pFile) const override
		{
			File* file = FileDevice::getInstance()->openFile(pFile, FileAccess::Read);
			bool exist = file->isValid();
			FileDevice::getInstance()->closeFile(file);

			//return exist;
			return true;
		}


		char getOsSeparator() const override
		{
			return '/';
		}


		Assimp::IOStream* Open(const char* pFile, const char* pMode = "rb") override
		{
			//return (Assimp::IOStream*)mem_new<AssImpStream>(*g_sysAllocator, m_dataStream);
			return (Assimp::IOStream*)new AssImpStream(m_dataStream);
		}

		void Close(Assimp::IOStream* pFile) override
		{
			//mem_delete(*g_sysAllocator, pFile);
			delete pFile;
		}
	};

	inline static glm::mat4 Assimp2Glm(const aiMatrix4x4& from)
	{
		return glm::mat4(
			(double)from.a1, (double)from.b1, (double)from.c1, (double)from.d1,
			(double)from.a2, (double)from.b2, (double)from.c2, (double)from.d2,
			(double)from.a3, (double)from.b3, (double)from.c3, (double)from.d3,
			(double)from.a4, (double)from.b4, (double)from.c4, (double)from.d4
		);
	}

	bool materialExists(const char* pFile)
	{
		File* file = FileDevice::getInstance()->openFile(pFile, FileAccess::Read);
		bool exist = file->isValid();
		FileDevice::getInstance()->closeFile(file);

		return exist;
	}


	SubMesh* ProccessSubMesh(aiMesh* mesh, aiNode* node, const aiScene* scene)
	{
		std::vector<Vertex> vertices;
		std::vector<uint32_t> indecies;

		for (uint32_t i = 0; i < mesh->mNumVertices; i++)
		{
			Vertex vertex;
			vertex.m_position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
			vertex.m_normal = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);

			if (mesh->mTextureCoords[0])
				vertex.m_texcoord0 = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
			else
				vertex.m_texcoord0 = glm::vec2(0.0f, 0.0f);

			//if (mesh->mTextureCoords[1])
			//	vertex.m_texcoord1 = glm::vec2(mesh->mTextureCoords[1][i].x, mesh->mTextureCoords[1][i].y);
			//else
			//	vertex.m_texcoord1 = glm::vec2(0.0f, 0.0f);

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
		sprintf(buffer, "data/materials/%s.material", material->GetName().C_Str());

		if (!materialExists(buffer))
		{
			aiString diffusePath;
			material->GetTexture(aiTextureType_DIFFUSE, 0, &diffusePath);

			if (diffusePath.length == 0)
				material->GetTexture(aiTextureType_BASE_COLOR, 0, &diffusePath);

			aiString normalPath;
			material->GetTexture(aiTextureType_NORMALS, 0, &normalPath);

			Material::createMaterialFromImport(material->GetName().C_Str(), diffusePath.C_Str(), normalPath.C_Str());
		}

		aiMatrix4x4 nodePosition = node->mTransformation;
		glm::mat4 transform = glm::mat4(1.0f);
		transform = Assimp2Glm(nodePosition);

		return mem_new<SubMesh>(*g_sysAllocator, vertices, indecies, transform, material->GetName().C_Str());
	}

	void ProccessNode(std::vector<SubMesh*>& submeshes, aiNode* node, const aiScene* scene)
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

	ModelBase::ModelBase(const std::string& name) : Content(name)
	{

	}

	void ModelBase::load(const std::string& filename)
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

	void ModelBase::load(const std::shared_ptr<DataStream>& dataStream)
	{
		AssImpIOSystem* assimpIoSystem = new AssImpIOSystem(dataStream);

		Assimp::Importer importer;
		importer.SetIOHandler(assimpIoSystem);
		const aiScene* scene = importer.ReadFile(m_filename.c_str(), aiProcess_Triangulate /*| aiProcess_TransformUVCoords | aiProcess_FlipUVs*/);

		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			spdlog::error("Failed to load model. {}", importer.GetErrorString());
			std::terminate();
		}

		assert(scene);
		ProccessNode(m_subMeshes, scene->mRootNode, scene);

		//delete assimpIoSystem;
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

	void ModelBase::createHwShit()
	{
		for (int i = 0; i < m_subMeshes.size(); i++)
		{
			SubMesh* submesh = m_subMeshes[i];

			// create vertex and index buffer
			BufferCreationDesc vertexBufferDesc = { 0 };
			vertexBufferDesc.m_data = submesh->m_vertices.data();
			vertexBufferDesc.m_dataSize = submesh->m_vertices.size() * sizeof(Vertex);
			vertexBufferDesc.m_access = BufferAccess::Static;
			submesh->m_vertexBuffer = GraphicsDevice::getInstance()->createVertexBuffer(vertexBufferDesc);
			submesh->m_vertices.clear();

			BufferCreationDesc indexBufferDesc = { 0 };
			indexBufferDesc.m_data = submesh->m_indecies.data();
			indexBufferDesc.m_dataSize = submesh->m_indecies.size() * sizeof(uint32_t);
			indexBufferDesc.m_access = BufferAccess::Static;
			submesh->m_indexBuffer = GraphicsDevice::getInstance()->createIndexBuffer(indexBufferDesc);
			submesh->m_indecies.clear();

			submesh->m_material = ContentManager::getInstance()->loadMaterial(submesh->m_materialName);
		}
	}

	void ModelBase::renderObjects()
	{
		for (int i = 0; i < m_subMeshes.size(); i++)
			m_subMeshes[i]->render();
	}

	SubMesh::SubMesh(std::vector<Vertex>& vertices, std::vector<uint32_t>& indecies, const glm::mat4& position, const char* materialname) :
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

	void SubMesh::load(std::vector<Vertex>& vertices, std::vector<uint32_t>& indecies, const glm::mat4& position, const char* materialname)
	{
		//m_materialName = materialname;
		m_materialName += "data/materials/";
		m_materialName += materialname;
		m_materialName += ".material";

		m_transform = position;

		m_vertices = vertices;
		m_indecies = indecies;
		m_verticesCount = vertices.size();
		m_indeciesCount = indecies.size();

		//m_material = g_materialSystem.CreateMaterial(materialname);
		//m_material = ContentManager::getInstance()->loadMaterial(materialname);
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
		//create saved render ctx as previous model.
		RenderContext savedCtx = RenderContext::getContext();

		// create local copy of render context
		RenderContext localCtx = RenderContext::getContext();

		// and overwrite model matrix
		localCtx.model = savedCtx.model * m_transform;

		// set our local render ctx
		RenderContext::setContext(localCtx);

		GraphicsDevice::getInstance()->setVertexBuffer(m_vertexBuffer);
		GraphicsDevice::getInstance()->setIndexBuffer(m_indexBuffer);

		m_material->bind();

		GraphicsDevice::getInstance()->drawArray(PrimitiveMode::Triangles, 0, m_verticesCount);

		// return what have been
		RenderContext::setContext(savedCtx);
	}
}
