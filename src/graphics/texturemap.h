#ifndef TEXTUREMAP_H
#define TEXTUREMAP_H

#include "engine/content/content.h"

#include "graphics/texture.h"

namespace engine
{
	class TextureMap : public Content//,
					  /* public eastl::enable_shared_from_this<TextureMap>*/
	{
	public:
		TextureMap();
		TextureMap(const eastl::string& filename);
		TextureMap(const eastl::string& filename, const TextureCreationDesc& desc);

		~TextureMap();

		void loadTexture(const eastl::string& filename, const TextureCreationDesc& desc);

		void release();
		GrTexture2D* getHWTexture() { return m_texture2D; }

		void bind();
		uint32_t getHandle();


		void load(const eastl::shared_ptr<DataStream>& dataStream) override;

	private:
		GrTexture2D* m_texture2D;
		const char* m_fileName;
	};
}

#endif // TEXTUREMAP_H
