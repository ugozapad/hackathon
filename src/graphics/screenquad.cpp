#include "pch.h"
#include "graphics/screenquad.h"

#include "graphics/graphicsdevice.h"
#include "graphics/shader.h"

namespace engine
{
	GrVertexBuffer* ScreenQuad::ms_vertexBuffer;
	Shader* ScreenQuad::ms_screenQuadShader;

	void ScreenQuad::init()
	{
		float quadVertices[] = {
			// positions   // texCoords
			-1.0f,  1.0f,  0.0f, 1.0f,
			-1.0f, -1.0f,  0.0f, 0.0f,
			 1.0f, -1.0f,  1.0f, 0.0f,

			-1.0f,  1.0f,  0.0f, 1.0f,
			 1.0f, -1.0f,  1.0f, 0.0f,
			 1.0f,  1.0f,  1.0f, 1.0f
		};

		ms_vertexBuffer = GraphicsDevice::getInstance()->createVertexBuffer(quadVertices, sizeof(quadVertices), BufferAccess::Static);
		ms_screenQuadShader = mem_new<Shader>(*g_sysAllocator, "quad", "data/shaders/quad.vsh", "data/shaders/quad.fsh");

		VertexDeclaration position;
		position.name = "position";
		position.size = 2;
		position.type = VertexDeclaration::Float;
		ms_screenQuadShader->addVertexDeclaration(position);

		VertexDeclaration uv;
		uv.name = "uv";
		uv.size = 2;
		uv.type = VertexDeclaration::Float;
		ms_screenQuadShader->addVertexDeclaration(uv);
	}

	void ScreenQuad::shutdown()
	{
		mem_delete(*g_sysAllocator, ms_screenQuadShader);
		GraphicsDevice::getInstance()->deleteVertexBuffer(ms_vertexBuffer);
	}

	void ScreenQuad::render(GrTexture2D* texture)
	{
		assert(texture);

		GraphicsDevice::getInstance()->depthTest(false);

		GraphicsDevice::getInstance()->setVertexBuffer(ms_vertexBuffer);
		GraphicsDevice::getInstance()->setTexture2D(0, texture);

		ms_screenQuadShader->bind();
		//g_renderDevice->DrawArray(PM_TRIANGLES, 0, 6);
		GraphicsDevice::getInstance()->drawArray(PrimitiveMode::Triangles, 0, 6);
	}

	void ScreenQuad::render(GrTexture2D* texture, Shader* shader)
	{
		assert(texture);
		assert(shader);

		GraphicsDevice::getInstance()->depthTest(false);

		GraphicsDevice::getInstance()->setVertexBuffer(ms_vertexBuffer);
		GraphicsDevice::getInstance()->setTexture2D(0, texture);

		shader->bind();
		//g_renderDevice->DrawArray(PM_TRIANGLES, 0, 6);
	}

	void ScreenQuad::renderWithoutShaderBinding(GrTexture2D* texture)
	{
		assert(texture);

		GraphicsDevice::getInstance()->depthTest(false);

		GraphicsDevice::getInstance()->setVertexBuffer(ms_vertexBuffer);
		GraphicsDevice::getInstance()->setTexture2D(0, texture);

		//g_renderDevice->DrawArray(PM_TRIANGLES, 0, 6);
	}

	void ScreenQuad::renderWithoutTextureBinding(Shader* shader)
	{
		assert(shader);

		GraphicsDevice::getInstance()->depthTest(false);

		GraphicsDevice::getInstance()->setVertexBuffer(ms_vertexBuffer);

		shader->bind();
		//g_renderDevice->DrawArray(PM_TRIANGLES, 0, 6);
	}

	void ScreenQuad::renderWithoutShaderAndTextureBinding()
	{
		GraphicsDevice::getInstance()->depthTest(false);

		GraphicsDevice::getInstance()->setVertexBuffer(ms_vertexBuffer);

		//g_renderDevice->DrawArray(PM_TRIANGLES, 0, 6);
	}
}