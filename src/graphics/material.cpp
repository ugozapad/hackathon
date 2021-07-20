#include "pch.h"
#include "graphics/material.h"
#include "graphics/texturemap.h"
#include "graphics/shader.h"
#include "graphics/graphicsdevice.h"

#include "file/filedevice.h"

#include "engine/camera.h"
#include "engine/content/contentmanager.h"

#include <sstream>

namespace engine
{
	void Material::createMaterialFromImport(const char* name, const char* diffuseName, const char* normalName)
	{
		eastl::string materiaName;
		materiaName += "data/materials/";
		materiaName += name;
		materiaName += ".material";

		File* file = FileDevice::getInstance()->openFile(materiaName, FileAccess::Write);
	
		char buffer[256];
		int len = sprintf(buffer, "material \"%s\"\n", name);
		file->write(buffer, len);

		len = sprintf(buffer, "{\n");
		file->write(buffer, len);

		len = sprintf(buffer, "\tshader diffuse\n");
		file->write(buffer, len);

		const char* newAlbedoName;

		if (diffuseName && diffuseName[0] == '\0')
			newAlbedoName = "notexture.png";
		else
			newAlbedoName = diffuseName;

		len = sprintf(buffer, "\talbedo \"%s\"\n", newAlbedoName);
		file->write(buffer, len);

		if (normalName && normalName[0] != '\0')
		{
			len = sprintf(buffer, "\tnormalmap \"%s\"\n", normalName);
			file->write(buffer, len);
		}

		len = sprintf(buffer, "}\n");
		file->write(buffer, len);

		FileDevice::getInstance()->closeFile(file);
	}

	Material::Material()
	{
		m_shader = nullptr;
		m_albedoTexture = nullptr;
		m_normalTexture = nullptr;
		m_detailTexture = nullptr;
		m_depthWrite = true;
		m_selfillum = false;
	}

	Material::Material(const eastl::string& filename) : Content(filename)
	{
		//Material();

		m_shader = nullptr;
		m_albedoTexture = nullptr;
		m_normalTexture = nullptr;
		m_detailTexture = nullptr;
		m_depthWrite = true;
		m_selfillum = false;
	}

	Material::~Material()
	{
		//m_shader = nullptr;
		//m_diffuseTexture = nullptr;
		//m_normalTexture = nullptr;
		//m_detailTexture = nullptr;

		//mem_delete(*g_sysAllocator, m_detailTexture);
		//mem_delete(*g_sysAllocator, m_normalTexture);
		//mem_delete(*g_sysAllocator, m_albedoTexture);
	}

