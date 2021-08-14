#include "pch.h"
#include "game/languagemanager.h"

namespace engine
{
	LanguageManager LanguageManager::ms_instance;

	enum class Languages
	{
		English,
		Russian
	};

	Languages getPreferedLanguage()
	{
		if (strstr(GetCommandLineA(), "-lang_en"))
			return Languages::English;
		else if (strstr(GetCommandLineA(), "-lang_ru"))
			return Languages::Russian;

		WORD language = PRIMARYLANGID(GetSystemDefaultLangID());
		if (language == LANG_RUSSIAN)
			return Languages::Russian;
		
		// Default
		return Languages::English;
	}

	void setLocaleLang(Languages lang)
	{
		switch (lang)
		{
		case Languages::English:
			setlocale(LC_ALL, NULL);
			break;
		case Languages::Russian:
			setlocale(LC_ALL, "russian");
			break;
		default:
			Core::error("setStdLang: setLocaleLang: Failed to set language to locale! Unknowed language id!");
			break;
		}
	}

	void LanguageManager::init()
	{
		Languages lang = getPreferedLanguage();
		setLocaleLang(lang);

		std::string filename;

		if (lang == Languages::English)
			filename = "lang_en.txt";
		else if (lang == Languages::Russian)
			filename = "lang_ru.txt";


	}

	void LanguageManager::shutdown()
	{

	}

	std::string LanguageManager::getAsciiStr(const std::string& langStrName)
	{
		throw std::logic_error("LanguageManager::getAsciiStr: not implemented");
	}

	std::wstring LanguageManager::getWideStr(const std::string& langStrName)
	{
		throw std::logic_error("LanguageManager::getWideStr: not implemented");
	}

}