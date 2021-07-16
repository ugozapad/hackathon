#ifndef SCREENQUAD_H
#define SCREENQUAD_H

namespace engine
{
	class GrVertexBuffer;
	class GrTexture2D;

	class Shader;

	class ScreenQuad
	{
	private:
		static GrVertexBuffer* ms_vertexBuffer;
		//static Shader* ms_screenQuadShader;

	public:
		static void init();
		static void shutdown();

		static void render(GrTexture2D* texture);

		// same as upper but with custom shader.
		static void render(GrTexture2D* texture, Shader* shader);

		// same as upper but without shader binding.
		static void renderWithoutShaderBinding(GrTexture2D* texture);

		// same as upper but without texture binding.
		static void renderWithoutTextureBinding(Shader* shader);

		// same as upper but without shader and texture binding. (just render screen quad)
		static void renderWithoutShaderAndTextureBinding();
	};

}

#endif // !SCREENQUAD_H
