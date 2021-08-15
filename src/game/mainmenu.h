#ifndef MAINMENU_H
#define MAINMENU_H

namespace engine
{
	class MainMenu;

	class SettingMenu
	{
	public:
		static bool ms_isShowed;
		static bool ms_isShowedApplyMenu;
	public:
		static void render();
	};

	class MainMenu
	{
	public:
		static MainMenu ms_instance;
	public:
		void render();
	};
}

#endif // !MAINMENU_H
