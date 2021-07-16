#ifndef GFXCOMMON_H
#define GFXCOMMON_H

namespace engine
{
	enum class BufferAccess
	{
		Static,
		Dynamic,
		Stream
	};

	enum class ImageFormat
	{
		// Unsigned byte 
		RGB16,
		RGBA16,
		RGB32,
		RGBA32,

		// Float
		RGB16F,
		RGBA16F,
		RGB32F,
		RGBA32F,
	};

	enum class BufferMapping
	{
		ReadOnly,
		WriteOnly,
		ReadAndWrite
	};

	struct IBufferBase
	{
		virtual ~IBufferBase() {}

		virtual void* Map(BufferMapping mapping) = 0;
		virtual void  Unmap() = 0;
	};
}

#endif // GFXCOMMON_H
