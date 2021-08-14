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

	void LanguageManager::init()
	{
		Languages lang = getPreferedLanguage();


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