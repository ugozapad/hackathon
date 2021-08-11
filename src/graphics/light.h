#ifndef LIGHT_H
#define LIGHT_H

#include "engine/components/component.h"
#include "lightmanager.h"

namespace engine
{
	struct LightRenderData
	{
		glm::vec3 m_pos;
		glm::vec3 m_dir;
		glm::vec3 m_color;
		glm::vec3 m_ambientColor;
		glm::vec3 m_specularColor;
		float m_shininess;
	};

	class LightComponent : public Component
	{
		ImplementObject(LightComponent, Component);
	public:
		LightComponent();
		~LightComponent();

		static void registerObject();

		void getRenderData(LightRenderData* data);

	private:
		glm::vec3 m_color;
		glm::vec3 m_ambientColor;
		glm::vec3 m_specularColor;
		float m_shininess;
	};
}

#endif // !LIGHT_H
