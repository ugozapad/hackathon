#ifndef CONTEXT_H
#define CONTEXT_H

#include "core/object.h"

namespace engine
{
	class FactoryBase
	{
	protected:
		const TypeInfo* m_typeInfo;

	public:
		const TypeInfo* getTypeInfo() { return m_typeInfo; }

	public:
		virtual std::shared_ptr<Object> createObject() = 0;
	};

	template <typename T>
	class FactoryTemplated : public FactoryBase
	{
	public:
		FactoryTemplated(const TypeInfo* typeInfo)
		{
			m_typeInfo = typeInfo;
		}

		std::shared_ptr<Object> createObject() override
		{
			typename std::shared_ptr<T> object = typename std::make_shared<T>();
			return std::static_pointer_cast<Object>(object);
		}
	};

	class Context : public Singleton<Context>
	{
	public:
		template <typename T>
		void registerObject()
		{
			m_factories.push_back(new FactoryTemplated<T>(T::getStaticTypeInfo()));
		}

		template <typename T>
		std::shared_ptr<T> createObject()
		{
			for (int i = 0; i < m_factories.size(); i++)
				if (m_factories[i]->getTypeInfo() == T::getStaticTypeInfo())
					return std::static_pointer_cast<T>(m_factories[i]->createObject());

			return std::shared_ptr<T>();
		}
		
		std::shared_ptr<Object> createObject(const TypeInfo* typeInfo)
		{
			for (int i = 0; i < m_factories.size(); i++)
				if (m_factories[i]->getTypeInfo() == typeInfo)
					return m_factories[i]->createObject();

			return std::shared_ptr<Object>();
		}

	private:
		std::vector<FactoryBase*> m_factories;
	};
}

#endif // !CONTEXT_H
