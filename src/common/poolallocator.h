#ifndef POOLALLOCATOR_H
#define POOLALLOCATOR_H

namespace engine
{
	class PoolAllocator : public AllocatorBase
	{
	public:

		PoolAllocator(size_t objectSize, uint8_t objectAlignment, size_t size, void* mem);
		~PoolAllocator();
		void* allocate(size_t size, size_t alignment) override;
		void  free(void* p) override;

	private:

		PoolAllocator(const PoolAllocator&);

		//Prevent copies because it might cause errors 
		PoolAllocator& operator=(const PoolAllocator&); size_t _objectSize;

		uint8_t _objectAlignment;
		void** _free_list;
	};
}

#endif // !POOLALLOCATOR_H
