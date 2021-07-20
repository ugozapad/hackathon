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

	private:
		View* m_view;

		glm::vec3 m_position;
	};

	class CameraProxy : public Singleton<CameraProxy>
	{
	private:
		static CameraProxy ms_cameraProxy;

	public:
		void    setCamera(Camera* camera) { m_camera = camera; }
		Camera* getCamera() { return m_camera; }

		void    setView(View* view) { m_camera->setView(view); }
		View*   getView() { return  m_camera->getView(); }

	private:
		Camera* m_camera;
	};
}

#endif // !CAMERA_H
