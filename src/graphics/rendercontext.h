#ifndef RENDERCONTEXT_H
#define RENDERCONTEXT_H

namespace engine
{
	class RenderContext
	{
	public:
		int width;
		int height;
		glm::mat4 view;
		glm::mat4 proj;
		glm::mat4 model;

	private:
		static RenderContext ms_instance;

	public:
		static RenderContext& getContext();
		static void setContext(RenderContext& ctx);

	};
}

#endif