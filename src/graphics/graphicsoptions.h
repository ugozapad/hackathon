#ifndef GRAPHICSSETTINGS_H
#define GRAPHICSSETTINGS_H

namespace engine
{
	class GraphicsOptions
	{
	public:
		bool loadSettings(const std::string& filename);
		void saveSettings(const std::string& filename);

		void applyDefaultOptions();

	public:
		int m_width, m_height;
		int m_refreshRate;
		bool m_fullscreen;

		int m_anisotropicQuality;
		int m_shadowsQuality;
	};

	extern GraphicsOptions g_graphicsOptions;
}

#endif // !GRAPHICSSETTINGS_H
