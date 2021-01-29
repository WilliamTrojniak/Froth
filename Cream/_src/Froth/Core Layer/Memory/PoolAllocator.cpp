#include "frothpch.h"
#include "PoolAllocator.h"

namespace Froth
{
	void* PoolAllocator::allocate()
	{
		// No chunks left in the current block,
		// or no block in existence.
		// Allocate a new one
		if (m_Alloc == nullptr)
		{
			m_Alloc = allocateBlock();
		}

		// Return the chunk that is being allocated
		Chunk* freeChunk = m_Alloc;

		// Advance the free chunk pointer
		m_Alloc = m_Alloc->next;

		return freeChunk;
	}

	void PoolAllocator::deallocate(void* chunk)
	{
		// The freed chunk's 'next' pointer points to 
		// the current allocation pointer
		reinterpret_cast<Chunk*>(chunk)->next = m_Alloc;

		// The allocation pointer is set to the newly freed chunk
		m_Alloc = reinterpret_cast<Chunk*>(chunk);
	}

	PoolAllocator::Chunk* PoolAllocator::allocateBlock()
	{
		// Calculate the size in bytes of the block
		size_t blockSize = m_ChunksPerBlock * m_BytesPerChunk;

		// Allocate the block with a pointer to its beginning
		size_t effectiveSize = blockSize + m_BytesPerChunk;
		Chunk* blockBegin = reinterpret_cast<Chunk*>(::operator new(effectiveSize));
		// old - Chunk* blockBegin = reinterpret_cast<Chunk*>(malloc(effectiveSize));
		m_Blocks.push_back(blockBegin);

		// Align the first chunk of the allocated block
		size_t offset = (reinterpret_cast<uintptr_t>(blockBegin)) % m_BytesPerChunk;
		Chunk* alignedBlock = reinterpret_cast<Chunk*>(reinterpret_cast<uintptr_t>(blockBegin) + m_BytesPerChunk - offset);
		
		// Link all the new free chunks together
		Chunk* chunk = alignedBlock;
		for (size_t i = 0; i < m_ChunksPerBlock - 1; i++)
		{
			chunk->next = reinterpret_cast<Chunk*>(reinterpret_cast<intptr_t>(chunk) + m_BytesPerChunk);
			chunk = chunk->next;
		}
		chunk->next = nullptr;

		return alignedBlock;
	}
	void PoolAllocator::freeAllBlocks()
	{
		for (Chunk* block : m_Blocks)
		{
			::operator delete(block, m_ChunksPerBlock * m_BytesPerChunk + m_BytesPerChunk);
		}
	}
}