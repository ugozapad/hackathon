#ifndef CAMERAFRUSTUM_H
#define CAMERAFRUSTUM_H

namespace engine
{
	struct Plane
	{
		float a, b, c;
		float d;
	};

	class CameraFrustum
	{
	public:
		void update(glm::mat4& vp);

	private:
		Plane m_plane[6];
	};
}

#endif // !CAMERAFRUSTUM_H
