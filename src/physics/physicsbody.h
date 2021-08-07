#ifndef PHYSICSBODY_H
#define PHYSICSBODY_H

namespace engine
{
	/*!
	\brief Base class for any rigid/static body in physics world.
	*/
	class PhysicsBody
	{
	public:
		/*!
		\brief Type of shape. If you using TriangleMesh, you need to pass collision mesh.
		*/
		enum class ShapeType
		{
			Box,
			Sphere,
			Cylinder,
			TriangleMesh,
		};

	public:

	};
}

#endif // !PHYSICSBODY_H
