#include "pch.h"
#include "graphics/graphicsdevice.h"

namespace engine
{
	GraphicsDevice* GraphicsDevice::instance()
	{
		return Singleton<GraphicsDevice>::getInstance();
	}

	void GraphicsDevice::init(GLFWwindow* window)
	{

	}

	void GraphicsDevice::shutdown()
	{

	}

	void GraphicsDevice::clearColor(float r, float g, float b, float a)
	{

	}

	void GraphicsDevice::clear(uint32_t flags)
	{

	}

	void GraphicsDevice::swapBuffers()
	{

	}

	void GraphicsDevice::drawArray(PrimitiveMode mode, size_t startOf, size_t count)
	{

	}

	void GraphicsDevice::drawElements(PrimitiveMode mode, size_t count)
	{

	}

	void GraphicsDevice::depthTest(bool value)
	{

	}

	void GraphicsDevice::depthMask(bool value)
	{

	}

	GrVertexBuffer* GraphicsDevice::createVertexBuffer(void* data, size_t size, BufferAccess access)
	{
		return nullptr;
	}

	GrVertexBuffer* GraphicsDevice::createVertexBuffer(const BufferCreationDesc& desc)
	{
		return nullptr;
	}

	void GraphicsDevice::deleteVertexBuffer(GrVertexBuffer* buffer)
	{

	}

	GrIndexBuffer* GraphicsDevice::createIndexBuffer(unsigned int* data, size_t size, BufferAccess access)
	{
		return nullptr;
	}

	GrIndexBuffer* GraphicsDevice::createIndexBuffer(const BufferCreationDesc& desc)
	{
		return nullptr;
	}

	void GraphicsDevice::deleteIndexBuffer(GrIndexBuffer* buffer)
	{

	}

	GrTexture2D* GraphicsDevice::createTexture2D(TextureCreationDesc& desc)
	{
		return nullptr;
	}

	void GraphicsDevice::deleteTexture2D(GrTexture2D* texture)
	{

	}

	GrFramebuffer* GraphicsDevice::createFramebuffer(int width, int height)
	{
		return nullptr;
	}

	void GraphicsDevice::deleteFramebuffer(GrFramebuffer* framebuffer)
	{

	}

	void GraphicsDevice::setVertexBuffer(GrVertexBuffer* buffer)
	{

	}

	void GraphicsDevice::setIndexBuffer(GrIndexBuffer* buffer)
	{

	}

	void GraphicsDevice::setTexture2D(int slot, GrTexture2D* texture)
	{

	}

	void GraphicsDevice::setFramebuffer(GrFramebuffer* framebuffer)
	{

	}

	void GraphicsDevice::setViewport(Viewport* viewport)
	{

	}
}