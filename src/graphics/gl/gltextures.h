#ifndef RENDER_GL_TEXTURES_H
#define RENDER_GL_TEXTURES_H

#include "graphics/texture.h"
#include "graphics/gl/glad/glad.h"

namespace engine
{
	class GLTexture2D : public GrTexture2D
	{
	public:
		GLTexture2D(const TextureCreationDesc& desc);
		~GLTexture2D();

		void release() override;
		uint32_t getHandle() override { return m_handle; }
		GLenum getType() { return m_textureType; }
	private:
		TextureCreationDesc m_desc;
		GLuint m_handle;
		GLenum m_textureType;
	};
}

#endif // !RENDER_GL_TEXTURES_H
