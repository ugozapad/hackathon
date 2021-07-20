#ifndef TEXTUREMAP_H
#define TEXTUREMAP_H

#include "engine/content/content.h"

#include "graphics/texture.h"

namespace engine
{
	class TextureMap : public Content
	{
	public:
		TextureMap();
		TextureMap(const eastl::string& filename);

		~TextureMap();
		
		void load(const eastl::shared_ptr<DataStream>& dataStream) override;
	
		void release();
		GrTexture2D* getHWTexture() { return m_texture2D; }

		void bind();
		uint32_t getHandle();

		void createHWTexture();

	private:
		GrTexture2D* m_texture2D;
		TextureCreationDesc m_texdesc;
	};
}

#endif // TEXTUREMAP_H
