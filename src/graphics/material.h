#ifndef MATERIAL_H
#define MATERIAL_H

#include "graphics/shader.h"

namespace engine
{
	enum MAT_TEX
	{
		MAT_TEX_DIFFUSE,
		MAT_TEX_NORMALMAP,
		MAT_TEX_DETAIL,
	};

	class TextureMap;

	class Material
	{
	public:
		static void CreateMaterialFromImport(const char* name, const char* diffuseName, const char* normalName);

	public:
		Material();
		~Material();

		void Load(const eastl::string& filename);
		void Bind();

		eastl::string& GetName() { return m_materialName; }
		Shader* GetShader() { return m_shader; }

		TextureMap* GetTexture(MAT_TEX tex);

	private:
		Shader* m_shader;

		TextureMap* m_albedoTexture;
		TextureMap* m_normalTexture;
		TextureMap* m_detailTexture;

		eastl::string m_materialName;

		bool m_depthWrite;
		bool m_selfillum;
	};
}

#endif // !MATERIAL_H
