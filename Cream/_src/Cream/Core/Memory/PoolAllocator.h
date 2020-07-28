#pragma once

namespace Cream
{
	



	// The Pool Allocator class.
	// Features:
	//	- Parameterized by number of chunks per block
	//	- Keeps track of the allocation pointer
	//  - Bump-allocates chunks
	//	- Requests a new larger block when needed
	class PoolAllocator
	{
	private:
		struct Chunk
		{
			/*
			When a chunk is free, the 'next' contains
			the address of the next chunk in a list.

			When the chunk is allocated, this space is
			used by the user.
			*/
		public:
			Chunk* next;
		};

	public:
		PoolAllocator(size_t chunksPerBlock) :
			m_ChunksPerBlock(chunksPerBlock)
		{
		}

		void* allocate(size_t sizeBytes);
		// Puts the deallocated chunk at the front of the free chunks list
		void deallocate(void* chunk, size_t sizeBytes);

	private:
		// Allocates a larger block for chunks
		Chunk* allocateBlock(size_t chunkSizeBytes);

		// Number of chunks to allocate in each block of the pool
		size_t m_ChunksPerBlock;
		// Allocation pointer
		Chunk* m_Alloc = nullptr;
	};
}