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
		Material(const std::string& filename);

		~Material();

		void load(const std::shared_ptr<DataStream>& dataStream) override;

		void parse(char* buf, int size);

	private:
		const char* parseTextureStage(char* buf);
	public:

		void createHwTextures();

		void bind();

		void resetAllStates();

		std::string& getName() { return m_materialName; }
		Shader* getShader() { return m_shader; }

		TextureMap* getTexture(MAT_TEX tex);

	private:
		Shader* m_shader;

		std::shared_ptr<TextureMap> m_albedoTexture;
		std::shared_ptr<TextureMap> m_normalTexture;
		std::shared_ptr<TextureMap> m_detailTexture;

		std::string m_materialName;
		std::string m_albedoTextureName;
		std::string m_normalTextureName;
		std::string m_detailTextureName;

		bool m_depthWrite;
		bool m_skipmips;
		bool m_selfillum;
		bool m_clampToEdge;
	};
}

#endif // !MATERIAL_H
