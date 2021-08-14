#ifndef LANGUAGEMANAGER_H
#define LANGUAGEMANAGER_H

namespace engine
{
	class LanguageManager : public Singleton<LanguageManager>
	{
		static LanguageManager ms_instance;
	public:
		void init();
		void shutdown();

		std::string getAsciiStr(const std::string& langStrName);
		std::wstring getWideStr(const std::string& langStrName);

	private:
		//std::unordered_map<>
	};
}

#endif // !LANGUAGEMANAGER_H
