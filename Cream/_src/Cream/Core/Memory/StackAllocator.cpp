#include "creampch.h"
#include "StackAllocator.h"

namespace Cream
{
	StackAllocator::StackAllocator(U32 stackSizeBytes)
		: m_StackSizeBytes(stackSizeBytes), m_Marker(0)
	{
		m_Pointer = reinterpret_cast<intptr_t>(malloc(m_StackSizeBytes));
	}

	StackAllocator::~StackAllocator()
	{
		free(reinterpret_cast<void*>(m_Pointer));
	}

	void* StackAllocator::alloc(U32 sizeBytes)
	{
		return alloc(sizeBytes, sizeBytes);
	}

	void* StackAllocator::alloc(U32 sizeBytes, U32 alignment)
	{
		const intptr_t currentPointer = m_Pointer + m_Marker;
		const U32 offset = currentPointer % alignment;
		const U32 effectiveSize = (offset == 0) ? sizeBytes : sizeBytes + alignment - offset;
		if (m_Marker + effectiveSize > m_StackSizeBytes)
		{
			CREAM_ASSERT(m_Marker + effectiveSize <= m_StackSizeBytes);
			return nullptr;
		}	

		m_Marker += effectiveSize;
		void* alignedPtr = reinterpret_cast<void*>((offset == 0) ? currentPointer : currentPointer + alignment - offset);
		return alignedPtr;
	}

	StackAllocator::Marker StackAllocator::getMarker()
	{
		return m_Marker;
	}

	void StackAllocator::freeToMarker(Marker marker)
	{
		m_Marker = (marker >= 0 && marker <= m_StackSizeBytes) ? marker : m_Marker;
	}

	void StackAllocator::clear()
	{
		m_Marker = 0;
	}
	U32 StackAllocator::getUnusedBytes()
	{
		return m_StackSizeBytes - m_Marker;
	}
	U32 StackAllocator::getUsedBytes()
	{
		return m_Marker;
	}
	U32 StackAllocator::getTotalBytes()
	{
		return m_StackSizeBytes;
	}
	
}
