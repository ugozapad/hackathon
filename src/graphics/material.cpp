#include "pch.h"
#include "graphics/material.h"
#include "graphics/texturemap.h"
#include "graphics/shader.h"
#include "graphics/shadermanager.h"
#include "graphics/graphicsdevice.h"
#include "graphics/rendercontext.h"
#include "graphics/shaderconstantmanager.h"

#include "common/parse.h"

#include "file/filedevice.h"

#include "engine/camera.h"
#include "engine/content/contentmanager.h"

#include <sstream>

namespace engine
{
	Shader* getEngineShaderFromMaterialName(const std::string& shaderName)
	{
		if (shaderName == "diffuse")
			return ShaderManager::getInstance()->createShader("def_geom");
		else if (shaderName == "skybox")
			return ShaderManager::getInstance()->createShader("forw_geom");

		return nullptr;
	}

	void Material::createMaterialFromImport(const char* name, const char* diffuseName, const char* normalName)
	{
		std::string materiaName;
		materiaName += "materials/";
		materiaName += name;
		materiaName += ".material";

		File* file = FileDevice::instance()->openFile(materiaName, FileAccess::Write);

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

		FileDevice::instance()->closeFile(file);
	}

	Material::Material()
	{
		m_shader = nullptr;
		m_albedoTexture = nullptr;
		m_normalTexture = nullptr;
		m_detailTexture = nullptr;
		m_depthWrite = true;
		m_selfillum = false;
		m_clampToEdge = false;
	}

	Material::Material(const std::string& filename) : Content(filename)
	{
		//Material();

		m_shader = nullptr;
		m_albedoTexture = nullptr;
		m_normalTexture = nullptr;
		m_detailTexture = nullptr;
		m_depthWrite = true;
		m_selfillum = false;
		m_clampToEdge = false;
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

	void Material::load(const std::shared_ptr<DataStream>& dataStream)
	{
		dataStream->seek(FileSeek::End, 0);
		size_t length = dataStream->tell();
		dataStream->seek(FileSeek::Begin, 0);

		char* data = new char[length + 1];
		dataStream->read((void*)data, length);
		data[length] = '\0';

		parse(data, length);

		delete[] data;
	}

#if 0
		const char* albedoTextureName = NULL;
		const char* normalTextureName = NULL;
		const char* detailTextureName = NULL;
		const char* shaderName = NULL;
		bool disableMipMapping = false;

		const char* tokenizerStr = ",\"\n\r\t ";
		char* token = strtok(buf, tokenizerStr);

		bool hasMaterialOnBegin = (strcmp(token, "material") == 0);
		if (!hasMaterialOnBegin)
			Core::error("Material::parse: no \"material\" key in material %s", m_filename.c_str());

		char* materialName = strtok(NULL, tokenizerStr);
		m_materialName = strdup(materialName);
		token = strtok(NULL, tokenizerStr);

		while (1)
		{
			token = strtok(NULL, tokenizerStr);

			if (!token)
				break;

			if (!token[0])
			{
				Core::error("Material::parse: no concluding '}' in material %s\n", m_filename.c_str());
			}

			if (token[0] == '{' /*|| token[0] == '}'*/)
				continue;

			if (strcmp(token, "shader") == 0)
			{
				char* shader = strtok(NULL, tokenizerStr);
				m_shader = getEngineShaderFromMaterialName(shader);
				shaderName = strdup(shader);
				continue;
			}
			else if (strcmp(token, "albedo") == 0)
			{
				char* albedoName = 0;
				char* nextTextureTok = strtok(NULL, tokenizerStr);

				if (nextTextureTok[0] == '{')
				{
					nextTextureTok = strtok(NULL, tokenizerStr);

					if (nextTextureTok && strcmp(nextTextureTok, "skipmips") == 0)
					{
						disableMipMapping = true;
						albedoName = strtok(NULL, tokenizerStr);
					}
					else if (nextTextureTok && strcmp(nextTextureTok, "clampedge") == 0)
					{
						m_clampToEdge = true;
						albedoName = strtok(NULL, tokenizerStr);
					}
					else
					{
						nextTextureTok = strtok(NULL, tokenizerStr);
					}
				}
				else // using albedo texture filename without any parameter
				{
					albedoTextureName = strdup(nextTextureTok);
				}

				if (!albedoTextureName)
					albedoTextureName = strdup(albedoName);

				continue;
			}
			else if (strcmp(token, "normalmap") == 0)
			{
				char* normalname = strtok(NULL, tokenizerStr);
				normalTextureName = strdup(normalname);
				continue;
			}
			else if (strcmp(token, "detail") == 0)
			{
				char* detailname = strtok(NULL, tokenizerStr);
				detailTextureName = strdup(detailname);
				continue;
			}
			else if (strcmp(token, "depth_write") == 0)
			{
				char* depthWriteValue = strtok(NULL, tokenizerStr);

				if (strcmp(depthWriteValue, "true") == 0)
					m_depthWrite = true;
				else if (strcmp(depthWriteValue, "false") == 0)
					m_depthWrite = false;

				continue;
			}
			else if (strcmp(token, "skipmips") == 0)
			{
				char* skipmipsValue = strtok(NULL, tokenizerStr);
				if (strcmp(skipmipsValue, "true") == 0)
					m_skipmips = true;
				else if (strcmp(skipmipsValue, "false") == 0)
					m_skipmips = false;

				continue;
			}
			else if (strcmp(token, "selfillum") == 0)
			{
				char* selfillumValue = strtok(NULL, tokenizerStr);

				if (strcmp(selfillumValue, "true") == 0)
					m_selfillum = true;
				else if (strcmp(selfillumValue, "false") == 0)
					m_selfillum = false;

				continue;
			}
			else
			{
				Core::error("Material::parse: unknown material parameter '%s' in '%s'\n", token, m_filename.c_str());
			}
		}

		//TextureCreationDesc desc;
		//desc.m_mipmapping = !disableMipMapping;

		m_albedoTextureName += "textures/";
		m_albedoTextureName += albedoTextureName;

		if (normalTextureName)
		{
			m_normalTextureName += "textures/";
			m_normalTextureName += normalTextureName;
		}

		if (detailTextureName)
		{
			m_detailTextureName += "textures/";
			m_detailTextureName += detailTextureName;
		}
#endif
	const char* tokenizerStr = "{},\"\n\r\t ";

	void Material::parse(char* buf, int size)
	{
		const char* albedoTextureName = NULL;
		const char* normalTextureName = NULL;
		const char* detailTextureName = NULL;
		const char* shaderName = NULL;
		bool disableMipMapping = false;


		char* token = strtok(buf, tokenizerStr);
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
				m_shader = getEngineShaderFromMaterialName(shader);
				shaderName = strdup(shader);
				token = strtok(NULL, tokenizerStr);
				continue;
			}
			else if (strcmp(token, "albedo") == 0)
			{
				//albedoTextureName = strdup(albedoName);
				albedoTextureName = parseTextureStage(token);
				token = strtok(NULL, tokenizerStr);
				continue;
			}
			else if (strcmp(token, "normalmap") == 0)
			{
				//char* normalname = strtok(NULL, tokenizerStr);
				//normalTextureName = strdup(normalname);
				normalTextureName = parseTextureStage(token);
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
			else if (strcmp(token, "skipmips") == 0)
			{
				char* skipmipsValue = strtok(NULL, tokenizerStr);
				if (strcmp(skipmipsValue, "true") == 0)
					m_skipmips = true;
				else if (strcmp(skipmipsValue, "false") == 0)
					m_skipmips = false;

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
				Core::error("Material::Load: get unknowed token '%s' while reading '%s' file.",
					token, m_filename.c_str());
			}

			token = strtok(NULL, tokenizerStr);
		}

		//TextureCreationDesc desc;
		//desc.m_mipmapping = !disableMipMapping;

		m_albedoTextureName += "textures/";
		m_albedoTextureName += albedoTextureName;

		if (normalTextureName)
		{
			m_normalTextureName += "textures/";
			m_normalTextureName += normalTextureName;
		}

		if (detailTextureName)
		{
			m_detailTextureName += "textures/";
			m_detailTextureName += detailTextureName;
		}
	}

