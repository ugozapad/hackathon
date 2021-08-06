#ifndef LINEARALLOCATOR_H
#define LINEARALLOCATOR_H

namespace engine
{
	class LinearAllocator : public AllocatorBase
	{
	public:

		LinearAllocator(size_t size, void* start);
		~LinearAllocator();

		void* allocate(size_t size, size_t alignment) override;
		void  free(void* p) override;
		void  clear();

	private:

		LinearAllocator(const LinearAllocator&);

		//Prevent copies because it might cause errors 
		LinearAllocator& operator=(const LinearAllocator&);
		void* m_currentPos;
	};
}

#endif // !LINEARALLOCATOR_H
