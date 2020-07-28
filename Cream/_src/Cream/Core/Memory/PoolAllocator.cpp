#include "creampch.h"
#include "PoolAllocator.h"
#include "Cream/Core/Assertions.h"

namespace Cream
{
	void* PoolAllocator::allocate(size_t sizeBytes)
	{
		// No chunks left in the current block,
		// or no block in existence.
		// Allocate a new one
		if (m_Alloc == nullptr)
		{
			m_Alloc = allocateBlock(sizeBytes);
		}

		// Return the chunk that is being allocated
		Chunk* freeChunk = m_Alloc;

		// Advance the free chunk pointer
		m_Alloc = m_Alloc->next;

		return freeChunk;
	}

	void PoolAllocator::deallocate(void* chunk, size_t sizeBytes)
	{
		// The freed chunk's 'next' pointer points to 
		// the current allocation pointer
		reinterpret_cast<Chunk*>(chunk)->next = m_Alloc;

		// The allocation pointer is set to the newly freed chunk
		m_Alloc = reinterpret_cast<Chunk*>(chunk);
	}

	PoolAllocator::Chunk* PoolAllocator::allocateBlock(size_t chunkSizeBytes)
	{
		CREAM_ASSERT(chunkSizeBytes >= sizeof(void*)); // Chunk sizes must be at least the size of a pointer to allow for pointer storage

		// Calculate the size in bytes of the block
		size_t blockSize = m_ChunksPerBlock * chunkSizeBytes;

		// Allocate the block with a pointer to its beginning
		size_t effectiveSize = blockSize + chunkSizeBytes;
		Chunk* blockBegin = reinterpret_cast<Chunk*>(malloc(effectiveSize));

		// Align the first chunk of the allocated block
		U32 offset = (reinterpret_cast<uintptr_t>(blockBegin)) % chunkSizeBytes;
		Chunk* alignedBlock = reinterpret_cast<Chunk*>(reinterpret_cast<uintptr_t>(blockBegin) + chunkSizeBytes - offset);
		
		// Link all the new free chunks together
		Chunk* chunk = alignedBlock;
		for (size_t i = 0; i < m_ChunksPerBlock - 1; i++)
		{
			chunk->next = reinterpret_cast<Chunk*>(reinterpret_cast<intptr_t>(chunk) + chunkSizeBytes);
			chunk = chunk->next;
		}
		chunk->next = nullptr;

		return alignedBlock;
	}
}