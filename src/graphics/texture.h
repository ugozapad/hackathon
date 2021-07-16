#ifndef ITEXTURE_H
#define ITEXTURE_H

#include "graphics/gfxcommon.h"

namespace engine
{

	struct TextureCreationDesc
	{
		void* m_data;
		ImageFormat m_format;
		int m_width;
		int m_height;
		bool m_mipmapping;
		bool m_isCompressed; // DDS Support
	};

	class GrTextureBase
	{
	public:
		virtual ~GrTextureBase() {}

		virtual void Release() = 0;
		virtual unsigned int GetHandle() = 0;
	};

	class GrTexture2D : public GrTextureBase
	{
	};

}

#endif // !ITEXTURE_H
