#ifndef IMAGE_H
#define IMAGE_H

#include "file/datastream.h"

#include "graphics/gfxcommon.h"
//#include "graphics/texture.h"

namespace engine
{
	// wrapper on image loading and other stuff
	class Image
	{
	public:
		static void freeImageData(void* data);

	public:
		Image();
		~Image();

		void free();

		void createFromFile(const eastl::string& filename);
		void createFromFile(const eastl::shared_ptr<DataStream>& stream);

		// create from existed data
		void createRaw(void* data, int width, int height, int channels);

		// saving
		void save(const eastl::string& filename);
		void save(const eastl::shared_ptr<DataStream>& stream);

		int getWidth();
		int getHeight();
		int getChannels();
		ImageFormat getImageFormat();

		void* getData();

		void setFlip(bool flip) { m_flip = flip; }
		bool getFlip() { return m_flip; }

		// image operations
		void resize(int width, int height);

	private:
		void* m_data;
		int m_width, m_height, m_channels;
		ImageFormat m_format;

		bool m_flip;
	};
}

#endif // !IMAGE_H
