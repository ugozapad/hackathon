#ifndef SHADERCONSTANTMANAGER_H
#define SHADERCONSTANTMANAGER_H

namespace engine
{
	enum class ShaderConstantType
	{
		Int,
		Float,
		Vector2,
		Vector3,
		Vector4,
		Matrix3x3,
//		Matrix3x4,
		Matrix4x4,
	};

	class Shader;

	class ShaderConstantManager : public Singleton<ShaderConstantManager>
	{
	private:
		static ShaderConstantManager ms_instance;

	public:
		void applyDefaultContants(Shader* shader);
	};
}

#endif // !SHADERCONSTANTMANAGER_H
