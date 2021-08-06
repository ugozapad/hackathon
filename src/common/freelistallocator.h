#ifndef FREELISTALLOCATOR_H
#define FREELISTALLOCATOR_H

namespace engine
{
	class FreeListAllocator : public AllocatorBase
	{
	public:

		FreeListAllocator(size_t size, void* start);
		~FreeListAllocator();

		void* allocate(size_t size, size_t alignment) override;
		void  free(void* p) override;

	private:

		struct AllocationHeader { size_t size; uint8_t adjustment; };
		struct FreeBlock { size_t size; FreeBlock* next; };
		FreeListAllocator(const FreeListAllocator&);

		//Prevent copies because it might cause errors 
		FreeListAllocator& operator=(const FreeListAllocator&);
		FreeBlock* _free_blocks;
	};
}

#endif // !FREELISTALLOCATOR_H
