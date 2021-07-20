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
		virtual eastl::shared_ptr<Object> createObject() = 0;
	};

	template <typename T>
	class FactoryTemplated : public FactoryBase
	{
	public:
		FactoryTemplated(const TypeInfo* typeInfo)
		{
			m_typeInfo = typeInfo;
		}

		eastl::shared_ptr<Object> createObject() override
		{
			typename eastl::shared_ptr<T> object = typename eastl::make_shared<T>();
			return eastl::static_shared_pointer_cast<Object>(object);
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
		eastl::shared_ptr<T> createObject()
		{
			for (int i = 0; i < m_factories.size(); i++)
				if (m_factories[i]->getTypeInfo() == T::getStaticTypeInfo())
					return eastl::static_shared_pointer_cast<T>(m_factories[i]->createObject());

			return eastl::shared_ptr<T>();
		}
		
		eastl::shared_ptr<Object> createObject(const TypeInfo* typeInfo)
		{
			for (int i = 0; i < m_factories.size(); i++)
				if (m_factories[i]->getTypeInfo() == typeInfo)
					return m_factories[i]->createObject();

			return eastl::shared_ptr<Object>();
		}

	private:
		eastl::vector<FactoryBase*> m_factories;
	};
}

#endif // !CONTEXT_H
