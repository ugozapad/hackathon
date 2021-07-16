#ifndef VIEW_H
#define VIEW_H

namespace engine
{
	class View
	{
	public:
		glm::mat4 m_projection, m_view;
		float m_fov, m_znear, m_zfar;
		int m_width, m_height;
		
	public:
		void updateInternalValues();
	};
}

#endif // !VIEW_H
