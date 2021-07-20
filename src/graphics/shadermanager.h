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

		Shader* createShader(const eastl::string& name);

	private:
		eastl::map<eastl::string, Shader*> m_shaders;
	};
}

#endif // !SHADERMANAGER_H
