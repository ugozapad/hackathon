#include "pch.h"
#include "graphics/texturemap.h"

#include "graphics/graphicsdevice.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace engine
{
	TextureMap::TextureMap(const eastl::string& filename)
	{
		TextureCreationDesc desc = {0};
		desc.m_mipmapping = true;
		loadTexture(filename, desc);
	}

	TextureMap::TextureMap(const eastl::string& filename, const TextureCreationDesc& desc)
	{
		loadTexture(filename, desc);
	}

	TextureMap::TextureMap()
	{

	}

	TextureMap::~TextureMap()
	{
		release();
	}

	void TextureMap::loadTexture(const eastl::string& filename, const TextureCreationDesc& desc)
	{
		stbi_set_flip_vertically_on_load(true);

		TextureCreationDesc texdesc;
		int width, height, channels;
		unsigned char* data = stbi_load(filename.c_str(), &width, &height, &channels, 0);

		if (width % 2 != 0 || height % 2 != 0)
		{
			spdlog::warn("image has non power of two size {}x{} {}", width, height, filename.c_str());
			//Msg("using EXT_texture_rectangle");

			//texdesc.m_isNPOT = true;
		}

		texdesc.m_width = width;
		texdesc.m_height = height;
		texdesc.m_format = (channels == 4) ? ImageFormat::RGBA32 : ImageFormat::RGB32;
		texdesc.m_data = data;
		texdesc.m_mipmapping = desc.m_mipmapping;
		m_texture2D = GraphicsDevice::getInstance()->createTexture2D(texdesc);

		stbi_image_free(data);
	}

	void TextureMap::release()
	{
		if (m_texture2D) {
			m_texture2D->release();
			GraphicsDevice::getInstance()->deleteTexture2D(m_texture2D);
		}
	}

	void TextureMap::bind()
	{
		throw std::logic_error("The method or operation is not implemented.");
	}

	GLuint TextureMap::getHandle()
	{
		throw std::logic_error("The method or operation is not implemented.");
	}

}