	void Material::load(const eastl::shared_ptr<DataStream>& dataStream)
	{
		dataStream->seek(FileSeek::End, 0);
		size_t length = dataStream->tell();
		dataStream->seek(FileSeek::Begin, 0);

		char* data = new char[length + 1];
		dataStream->read((void*)data, length);
		data[length] = '\0';

		const char* albedoTextureName = NULL;
		const char* normalTextureName = NULL;
		const char* detailTextureName = NULL;
		const char* shaderName = NULL;
		bool disableMipMapping = false;

		const char* tokenizerStr = "{},\"\n\r\t ";
		char* token = strtok(data, tokenizerStr);
		while (token)
		{
			if (strcmp(token, "material") == 0)
			{
				char* materialName = strtok(NULL, tokenizerStr);
				m_materialName = strdup(materialName);
				token = strtok(NULL, tokenizerStr);
				continue;
			}
			else if (strcmp(token, "shader") == 0)
			{
				char* shader = strtok(NULL, tokenizerStr);
				//m_shader = g_shaderSystem.CreateShader(technique);
				shaderName = strdup(shader);
				token = strtok(NULL, tokenizerStr);
				continue;
			}
			else if (strcmp(token, "albedo") == 0)
			{
				char* albedoName = 0;
				char* nextTextureTok = strtok(NULL, tokenizerStr);
				if (nextTextureTok && strcmp(nextTextureTok, "skipmips") == 0)
				{
					disableMipMapping = true;
					albedoName = strtok(NULL, tokenizerStr);
				}
				else
					albedoName = nextTextureTok;

				albedoTextureName = strdup(albedoName);
				token = strtok(NULL, tokenizerStr);
				continue;
			}
			else if (strcmp(token, "normalmap") == 0)
			{
				char* normalname = strtok(NULL, tokenizerStr);
				normalTextureName = strdup(normalname);
				token = strtok(NULL, tokenizerStr);
				continue;
			}
			else if (strcmp(token, "detail") == 0)
			{
				char* detailname = strtok(NULL, tokenizerStr);
				detailTextureName = strdup(detailname);
				token = strtok(NULL, tokenizerStr);
				continue;
			}
			else if (strcmp(token, "depth_write") == 0)
			{
				char* depthWriteValue = strtok(NULL, tokenizerStr);

				if (strcmp(depthWriteValue, "true") == 0)
					m_depthWrite = true;
				else if (strcmp(depthWriteValue, "false") == 0)
					m_depthWrite = false;

				token = strtok(NULL, tokenizerStr);
				continue;
			}
			else if (strcmp(token, "selfillum") == 0)
			{
				char* selfillumValue = strtok(NULL, tokenizerStr);

				if (strcmp(selfillumValue, "true") == 0)
					m_selfillum = true;
				else if (strcmp(selfillumValue, "false") == 0)
					m_selfillum = false;

				token = strtok(NULL, tokenizerStr);
				continue;
			}
			else
			{
				spdlog::error("Material::Load: get unknowed token '{}' while reading '{}' file.",
					token, m_filename.c_str());

				std::terminate();
			}

			token = strtok(NULL, tokenizerStr);
		}

		delete[] data;

		//TextureCreationDesc desc;
		//desc.m_mipmapping = !disableMipMapping;

		m_albedoTextureName += "data/textures/";
		m_albedoTextureName += albedoTextureName;

		if (normalTextureName)
		{
			m_normalTextureName += "data/textures/";
			m_normalTextureName += normalTextureName;
		}
			
		if (detailTextureName)
		{
			m_detailTextureName += "data/textures/";
			m_detailTextureName += detailTextureName;
		}
	}

	void Material::createHwTextures()
	{
		m_albedoTexture = ContentManager::getInstance()->loadTexture(m_albedoTextureName);

		if (!m_normalTextureName.empty())
			m_normalTexture = ContentManager::getInstance()->loadTexture(m_normalTextureName);

		if (!m_detailTextureName.empty())
			m_detailTexture = ContentManager::getInstance()->loadTexture(m_detailTextureName);
	}

	void Material::bind()
	{
		GraphicsDevice* device = GraphicsDevice::getInstance();
		device->depthTest(true);
		device->depthMask(m_depthWrite);

		assert(m_shader);
		m_shader->bind();

		// activate diffuse texture as 0
		device->setTexture2D(0, m_albedoTexture->getHWTexture());
		m_shader->setInteger("u_albedoTexture", 0);

		// Set up normal texture as 1
		if (m_normalTexture)
		{
			device->setTexture2D(1, m_normalTexture->getHWTexture());
			m_shader->setInteger("u_normalTexture", 1);
		}

		// Set up detail texture as 5
		if (m_detailTexture)
		{
			device->setTexture2D(5, m_detailTexture->getHWTexture());
			m_shader->setInteger("u_detailTexture", 5);
		}

		m_shader->setFloat("u_znear", CameraProxy::getInstance()->getView()->m_znear);
		m_shader->setFloat("u_zfar", CameraProxy::getInstance()->getView()->m_zfar);

		m_shader->setInteger("u_selfillum", m_selfillum);
	}

	TextureMap* Material::getTexture(MAT_TEX tex)
	{
		switch (tex)
		{
		case MAT_TEX_DIFFUSE: return m_albedoTexture.get();
		case MAT_TEX_NORMALMAP: return m_normalTexture.get();
		case MAT_TEX_DETAIL: return m_detailTexture.get();
		}

		return nullptr;
	}
}
