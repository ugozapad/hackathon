#include "pch.h"
#include "engine/content/content.h"

namespace engine
{
	Content::Content()
	{

	}

	Content::Content(const std::string& filename)
	{
		m_filename = filename;
	}

	Content::~Content()
	{

	}

	void Content::load(const std::shared_ptr<DataStream>& dataStream)
	{

	}

}