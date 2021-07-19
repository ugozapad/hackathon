#ifndef MATERIAL_H
#define MATERIAL_H

#include "engine/content/content.h"
#include "graphics/shader.h"
#include "graphics/texturemap.h"

namespace engine
{
	enum MAT_TEX
	{
		MAT_TEX_DIFFUSE,
		MAT_TEX_NORMALMAP,
		MAT_TEX_DETAIL,
	};

	class TextureMap;

	class Material : public Content
	{
	public:
		static void createMaterialFromImport(const char* name, const char* diffuseName, const char* normalName);

	public:
		Material();
		Material(const eastl::string& filename);

		~Material();

		void load(const eastl::shared_ptr<DataStream>& dataStream) override;

		void createHwTextures();

		void bind();

		eastl::string& getName() { return m_materialName; }
		Shader* getShader() { return m_shader; }

		TextureMap* getTexture(MAT_TEX tex);

	private:
		Shader* m_shader;

		eastl::shared_ptr<TextureMap> m_albedoTexture;
		eastl::shared_ptr<TextureMap> m_normalTexture;
		eastl::shared_ptr<TextureMap> m_detailTexture;

		eastl::string m_materialName;
		eastl::string m_albedoTextureName;
		eastl::string m_normalTextureName;
		eastl::string m_detailTextureName;

		bool m_depthWrite;
		bool m_selfillum;
	};
}

#endif // !MATERIAL_H
