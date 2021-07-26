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

	public:
		glm::vec3 m_position;
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
