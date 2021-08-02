#include "pch.h"
#include "engine/camera.h"

namespace engine
{
	CameraProxy CameraProxy::ms_cameraProxy;

	void Camera::update()
	{
		m_view->m_view = get_view_matrix();
		m_view->updateInternalValues();	
	}

	glm::mat4 Camera::get_view_matrix()
	{
		return glm::lookAt(m_position, m_position + m_direction, glm::vec3(0.0f, 1.0f, 0.0f));
	}

}