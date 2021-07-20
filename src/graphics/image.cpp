#include "pch.h"
#include "graphics/image.h"

#include "file/filestream.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include "stb_image_resize.h"

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

	static void stbi_write_callback(void* context, void* data, int size)
	{
		DataStream* stream = reinterpret_cast<DataStream*>(context);
		return stream->write(data, size);
	}

	void Image::freeImageData(void* data)
	{
		stbi_image_free(data);
	}

	Image::Image()
	{
		m_data = nullptr;
		m_width = 0;
		m_height = 0;
		m_channels = 0;
		ImageFormat m_format = (ImageFormat)0;
	}

	Image::~Image()
	{

	}

	void Image::free()
	{
		if (m_data)
			Image::freeImageData(m_data);
	}

	void Image::createFromFile(const eastl::string& filename)
	{

	}

	void Image::createFromFile(const eastl::shared_ptr<DataStream>& stream)
	{
		stbi_set_flip_vertically_on_load(true);
		m_data = stbi_load_from_callbacks(&g_stdio_callbacks, reinterpret_cast<void*>(stream.get()), &m_width, &m_height, &m_channels, 0);
	}

	void Image::createRaw(void* data, int width, int height, int channels)
	{
		m_data = data;
		m_width = width;
		m_height = height;
		m_channels = channels;
	}

	void Image::save(const eastl::string& filename)
	{
		eastl::shared_ptr<FileStream> stream = eastl::make_shared<FileStream>(filename, "w");
		stbi_flip_vertically_on_write(m_flip);

		size_t extPos = filename.find('.');
		eastl::string ext = filename.substr(extPos + 1);

		if (ext == "png")
			stbi_write_png_to_func(stbi_write_callback, (void*)stream.get(), m_width, m_height, m_channels, m_data, m_width * m_channels);
		else if (ext == "tga")
			stbi_write_tga_to_func(stbi_write_callback, (void*)stream.get(), m_width, m_height, m_channels, m_data);
		else if (ext == "jpg")
			stbi_write_jpg_to_func(stbi_write_callback, (void*)stream.get(), m_width, m_height, m_channels, m_data, 90);
		else
			throw std::logic_error("Image::save: unknowed format to save!");
	}

	void Image::save(const eastl::shared_ptr<DataStream>& stream)
	{

	}

	int Image::getWidth()
	{
		return m_width;
	}

	int Image::getHeight()
	{
		return m_height;
	}

	int Image::getChannels()
	{
		return m_channels;
	}

	ImageFormat Image::getImageFormat()
	{
		return m_format;
	}

	void* Image::getData()
	{
		return m_data;
	}

}