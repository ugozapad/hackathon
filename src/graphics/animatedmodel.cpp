#include "pch.h"
#include "graphics/animatedmodel.h"

#include "ozz/base/io/stream.h"
#include "ozz/base/io/archive.h"

#include "file/datastream.h"

namespace engine
{
	class OzzStream : public ozz::io::Stream
	{
	public:
		OzzStream(const std::shared_ptr<DataStream>& stream);
		~OzzStream();

		bool opened() const override;

		size_t Read(void* _buffer, size_t _size) override;
		size_t Write(const void* _buffer, size_t _size) override;

		int Seek(int _offset, Origin _origin) override;

		int Tell() const override;

		size_t Size() const override;

	private:
		std::shared_ptr<DataStream>& m_stream;
		size_t m_streamSize;
	};

	OzzStream::OzzStream(const std::shared_ptr<DataStream>& stream) :
		m_stream(const_cast<std::shared_ptr<DataStream>&>(stream))
	{
		m_stream->seek(FileSeek::End, 0);
		m_streamSize = m_stream->tell();
		m_stream->seek(FileSeek::Begin, 0);
	}

	OzzStream::~OzzStream()
	{

	}

	bool OzzStream::opened() const
	{
		return true;
	}

	size_t OzzStream::Read(void* _buffer, size_t _size)
	{
		size_t a = m_stream->tell();
		m_stream->read(_buffer, _size);
		size_t b = m_stream->tell();

		return b - a;
	}

	size_t OzzStream::Write(const void* _buffer, size_t _size)
	{
		size_t a = m_stream->tell();
		m_stream->write((void*)_buffer, _size);
		size_t b = m_stream->tell();

		return b - a;
	}

	int OzzStream::Seek(int _offset, Origin _origin)
	{
		FileSeek origin;

		switch (_origin)
		{
		case ozz::io::Stream::kCurrent:
			origin = FileSeek::Current;
			break;
		case ozz::io::Stream::kEnd:
			origin = FileSeek::End;
			break;
		case ozz::io::Stream::kSet:
			origin = FileSeek::Begin;
			break;
		default:
			break;
		}

		m_stream->seek(origin, _offset);

		return 0;
	}

	int OzzStream::Tell() const
	{
		return m_stream->tell();
	}

	size_t OzzStream::Size() const
	{
		return m_streamSize;
	}

	void testOzz()
	{
		std::shared_ptr<DataStream> stream = std::make_shared<DataStream>();
		OzzStream ozzStream(stream);
		ozz::io::IArchive archive(&ozzStream);

		Animator animator;
		archive >> animator.m_skeleton;

	}

}