	const char* Material::parseTextureStage(char* buf)
	{
		char* albedoName = 0;
		char* nextTextureTok = strtok(NULL, tokenizerStr);
		if (nextTextureTok && strcmp(nextTextureTok, "skipmips") == 0)
		{
			m_skipmips = true;
			albedoName = strtok(NULL, tokenizerStr);
		}
		else if (nextTextureTok && strcmp(nextTextureTok, "clampedge") == 0)
		{
			m_clampToEdge = true;
			albedoName = strtok(NULL, tokenizerStr);
		}
		else
		{
			albedoName = nextTextureTok;
		}

		return strdup(albedoName);
	}

	void Material::createHwTextures()
	{
		ContentManager* contentManager = ContentManager::getInstance();

		m_albedoTexture = contentManager->loadTexture(m_albedoTextureName);

		if (m_albedoTexture && !m_skipmips)
		{
			GraphicsDevice::instance()->setTexture2D(0, m_albedoTexture->getHWTexture());
			m_albedoTexture->gen_mipmaps();
			GraphicsDevice::instance()->setTexture2D(0, 0);
		}
		
		if (!m_normalTextureName.empty())
			m_normalTexture = contentManager->loadTexture(m_normalTextureName);

		if (!m_detailTextureName.empty())
			m_detailTexture = contentManager->loadTexture(m_detailTextureName);
	}

	void Material::bind()
	{
		GraphicsDevice* device = GraphicsDevice::getInstance();

		// reset all
		device->depthMask(false);
		device->depthTest(true);

		device->depthTest(true);

		if (!m_depthWrite)
			device->depthTest(false);

		device->depthMask(m_depthWrite);

		assert(m_shader);
		m_shader->bind();

		// activate diffuse texture as 0
		device->setTexture2D(0, m_albedoTexture->getHWTexture());

		// if clamp to edge is enable
		if (m_clampToEdge)
		{
			m_albedoTexture->setWrapS(TextureWrap::ClampToEdge);
			m_albedoTexture->setWrapT(TextureWrap::ClampToEdge);
		}
		else
		{
			m_albedoTexture->setWrapS(TextureWrap::Repeat);
			m_albedoTexture->setWrapT(TextureWrap::Repeat);
		}

		if (m_skipmips)
		{
			m_albedoTexture->setMin(TextureFilter::Linear);
			m_albedoTexture->setMag(TextureFilter::Linear);
		}
		else
		{
			m_albedoTexture->setMin(TextureFilter::LinearMipmapLinear);
			m_albedoTexture->setMag(TextureFilter::Linear);
		}

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

		// Set default shader constants
		ShaderConstantManager::getInstance()->setDefaultContants(m_shader);

#if 0
		// apply constants here !!!
		RenderContext& renderContext = RenderContext::getContext();
		m_shader->setMatrix("u_model", renderContext.model);
		m_shader->setMatrix("u_view", renderContext.view);
		m_shader->setMatrix("u_proj", renderContext.proj);

		// compute mvp matrix
		glm::mat4 mvp = glm::mat4(1.0f);
		mvp = renderContext.proj * renderContext.view * renderContext.model;

		m_shader->setMatrix("u_mvp", mvp);
		m_shader->setMatrix("u_modelViewProjection", mvp);
#endif
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
