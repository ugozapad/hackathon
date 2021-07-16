#include "pch.h"
#include "graphics/graphicsoptions.h"

#include "common/iniFile.h"

#include <GLFW/glfw3.h>

namespace engine
{
	GraphicsOptions g_graphicsOptions;

	bool GraphicsOptions::loadSettings(const eastl::string& filename)
	{
		CIniFile settingsFile(filename.c_str());
		if (!settingsFile.ReadFile())
			return false;

		m_width = settingsFile.GetValueI("DisplaySettings", "Width", 800);
		m_height = settingsFile.GetValueI("DisplaySettings", "Height", 600);
		m_fullscreen = settingsFile.GetValueB("DisplaySettings", "Fullscreen", false);

		return true;
	}

	void GraphicsOptions::saveSettings(const eastl::string& filename)
	{
		CIniFile settingsFile(filename.c_str());
		if (settingsFile.ReadFile())
			settingsFile.Erase();

		settingsFile.SetValueI("DisplaySettings", "Width", m_width);
		settingsFile.SetValueI("DisplaySettings", "Height", m_height);
		settingsFile.SetValueB("DisplaySettings", "Fullscreen", m_fullscreen);

		settingsFile.WriteFile();
	}

	void GraphicsOptions::applyDefaultOptions()
	{
		GLFWmonitor* monitor = glfwGetPrimaryMonitor();
		const GLFWvidmode* videoMode = glfwGetVideoMode(monitor);

		m_width = videoMode->width;
		m_height = videoMode->height;
		m_fullscreen = true;
	}

}