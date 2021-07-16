#ifndef VERTEX_BUFFER_H
#define VERTEX_BUFFER_H

#include "gfxcommon.h"

namespace engine
{
	class GrVertexBuffer : public GrBufferBase
	{
	public:
		virtual ~GrVertexBuffer() {}

		virtual void bind() = 0;
	};
}

#endif // VERTEX_BUFFER_H
