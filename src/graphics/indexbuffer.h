#ifndef INDEX_BUFFER_H
#define INDEX_BUFFER_H

#include "gfxcommon.h"

namespace engine
{
	class GrIndexBuffer : public GrBufferBase
	{
	public:
		virtual ~GrIndexBuffer() {}

		virtual void bind() = 0;
	};
}

#endif // INDEX_BUFFER_H
