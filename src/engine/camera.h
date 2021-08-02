#ifndef CAMERA_H
#define CAMERA_H

#include "graphics/view.h"

namespace engine
{
	class Camera
	{
	public:
		void  setView(View* view) { m_view = view; m_view->updateInternalValues(); }
		View* getView() { return m_view; }

		glm::vec3& getPosition() { return m_position; }

		void update();

	private:
		glm::mat4 get_view_matrix();

		View* m_view;

	public:
		glm::vec3 m_position = glm::vec3(0.0, 0.0, 1.0);
		glm::vec3 m_direction = glm::vec3(0.0, 0.0, -1.0);
		float yaw = 0.0f;
		float pitch = 0.0f;
	};

	class CameraProxy : public Singleton<CameraProxy>,
						public Camera
	{
	private:
		static CameraProxy ms_cameraProxy;

	public:


	};
}

#endif // !CAMERA_H
