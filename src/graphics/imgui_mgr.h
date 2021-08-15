#ifndef IMGUI_MGR_H
#define IMGUI_MGR_H

struct GLFWwindow;

namespace engine
{
	class ImguiManager : public Singleton<ImguiManager>
	{
	public:
		void init(GLFWwindow* window);
		void shutdown();

		void beginFrame();
		void endFrame();

	private:
		void draw();
	};

	extern ImguiManager g_imguiMgr;
}

#endif // !IMGUI_MGR_H
