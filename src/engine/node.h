#ifndef NODE_H
#define NODE_H

namespace engine
{
	class World;
	class Component;

	class Node : public Object
	{
		ImplementObject(Node, Object);
	public:
		static void registerObject();

	private:
		friend class World;

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

		std::shared_ptr<Component> createComponentByType(const TypeInfo* typeinfo);

		template <typename T>
		std::shared_ptr<T> createComponentByType()
		{
			return std::static_pointer_cast<T>(createComponentByType(T::getStaticTypeInfo()));
		}

		void setRootNode(Node* node);

		std::shared_ptr<Node> createChild();

	private:
		typedef std::list<std::shared_ptr<Component>>::iterator ComponentIt;
		std::list<std::shared_ptr<Component>> m_components;

		typedef std::list<std::shared_ptr<Node>>::iterator NodeIt;
		std::list<std::shared_ptr<Node>> m_childs;

		Node* m_rootNode;

		glm::vec3 m_position;
		glm::vec3 m_rotation;
		glm::vec3 m_scale;
	};
}

#endif