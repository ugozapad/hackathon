#include "pch.h"
#include "core/object.h"

namespace engine
{
	TypeInfo::TypeInfo(const char* name, const TypeInfo* baseInfo)
	{
		m_name = name;
		m_baseInfo = baseInfo;
	}

	TypeInfo::~TypeInfo()
	{
		m_baseInfo = nullptr;
		m_name = nullptr;
	}

	Object::Object()
	{

	}

	Object::~Object()
	{

	}

}
