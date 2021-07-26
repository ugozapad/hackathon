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
		TextureMap(const std::string& filename);

		~TextureMap();
		
		void load(const std::shared_ptr<DataStream>& dataStream) override;
	
		void release();
		GrTexture2D* getHWTexture() { return m_texture2D; }

		void bind();
		uint32_t getHandle();

		void createHWTexture();

		// #HACK
		void setWrapS(TextureWrap wrap);
		void setWrapT(TextureWrap wrap);
		void setMin(TextureFilter filter);
		void setMag(TextureFilter filter);

		void gen_mipmaps();

	private:
		GrTexture2D* m_texture2D;
		TextureCreationDesc m_texdesc;
	};
}

#endif // TEXTUREMAP_H
