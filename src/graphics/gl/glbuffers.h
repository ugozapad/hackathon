#ifndef RENDER_GL_BUFFERS_H
#define RENDER_GL_BUFFERS_H

#include "graphics/vertexbuffer.h"
#include "graphics/indexbuffer.h"
#include "glad/glad.h"

namespace engine
{
	GLenum getBufferAccess(BufferAccess access);
	GLenum getBufferMapping(BufferMapping mapping);

	class GLVertexBuffer : public GrVertexBuffer
	{
	public:
		GLVertexBuffer(void* data, size_t size, BufferAccess access);
		~GLVertexBuffer();

		void bind();

		void* map(BufferMapping mapping);
		void unmap();

	private:
		GLuint m_glbuffer;
	};

	class GLIndexBuffer : public GrIndexBuffer
	{
	public:
		GLIndexBuffer(unsigned int* data, size_t size, BufferAccess access);
		~GLIndexBuffer();

		void bind();

		void* map(BufferMapping mapping);
		void unmap();

	private:
		GLuint m_glbuffer;
	};
}

#endif // !RENDER_GL_BUFFERS_H
