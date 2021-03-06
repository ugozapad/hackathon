#ifndef SHADERMANAGER_H
#define SHADERMANAGER_H

namespace engine
{
	class Shader;
	
	class ShaderManager : public Singleton<ShaderManager>
	{
	private:
		static ShaderManager ms_instance;

	public:
		void init();
		void shutdown();

		Shader* createShader(const std::string& name);
		Shader* createShader(const std::string& name, const std::string& vsfilename, const std::string& fsfilename);

		void reloadShaders();

	private:
		std::map<std::string, Shader*> m_shaders;
	};
}

#endif // !SHADERMANAGER_H
