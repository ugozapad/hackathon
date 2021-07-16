#ifndef TEXTUREMAP_H
#define TEXTUREMAP_H

#include "graphics/texture.h"

namespace engine
{
	class TextureMap
	{
	public:
		TextureMap(const eastl::string& filename);
		TextureMap(const eastl::string& filename, const TextureCreationDesc& desc);

		~TextureMap();

		void loadTexture(const eastl::string& filename, const TextureCreationDesc& desc);

		void release();
		GrTexture2D* getHWTexture() { return m_texture2D; }

		void bind();
		uint32_t getHandle();

	private:
		GrTexture2D* m_texture2D;
		const char* m_fileName;
	};
}

#endif // TEXTUREMAP_H
