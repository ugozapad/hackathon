#ifndef GRAPHICSSETTINGS_H
#define GRAPHICSSETTINGS_H

namespace engine
{
	class GraphicsOptions
	{
	public:
		bool loadSettings(const eastl::string& filename);
		void saveSettings(const eastl::string& filename);

		void applyDefaultOptions();

	public:
		int m_width, m_height;
		bool m_fullscreen;
	};

	extern GraphicsOptions g_graphicsOptions;
}

#endif // !GRAPHICSSETTINGS_H
