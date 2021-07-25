#ifndef ITEXTURE_H
#define ITEXTURE_H

#include "graphics/gfxcommon.h"

namespace engine
{
	enum class TextureCompressionFormat
	{
		DXT1,
		DXT5,
	};

	struct TextureCreationDesc
	{
		void* m_data;
		ImageFormat m_format;
		int m_width;
		int m_height;
		bool m_mipmapping;

		// DDS Support
		bool m_isCompressed;
		TextureCompressionFormat m_compressionFormat;
		int m_size;
	};

	class GrTextureBase
	{
	public:
		virtual ~GrTextureBase() {}

		virtual void release() = 0;
		virtual unsigned int getHandle() = 0;

	};

	class GrTexture2D : public GrTextureBase
	{
	};

}

#endif // !ITEXTURE_H
