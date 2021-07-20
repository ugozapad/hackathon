#ifndef CONTEXT_H
#define CONTEXT_H

#include "core/object.h"

namespace engine
{
	class FactoryBase
	{
	public:
		virtual eastl::shared_ptr<Object> createObject() = 0;
	};

	template <typename T>
	class FactoryTemplated : public FactoryBase
	{
	private:
		const TypeInfo* m_typeInfo;
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

	private:
		eastl::vector<FactoryBase*> m_factories;
	};
}

#endif // !CONTEXT_H
