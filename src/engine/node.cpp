#include "pch.h"
#include "engine/node.h"

#include "engine/components/component.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

namespace engine
{
	void Node::registerObject()
	{
		Context::getInstance()->registerObject<Node>();
	}

	Node::Node() :
		m_rootNode(nullptr),
		m_position(0.0f, 0.0f, 0.0f),
		m_rotation(0.0f, 0.0f, 0.0f),
		m_scale(1.0f, 1.0f, 1.0f)
	{
	}

	Node::~Node()
	{

	}

	glm::mat4 Node::getTranslation()
	{
		glm::vec3 radiansRotation = glm::vec3(glm::radians(m_rotation.x), glm::radians(m_rotation.y), glm::radians(m_rotation.z));
		glm::mat4 rotation = glm::toMat4(glm::quat(radiansRotation));
		glm::mat4 tranlation = glm::mat4(1.0f);

		if (m_rootNode)
			tranlation = m_rootNode->getTranslation() * glm::translate(tranlation, m_position) * rotation * glm::scale(tranlation, m_scale);
		else
			tranlation = glm::translate(tranlation, m_position) * rotation * glm::scale(tranlation, m_scale);

		return tranlation;
	}

	Component* Node::getComponentByTypeInfo(const TypeInfo* typeinfo)
	{
		for (ComponentIt it = m_components.begin(); it != m_components.end(); ++it)
		{
			eastl::shared_ptr<Component> component = *it;

			if (component)
			{
				if (component->isA(typeinfo))
				{
					return component.get();
				}	
			}
		}

		return nullptr;
	}

}