#include "pch.h"
#include "graphics/texturemap.h"

#include "graphics/graphicsdevice.h"

#include "graphics/image.h"
#include "graphics/nv_dds.h"

namespace engine
{
	TextureMap::TextureMap(const eastl::string& filename) :
		Content(filename)
	{
		memset(&m_texdesc, 0, sizeof(m_texdesc));
	}

	TextureMap::TextureMap()
	{

	}

	TextureMap::~TextureMap()
	{
		release();
	}

	void TextureMap::release()
	{
		if (m_texture2D) {
			m_texture2D->release();
			GraphicsDevice::getInstance()->deleteTexture2D(m_texture2D);
			m_texture2D = nullptr;
		}

		if (m_texdesc.m_data)
			Image::freeImageData(m_texdesc.m_data);
	}

	void TextureMap::bind()
	{
		throw std::logic_error("The method or operation is not implemented.");
	}

	GLuint TextureMap::getHandle()
	{
		throw std::logic_error("The method or operation is not implemented.");
	}

	void TextureMap::createHWTexture()
	{
		m_texture2D = GraphicsDevice::getInstance()->createTexture2D(m_texdesc);
	}

	void TextureMap::load(const eastl::shared_ptr<DataStream>& dataStream)
	{
		size_t extensionLocation = m_filename.find('.');
		eastl::string extension = m_filename.substr(extensionLocation + 1);

		if (extension != "dds")
		{
			Image image;
			image.createFromFile(dataStream);

			m_texdesc.m_width = image.getWidth();
			m_texdesc.m_height = image.getHeight();
			m_texdesc.m_format = (image.getChannels() == 4) ? ImageFormat::RGBA32 : ImageFormat::RGB32;
			m_texdesc.m_data = image.getData();
			m_texdesc.m_mipmapping = true;
		}
		else
		{
#define GL_COMPRESSED_RGB_S3TC_DXT1_EXT                   0x83F0
#define GL_COMPRESSED_RGBA_S3TC_DXT1_EXT                  0x83F1
#define GL_COMPRESSED_RGBA_S3TC_DXT3_EXT                  0x83F2
#define GL_COMPRESSED_RGBA_S3TC_DXT5_EXT                  0x83F3

			using namespace nv_dds;

			CDDSImage image;
			image.load(dataStream);

			m_texdesc.m_width = image.get_width();
			m_texdesc.m_height = image.get_height();
			m_texdesc.m_format = (image.get_components() == 4) ? ImageFormat::RGBA32 : ImageFormat::RGB32;
			m_texdesc.m_data = image;
			m_texdesc.m_mipmapping = true;
			m_texdesc.m_isCompressed = true;
			m_texdesc.m_size = image.get_size();

			if (image.get_format() == GL_COMPRESSED_RGBA_S3TC_DXT3_EXT)
				std::terminate();

			m_texdesc.m_compressionFormat = (image.get_format() == GL_COMPRESSED_RGB_S3TC_DXT1_EXT) ? TextureCompressionFormat::DXT1 : TextureCompressionFormat::DXT5;
		}
	}
}
