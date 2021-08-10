#include "pch.h"
#include "gltextures.h"

// HACK HACK HACK
#include "graphics/graphicsoptions.h"

#define GL_COMPRESSED_RGB_S3TC_DXT1_EXT                   0x83F0
#define GL_COMPRESSED_RGBA_S3TC_DXT1_EXT                  0x83F1
#define GL_COMPRESSED_RGBA_S3TC_DXT3_EXT                  0x83F2
#define GL_COMPRESSED_RGBA_S3TC_DXT5_EXT                  0x83F3

namespace engine
{
	GLenum GetGLFormat(ImageFormat format)
	{
		switch (format)
		{

			//case ImageFormat::DEPTH24_STENCIL8: return GL_DEPTH24_STENCIL8;
			//case ImageFormat::DEPTH32_STENCIL_8_F: return GL_DEPTH32F_STENCIL8;

		case ImageFormat::RGB16: return GL_RGB16;
		case ImageFormat::RGBA16: return GL_RGBA16;
		case ImageFormat::RGB32: return GL_RGB;
		case ImageFormat::RGBA32: return GL_RGBA;

		case ImageFormat::RGB16F: return GL_RGB16F;
		case ImageFormat::RGBA16F: return GL_RGBA16F;
		case ImageFormat::RGB32F: return GL_RGB32F;
		case ImageFormat::RGBA32F: return GL_RGBA32F;

		case ImageFormat::DEPTH24: return GL_DEPTH_COMPONENT24;
		case ImageFormat::DEPTH32: return GL_DEPTH_COMPONENT32;
		case ImageFormat::DEPTH32F: return GL_DEPTH_COMPONENT32F;

			//case ImageFormat::SRGB: return GL_SRGB;
		}

		return 0;
	}

	GLenum GetGLInternalFormat(ImageFormat format)
	{
		switch (format)
		{

			//case ImageFormat::FMT_SRGB: return GL_RGB;

			//case ImageFormat::FMT_DEPTH24_STENCIL8: return GL_DEPTH24_STENCIL8;
			//case ImageFormat::FMT_DEPTH32_STENCIL_8_F: return GL_DEPTH32F_STENCIL8;

		case ImageFormat::RGB16:
		case ImageFormat::RGB32:
		case ImageFormat::RGB16F:
		case ImageFormat::RGB32F:
			return GL_RGB;

		case ImageFormat::RGBA16:
		case ImageFormat::RGBA32:
		case ImageFormat::RGBA32F:
		case ImageFormat::RGBA16F:
			return GL_RGBA;

		case ImageFormat::DEPTH24:
		case ImageFormat::DEPTH32:
		case ImageFormat::DEPTH32F:
			return GL_DEPTH_COMPONENT;
		}

		return 0;
	}

	GLenum GetGLTextureDataType(ImageFormat format)
	{
		switch (format)
		{
		case ImageFormat::RGB16:
		case ImageFormat::RGBA16:
		case ImageFormat::RGB32:
		case ImageFormat::RGBA32:
		case ImageFormat::DEPTH24:
		case ImageFormat::DEPTH32:
			return GL_UNSIGNED_BYTE;

		case ImageFormat::RGB16F:
		case ImageFormat::RGBA16F:
		case ImageFormat::RGB32F:
		case ImageFormat::RGBA32F:
		case ImageFormat::DEPTH32F:
			return GL_FLOAT;
		}

		return 0;
	}

	GLint GetComponentsFromFormat(ImageFormat format)
	{
		switch (format)
		{
		case ImageFormat::RGB16:
		case ImageFormat::RGB32:
		case ImageFormat::RGB16F:
		case ImageFormat::RGB32F:
			return 3;

		case ImageFormat::RGBA16:
		case ImageFormat::RGBA32:
		case ImageFormat::RGBA16F:
		case ImageFormat::RGBA32F:
			return 4;
		default:
			break;
		}

		return 0;
	}

	GLenum GetCompressionFormat(ImageFormat textureFormat, TextureCompressionFormat compressionFormat)
	{
		GLenum glFormat = 0;

		switch (compressionFormat)
		{
		case engine::TextureCompressionFormat::DXT1:
			if (textureFormat == ImageFormat::RGB32)
				glFormat = GL_COMPRESSED_RGB_S3TC_DXT1_EXT;
			else if (textureFormat == ImageFormat::RGBA32)
				glFormat = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;

			break;
		case engine::TextureCompressionFormat::DXT5:
			if (textureFormat == ImageFormat::RGBA32)
				glFormat = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
			break;
		default:
			break;
		}


		return glFormat;
	}

	//#define ENABLE_TEST_ANISOTROPIC_FILTERING

	GLTexture2D::GLTexture2D(const TextureCreationDesc& desc) :
		m_desc(desc),
		m_handle(0)
	{
		glGenTextures(1, &m_handle);
		glBindTexture(GL_TEXTURE_2D, m_handle);

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		if (m_desc.m_isCompressed)
		{
			unsigned int pixelDataSize = (m_desc.m_width * m_desc.m_height * GetComponentsFromFormat(m_desc.m_format)) >> 3;
			pixelDataSize = (pixelDataSize < 32) ? 32 : pixelDataSize;

			glCompressedTexImage2D(GL_TEXTURE_2D,
				0,
				GetCompressionFormat(m_desc.m_format, m_desc.m_compressionFormat),
				m_desc.m_width,
				m_desc.m_height,
				0,
				pixelDataSize,
				m_desc.m_data);

			//glCompressedTexImage2D(GL_TEXTURE_2D, 0, GetCompressionFormat(m_desc.m_format, m_desc.m_compressionFormat),
			//	m_desc.m_width, m_desc.m_height, 0, m_desc.m_size, m_desc.m_data);
		}
		else
		{
			glTexImage2D(GL_TEXTURE_2D,
				0,
				GetGLFormat(m_desc.m_format),
				m_desc.m_width, m_desc.m_height,
				0,
				GetGLInternalFormat(m_desc.m_format),
				//GL_UNSIGNED_BYTE, 
				GetGLTextureDataType(m_desc.m_format),
				m_desc.m_data);
		}

		if (m_desc.m_mipmapping)
		{
			//if (!m_desc.m_isCompressed)
				//glGenerateMipmap(GL_TEXTURE_2D);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			// FUCK OFF
			//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

#define ENABLE_TEST_ANISOTROPIC_FILTERING

#ifdef ENABLE_TEST_ANISOTROPIC_FILTERING

#			define GL_TEXTURE_MAX_ANISOTROPY_EXT 0x84FE
#			define GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT 0x84FF

			//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 16);

			// HACK HACK HACK
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, g_graphicsOptions.m_anisotropicQuality);
#endif // ENABLE_TEST_ANISOTROPIC_FILTERING

		}
		else
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		}

		glBindTexture(GL_TEXTURE_2D, 0);
	}

	GLTexture2D::~GLTexture2D()
	{
		release();
	}

	void GLTexture2D::release()
	{
		if (m_handle != 0)
			glDeleteTextures(1, &m_handle);
	}

}
