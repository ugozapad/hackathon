#include "pch.h"
#include "glbuffers.h"


namespace engine
{
	GLVertexBuffer::GLVertexBuffer(void* data, size_t size, BufferAccess access)
	{
		glGenBuffers(1, &m_glbuffer);
		glBindBuffer(GL_ARRAY_BUFFER, m_glbuffer);
		glBufferData(GL_ARRAY_BUFFER, size, data, getBufferAccess(access));
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	GLVertexBuffer::~GLVertexBuffer()
	{
		glDeleteBuffers(1, &m_glbuffer);
	}

	void GLVertexBuffer::bind()
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_glbuffer);
	}

	void* GLVertexBuffer::map(BufferMapping mapping)
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_glbuffer);
		void* ret = glMapBuffer(GL_ARRAY_BUFFER, getBufferMapping(mapping));
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		return ret;
	}

	void GLVertexBuffer::unmap()
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_glbuffer);
		glUnmapBuffer(GL_ARRAY_BUFFER);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	GLIndexBuffer::GLIndexBuffer(unsigned int* data, size_t size, BufferAccess access)
	{
		glGenBuffers(1, &m_glbuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_glbuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, getBufferAccess(access));
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	GLIndexBuffer::~GLIndexBuffer()
	{
		glDeleteBuffers(1, &m_glbuffer);
	}

	void GLIndexBuffer::bind()
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_glbuffer);
	}

	void* GLIndexBuffer::map(BufferMapping mapping)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_glbuffer);
		void* ret = glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, getBufferMapping(mapping));
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		return ret;
	}

	void GLIndexBuffer::unmap()
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_glbuffer);
		glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	GLenum getBufferAccess(BufferAccess access)
	{
		switch (access)
		{
		case BufferAccess::Static: return GL_STATIC_DRAW;
		case BufferAccess::Dynamic: return GL_DYNAMIC_DRAW;
		case BufferAccess::Stream: return GL_STREAM_DRAW;
		}

		return 0;
	}

	GLenum getBufferMapping(BufferMapping mapping)
	{
		switch (mapping)
		{
		case BufferMapping::ReadOnly: return GL_READ_ONLY;
		case BufferMapping::WriteOnly: return GL_WRITE_ONLY;
		case BufferMapping::ReadAndWrite: return GL_READ_WRITE;
		}

		return 0;
	}

}
