#include "creampch.h"
#include "DoubleBufferedAllocator.h"

namespace Cream
{
	DoubleBufferedAllocator::DoubleBufferedAllocator(U32 stackSizeBytes)
		: m_StackSizeBytes(stackSizeBytes)
	{
	}

	void DoubleBufferedAllocator::swapBuffers()
	{
		m_CurrentStack = static_cast<U32>(!m_CurrentStack);
	}

	void DoubleBufferedAllocator::clearCurrentBuffer()
	{
		m_Stacks[m_CurrentStack].clear();
	}

	void* DoubleBufferedAllocator::alloc(U32 nBytes)
	{
		return m_Stacks[m_CurrentStack].alloc(nBytes);
	}

	void* DoubleBufferedAllocator::alloc(U32 nBytes, U32 alignment)
	{
		return m_Stacks[m_CurrentStack].alloc(nBytes, alignment);
	}
}