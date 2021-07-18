#include "pch.h"
#include "graphics/texturemap.h"

#include "graphics/graphicsdevice.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace engine
{
	static int stb_eof(void* user)
	{
		using namespace engine;

		DataStream* stream = reinterpret_cast<DataStream*>(user);
		return stream->eof();
	}

	static int stb_read(void* user, char* data, int size)
	{
		using namespace engine;

		if (size < 0)
			size = 0;

		//std::istream* stream = reinterpret_cast<std::istream*>(user);
		DataStream* stream = reinterpret_cast<DataStream*>(user);
		char* p = data;
		int totalBytesRead = 0;
		do
		{
			if (stream->eof())
				break;

			int beginreadsize = stream->tell();
			stream->read(p, static_cast<size_t>(size));
			int endreadsize = stream->tell();

			//int bytesRead = static_cast<int>(stream->gcount());
			int bytesRead = static_cast<int>(endreadsize - beginreadsize);

			totalBytesRead += bytesRead;
			p += bytesRead;
			size -= bytesRead;
		} while (size > 0);

		return totalBytesRead;
	}

	static void stb_skip(void* user, int n)
	{
		using namespace engine;

		DataStream* stream = reinterpret_cast<DataStream*>(user);
		stream->seek(FileSeek::Current, n);
	}

	static stbi_io_callbacks g_stdio_callbacks = { stb_read, stb_skip, stb_eof };

	TextureMap::TextureMap(const eastl::string& filename)
	{
		TextureCreationDesc desc = { 0 };
		desc.m_mipmapping = true;
		loadTexture(filename, desc);
	}

	TextureMap::TextureMap(const eastl::string& filename, const TextureCreationDesc& desc)
	{
		loadTexture(filename, desc);
	}

	TextureMap::TextureMap()
	{
		memset(&m_texdesc, 0, sizeof(m_texdesc));
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

		if (m_data)
			stbi_image_free(m_data);
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
		stbi_set_flip_vertically_on_load(true);

		int width, height, channels;
		m_data = stbi_load_from_callbacks(&g_stdio_callbacks, reinterpret_cast<void*>(dataStream.get()), &width, &height, &channels, 0);

		m_texdesc.m_width = width;
		m_texdesc.m_height = height;
		m_texdesc.m_format = (channels == 4) ? ImageFormat::RGBA32 : ImageFormat::RGB32;
		m_texdesc.m_data = m_data;
		m_texdesc.m_mipmapping = true;
	}

}
