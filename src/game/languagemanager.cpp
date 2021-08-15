#include "pch.h"
#include "game/languagemanager.h"
#include "file/filesystem.h"
#include <sstream>
#include <strstream>

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

	bool has_chars(char c)
	{
		if (c == '\"' || c == '.' || c == ',' || c == '\'')
			return true;
		else
			return false;
	}


	void LanguageManager::init()
	{
		Languages lang = getPreferedLanguage();

		// #TODO : HACK HACK HACK
		lang = Languages::English;

		setLocaleLang(lang);

		std::string filename;

		if (lang == Languages::English)
			filename = "lang_en.txt";
		else if (lang == Languages::Russian)
			filename = "lang_ru.txt";

		DataStreamPtr langFile = FileSystem::getInstance()->openReadFile(filename);
		std::string langFileData;

		langFile->seek(FileSeek::End, 0);
		size_t size = langFile->tell();
		langFile->seek(FileSeek::Begin, 0);

		langFileData.resize(size + 1);
		langFile->read(&langFileData[0], size);

		langFileData[size] = '\0';

		std::stringstream langDataStream;
		langDataStream << langFileData;

		

		char buffer[256];
		while (langDataStream.getline(buffer, 256))
		{
			if (buffer[0] == '/' && buffer[1] == '/')
				continue;

			if (buffer[0] == '\r')
				continue;



			std::string str = buffer;

			std::string key;
			std::string text;

			int endkey = 0;
			for (int i = 0; i < str.size(); i++)
			{
				char c = str[i];
				if (std::isspace(c))
				{
					endkey = i;
					break;
				}

				key.push_back(c);
			}

			for (int i = endkey+1; i < str.size(); i++)
			{
				char c = str[i];
				if (c == '"')
					continue;

				text.push_back(c);
			}

			m_strings.emplace(key, text);
		}
		
		for (auto it : m_strings)
		{
			it.second.erase(std::remove(it.second.begin(), it.second.end(), '"'), it.second.end());
			std::remove_if(it.second.begin(), it.second.end(), has_chars);
		}

	}

	void LanguageManager::shutdown()
	{

	}

	std::string LanguageManager::getAsciiStr(const std::string& langStrName)
	{
		auto text = m_strings.find(langStrName);
		if (text == m_strings.end())
			return langStrName;
		
		return text->second;
	}

	std::wstring LanguageManager::getWideStr(const std::string& langStrName)
	{
		throw std::logic_error("LanguageManager::getWideStr: not implemented");
	}

}