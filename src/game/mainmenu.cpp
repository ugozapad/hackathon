#include "pch.h"
#include "game/mainmenu.h"
#include "imgui.h"
#include "graphics/view.h"
#include "game/languagemanager.h"
#include "graphics/graphicsoptions.h"
#include <GLFW/glfw3.h>

namespace engine
{
	extern View* g_engineView;
	extern GLFWwindow* g_engineWindow;

	extern void loadLevel();

	class VidMode
	{
	public:
		struct VideoMode
		{
			std::string name;
			int width, height, refreshRate;
		};
	public:
		static bool ms_inited;
		static int ms_vidModeCount;
		static int ms_currentVidMode;
		static std::vector<VideoMode> ms_vidModes;
	public:
		static void init();
	};

	bool                                     VidMode::ms_inited = false;
	int                                      VidMode::ms_vidModeCount = 0;
	int                                      VidMode::ms_currentVidMode;
	std::vector<VidMode::VideoMode>          VidMode::ms_vidModes;

	void VidMode::init()
	{
		GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();
		const GLFWvidmode* videoModes = glfwGetVideoModes(primaryMonitor, &ms_vidModeCount);

		for (int i = 0; i < ms_vidModeCount; i++)
		{
			spdlog::info("{} {} {}", videoModes[i].width, videoModes[i].height, videoModes[i].refreshRate);

			int refresh = videoModes[i].refreshRate;
			if (refresh != 60 && refresh != 70 /*&& refresh != 75*/)
				continue;

			const GLFWvidmode& vidMode = videoModes[i];

			VideoMode mode;
			mode.width = vidMode.width;
			mode.height = vidMode.height;
			mode.refreshRate = vidMode.refreshRate;

			char buffer[256];
			sprintf(buffer, "%ix%i %i", mode.width, mode.height, mode.refreshRate);
			mode.name = buffer;

			ms_vidModes.push_back(mode);
		}


		for (int i = 0; i < ms_vidModes.size(); i++)
		{
			GraphicsOptions* opts = &g_graphicsOptions;

			if (opts->m_width == ms_vidModes[i].width &&
				opts->m_height == ms_vidModes[i].height &&
				opts->m_refreshRate == ms_vidModes[i].refreshRate)
			{
				ms_currentVidMode = i;
				break;
			}
		}

		ms_inited = true;
	}

	MainMenu MainMenu::ms_instance;

	void MainMenu::render()
	{
		if (!VidMode::ms_inited)
			VidMode::init();

		LanguageManager* langman = LanguageManager::getInstance();

		ImGui::SetNextWindowPos(ImVec2(100, 300));
		ImGui::SetNextWindowSize(ImVec2(g_engineView->m_width / 2, g_engineView->m_height / 2));

		ImGui::Begin("Main Menu", nullptr, ImGuiWindowFlags_NoDecoration);

		if (ImGui::Button(langman->getAsciiStr("#STR_MENU_NEW_GAME").c_str()))
		{
			loadLevel();
		}

		if (ImGui::Button(langman->getAsciiStr("#STR_MENU_SETTINGS").c_str()))
		{
			if (!SettingMenu::ms_isShowed)
				SettingMenu::ms_isShowed = true;
		}

		if (ImGui::Button(langman->getAsciiStr("#STR_MENU_EXIT").c_str()))
		{
			glfwSetWindowShouldClose(g_engineWindow, GLFW_TRUE);
		}

		ImGui::End();

		if (SettingMenu::ms_isShowed)
			SettingMenu::render();

		//ImGui::ShowDemoWindow();
	}

	bool SettingMenu::ms_isShowed = false;
	bool SettingMenu::ms_isShowedApplyMenu = false;

	void showApplyWindow()
	{
		LanguageManager* langman = LanguageManager::getInstance();

		int wndSize = 300;
		ImGui::SetNextWindowPos(ImVec2((g_engineView->m_width / 2) - (420 / 2), (g_engineView->m_height / 2) - (120 / 2)));
		//ImGui::SetNextWindowPos(ImVec2(0, 0), 0, ImVec2(wndSize/4, wndSize/4));
		ImGui::SetNextWindowSize(ImVec2(420, 120));
		ImGui::Begin("Settings Apply", 0, ImGuiWindowFlags_NoDecoration);
		ImGui::Text(langman->getAsciiStr("#STR_MENU_SETTINGS_ENGINE_RESTART").c_str());

		if (ImGui::Button("Ok"))
			SettingMenu::ms_isShowedApplyMenu = false;

		ImGui::End();
	}

	void SettingMenu::render()
	{
		LanguageManager* langman = LanguageManager::getInstance();
		GraphicsOptions* opts = &g_graphicsOptions;

		ImGui::Begin("Settings");

		ImGui::Text(langman->getAsciiStr("#STR_MENU_SETTINGS_SCREEN_RESOLUTION").c_str());
		ImGui::SameLine();
		static int item_current_idx = VidMode::ms_currentVidMode; // Here we store our selection data as an index.

		const char* combo_preview_value = VidMode::ms_vidModes[item_current_idx].name.c_str();
		if (ImGui::BeginCombo("", combo_preview_value))
		{
			for (int i = 0; i < VidMode::ms_vidModes.size(); i++)
			{

				const bool is_selected = (item_current_idx == i);
				if (ImGui::Selectable(VidMode::ms_vidModes[i].name.c_str(), is_selected))
					item_current_idx = i;

				// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}

			ImGui::EndCombo();
		}


		ImGui::Text(langman->getAsciiStr("#STR_MENU_SETTINGS_FULLSCREEN").c_str());

		ImGui::Text(langman->getAsciiStr("#STR_MENU_SETTINGS_SHADOWMAP_QUALITY").c_str());

		ImGui::Text(langman->getAsciiStr("#STR_MENU_SETTINGS_ANISOTROPIC_QUALITY").c_str());
		//ImGui::SameLine();
		//ImGui::SliderInt("", &opts->m_anisotropicQuality, 0, 16);

		ImGui::Separator();

		if (ImGui::Button("Apply"))
		{
			ms_isShowedApplyMenu = true;

			auto modee = VidMode::ms_vidModes[item_current_idx];
			g_graphicsOptions.m_width = modee.width;
			g_graphicsOptions.m_height = modee.height;
			g_graphicsOptions.saveSettings("engine.ini");
		}

		ImGui::SameLine();

		if (ImGui::Button("Cancel"))
		{
			SettingMenu::ms_isShowed = false;
		}

		ImGui::End();

		if (ms_isShowedApplyMenu)
			showApplyWindow();
	}

}
