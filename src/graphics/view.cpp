#include "pch.h"
#include "graphics/view.h"

namespace engine
{
	void View::updateInternalValues()
	{
		m_projection = glm::perspective(glm::radians(70.0f),
			(float)m_width / (float)m_height,
			m_znear,
			m_zfar);
	}
}