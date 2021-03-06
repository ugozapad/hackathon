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
		DEPTH24,
		DEPTH32,

		// Float
		RGB16F,
		RGBA16F,
		RGB32F,
		RGBA32F,
		DEPTH32F
	};

	enum class BufferMapping
	{
		ReadOnly,
		WriteOnly,
		ReadAndWrite
	};

	enum class BufferDataType
	{
		UnsignedInt,
	};

	struct BufferCreationDesc
	{
		void* m_data;
		size_t m_dataSize;
		BufferAccess m_access;
	};

	struct GrBufferBase
	{
		virtual ~GrBufferBase() {}

		virtual void* map(BufferMapping mapping) = 0;
		virtual void  unmap() = 0;
	};

	enum class PrimitiveMode
	{
		Triangles,
		TriangleStrip,
	};

	struct Viewport
	{
		int m_x;
		int m_y;
		int m_width;
		int m_height;
	};

	enum class TextureWrapComponent
	{
		S,
		T
	};

	enum class TextureWrap
	{
		Repeat,
		MirroredRepeat,
		ClampToEdge,
		ClampToBorder
	};

	enum class TextureFilter
	{
		Nearest,
		Linear,
		NearestMipmapNearest,
		LinearMipmapNearest,
		NearestMipmapLinear,
		LinearMipmapLinear
	};

}

#endif // GFXCOMMON_H
