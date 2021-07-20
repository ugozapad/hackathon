#ifndef OBJECT_H
#define OBJECT_H

namespace engine
{
	class TypeInfo
	{
	public:
		TypeInfo(const char* name, const TypeInfo* baseInfo);
		~TypeInfo();

	public:
		const char* m_name;
		const TypeInfo* m_baseInfo;
	};

#define ImplementObject(typeName, baseTypeName) \
	public: \
		static const TypeInfo* getStaticTypeInfo() { static const TypeInfo s_typeInfo(#typeName, 0); return &s_typeInfo; } \
		virtual const TypeInfo* getTypeInfo() { return getStaticTypeInfo();  }
	
	class Object
	{
		ImplementObject(Object, 0);
	public:
		Object();
		virtual ~Object();

		template <typename T>
		bool isA()
		{
			for (const TypeInfo* typeInfo = getTypeInfo(); typeInfo != nullptr; typeInfo = typeInfo->m_baseInfo)
				if (typeInfo == T::getStaticTypeInfo())
					return true;

			return false;
		}

		bool isA(const TypeInfo* classTypeInfo)
		{
			for (const TypeInfo* typeInfo = getTypeInfo(); typeInfo != nullptr; typeInfo = typeInfo->m_baseInfo)
				if (typeInfo == classTypeInfo)
					return true;

			return false;
		}
	};



}

#endif // !OBJECT_H
