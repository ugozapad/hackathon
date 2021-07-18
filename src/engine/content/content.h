#ifndef CONTENT_H
#define CONTENT_H

#include "file/datastream.h"

namespace engine
{
	// Base class for all engine manager resources (i.e. textures, models and other shit)
	class Content //: eastl::enable_shared_from_this<Content>
	{
	public:
		Content();
		virtual ~Content();

		virtual void load(const eastl::shared_ptr<DataStream>& dataStream);
	};
}

#endif // !CONTENT_H
