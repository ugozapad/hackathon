#include "pch.h"
#include "engine/content/content.h"

namespace engine
{
	Content::Content()
	{

	}

	Content::Content(const eastl::string& filename)
	{
		m_filename = filename;
	}

	Content::~Content()
	{

	}

	void Content::load(const eastl::shared_ptr<DataStream>& dataStream)
	{

	}

}