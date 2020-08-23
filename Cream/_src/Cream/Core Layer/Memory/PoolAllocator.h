#pragma once
#include "Cream/Platform Independence Layer/Collections/ListSingleLink.h"
#include "Cream/Core Layer/Assertions.h"

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
		// Create a pool allocator with given chunks per block of the pool
		PoolAllocator(size_t bytesPerChunk, size_t chunksPerBlock) :
			m_BytesPerChunk(bytesPerChunk), m_ChunksPerBlock(chunksPerBlock)
		{
			CREAM_ASSERT(m_BytesPerChunk >= sizeof(void*)); // Chunk sizes must be at least the size of a pointer to allow for pointer storage
		}

		// Frees all of the memory allocated by the pool allocator back to the OS
		~PoolAllocator()
		{
			freeAllBlocks();
		}

		// Allocates a chunk of size 'sizeBytes' by removing its pointer from the list of free chunks
		void* allocate();

		// Puts the deallocated chunk at the front of the free chunks list
		void deallocate(void* chunk);

	private:
		// Allocates a larger block for chunks
		Chunk* allocateBlock();

		void freeAllBlocks();

		// Size of each chunk in bytes
		size_t m_BytesPerChunk;
		// Number of chunks to allocate in each block of the pool
		size_t m_ChunksPerBlock;
		// Allocation pointer
		Chunk* m_Alloc = nullptr;

		// Pointer to each block
		Cream::List<Chunk*> m_Blocks;
	};
}