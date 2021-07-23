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

	private:
		std::map<std::string, Shader*> m_shaders;
	};
}

#endif // !SHADERMANAGER_H
