#ifndef CONTENT_H
#define CONTENT_H

#include "file/datastream.h"

namespace engine
{
	// Base class for all engine manager resources (i.e. textures, models and other shit)
	class Content //: std::enable_shared_from_this<Content>
	{
	public:
		Content();
		Content(const std::string& filename);
		virtual ~Content();

		virtual void load(const std::shared_ptr<DataStream>& dataStream);

	protected:
		std::string m_filename;
	};
}

#endif // !CONTENT_H
