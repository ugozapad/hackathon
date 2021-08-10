#include "pch.h"

#include "graphics/gl/glad/glad.h"

#include "graphics/texturemap.h"

#include "graphics/graphicsdevice.h"

#include "graphics/image.h"
#include "graphics/nv_dds.h"

namespace engine
{
	TextureMap::TextureMap(const std::string& filename) :
		Content(filename)
	{
		memset(&m_texdesc, 0, sizeof(m_texdesc));
	}

	TextureMap::TextureMap()
	{
		memset(&m_texdesc, 0, sizeof(m_texdesc));
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
		Image::freeImageData(m_texdesc.m_data);
		m_texdesc.m_data = nullptr;
	}

	void TextureMap::load(const std::shared_ptr<DataStream>& dataStream)
	{
		size_t extensionLocation = m_filename.find('.');
		std::string extension = m_filename.substr(extensionLocation + 1);

		if (extension != "dds")
		{
			Image image;
			image.setFlip(true);
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

	GLint getGlWrap(TextureWrap wrap)
	{
		GLint param = 0;

		if (wrap == TextureWrap::Repeat)
			param = GL_REPEAT;
		else if (wrap == TextureWrap::MirroredRepeat)
			param = GL_MIRRORED_REPEAT;
		else if (wrap == TextureWrap::ClampToEdge)
			param = GL_CLAMP_TO_EDGE;
		else if (wrap == TextureWrap::ClampToBorder)
			param = GL_CLAMP_TO_BORDER;

		return param;
	}

	void TextureMap::setWrapS(TextureWrap wrap)
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, getGlWrap(wrap));
	}

	void TextureMap::setWrapT(TextureWrap wrap)
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, getGlWrap(wrap));
	}


	GLint getGlTexFilter(TextureFilter filter)
	{
		GLint param = 0;

		if (filter == TextureFilter::Linear)
			param = GL_LINEAR;
		else if (filter == TextureFilter::Nearest)
			param = GL_NEAREST;
		else if (filter == TextureFilter::LinearMipmapLinear)
			param = GL_LINEAR_MIPMAP_LINEAR;
		else if (filter == TextureFilter::LinearMipmapNearest)
			param = GL_LINEAR_MIPMAP_NEAREST;
		else if (filter == TextureFilter::NearestMipmapLinear)
			param = GL_NEAREST_MIPMAP_LINEAR;
		else if (filter == TextureFilter::NearestMipmapNearest)
			param = GL_NEAREST_MIPMAP_NEAREST;	

		return param;
	}

	void TextureMap::setMin(TextureFilter filter)
	{
		GLint param = 0;
		param = getGlTexFilter(filter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, param);
	}

	void TextureMap::setMag(TextureFilter filter)
	{
		GLint param = 0;
		param = getGlTexFilter(filter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, param);
	}

	void TextureMap::gen_mipmaps()
	{
		if (!m_texdesc.m_isCompressed)
		{
			if (!m_texdesc.m_mipmapping)
				m_texdesc.m_mipmapping = true;

			glGenerateMipmap(GL_TEXTURE_2D);
		}
	}

}
