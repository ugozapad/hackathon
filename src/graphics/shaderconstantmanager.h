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

	class ShaderConstantManager : public Singleton<ShaderConstantManager>
	{
	private:
		static ShaderConstantManager ms_instance;

	};
}

#endif // !SHADERCONSTANTMANAGER_H
