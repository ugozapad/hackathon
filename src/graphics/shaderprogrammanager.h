#ifndef SHADERPROGRAMMANAGER_H
#define SHADERPROGRAMMANAGER_H

namespace engine
{
	class ShaderProgram;

	class ShaderProgramManager
	{
	public:
		static ShaderProgramManager* instance();

	public:

		ShaderProgram* create_shader();

	};
}

#endif // !SHADERPROGRAMMANAGER_H
