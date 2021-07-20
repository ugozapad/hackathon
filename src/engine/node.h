#ifndef NODE_H
#define NODE_H

namespace engine
{
	class Component;

	class Node : public Object
	{
		ImplementObject(Node, Object);
	public:
		static void registerObject();

	public:
		Node();
		virtual ~Node();

		glm::vec3 getPosition() { return m_position; }
		glm::vec3 getRotation() { return m_rotation; }
		glm::vec3 getScale() { return m_scale; }

		void setPosition(glm::vec3& pos) { m_position = pos; }
		void setRotation(glm::vec3& rot) { m_rotation = rot; }
		void setScale(glm::vec3& scale) { m_scale = scale; }

		glm::mat4 getTranslation();

		Component* getComponentByTypeInfo(const TypeInfo* typeinfo);

		template <typename T>
		T* getComponentByType()
		{
			return (T*)getComponentByTypeInfo(T::getStaticTypeInfo());
		}

		template <typename T>
		eastl::shared_ptr<T> createComponentByType()
		{
			eastl::shared_ptr<T> component = Context::getInstance()->createObject<T>();
			m_components.push_back(component);
			return component;
		}

	private:
		typedef eastl::fixed_list<eastl::shared_ptr<Component>, 56>::iterator ComponentIt;
		eastl::fixed_list<eastl::shared_ptr<Component>, 56> m_components;

		Node* m_rootNode;

		glm::vec3 m_position;
		glm::vec3 m_rotation;
		glm::vec3 m_scale;
	};
}

#endif