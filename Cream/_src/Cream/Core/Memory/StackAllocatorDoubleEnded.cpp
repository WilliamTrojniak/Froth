#include "creampch.h"
#include "StackAllocatorDoubleEnded.h"

namespace Cream
{
	StackAllocatorDoubleEnded::StackAllocatorDoubleEnded(U32 sizeBytes)
		: m_StackSizeBytes(sizeBytes), m_MarkerTop(0), m_MarkerBottom(0)
	{
		m_Pointer = reinterpret_cast<intptr_t>(::operator new(m_StackSizeBytes));
		// old - m_Pointer = reinterpret_cast<intptr_t>(malloc(m_StackSizeBytes));
		CREAM_ASSERT(m_Pointer); // Failed to allocate memory
	}

	StackAllocatorDoubleEnded::~StackAllocatorDoubleEnded()
	{
		::operator delete(reinterpret_cast<void*>(m_Pointer), m_StackSizeBytes);
		// old - free(reinterpret_cast<void*>(m_Pointer));
	}

	void* StackAllocatorDoubleEnded::allocTop(U32 sizeBytes)
	{
		return allocTop(sizeBytes, sizeBytes);
	}

	void* StackAllocatorDoubleEnded::allocTop(U32 sizeBytes, U32 alignment)
	{
		const intptr_t currentPointer = m_Pointer + m_MarkerTop;
		const U32 offset = currentPointer % alignment;
		const U32 effectiveSize = (offset == 0) ? sizeBytes : sizeBytes + alignment - offset;
		if (m_MarkerTop + effectiveSize > m_StackSizeBytes - m_MarkerBottom)
		{
			CREAM_ASSERT(m_MarkerTop + effectiveSize <= m_StackSizeBytes - m_MarkerBottom); // Insufficient memory available for requested allocation.
			return nullptr;
		}

		m_MarkerTop += effectiveSize;
		void* alignedPtr = reinterpret_cast<void*>((offset == 0) ? currentPointer : currentPointer + alignment - offset);
		return alignedPtr;
	}

	void* StackAllocatorDoubleEnded::allocBottom(U32 sizeBytes)
	{
		return allocBottom(sizeBytes, sizeBytes);
	}

	void* StackAllocatorDoubleEnded::allocBottom(U32 sizeBytes, U32 alignment)
	{
		const intptr_t unalignedPtr = m_Pointer + m_StackSizeBytes - m_MarkerBottom - sizeBytes;
		const U32 offset = unalignedPtr % alignment;
		const U32 effectiveSize = sizeBytes + offset;
		if (m_MarkerBottom + effectiveSize > m_StackSizeBytes - m_MarkerTop)
		{
			CREAM_ASSERT(m_MarkerBottom + effectiveSize <= m_StackSizeBytes - m_MarkerTop);
			return nullptr;
		}

		m_MarkerBottom += effectiveSize;
		void* alignedPtr = reinterpret_cast<void*>(unalignedPtr - offset);
		return alignedPtr;
	}

	StackAllocatorDoubleEnded::Marker StackAllocatorDoubleEnded::getMarkerTop()
	{
		return m_MarkerTop;
	}

	StackAllocatorDoubleEnded::Marker StackAllocatorDoubleEnded::getMarkerBottom()
	{
		return m_MarkerBottom;
	}

	bool StackAllocatorDoubleEnded::freeToMarkerTop(Marker marker)
	{
		if (marker >= 0 && marker <= m_StackSizeBytes - m_MarkerBottom)
		{
			m_MarkerTop = marker;
			return true;
		}
		return false;
	}

	bool StackAllocatorDoubleEnded::freeToMarkerBottom(Marker marker)
	{
		if (marker <= m_StackSizeBytes && marker >= m_MarkerTop)
		{
			m_MarkerBottom = marker;
			return true;
		}
		return false;
	}

	void StackAllocatorDoubleEnded::clear()
	{
		m_MarkerTop = 0;
		m_MarkerBottom = 0;

	}

	void StackAllocatorDoubleEnded::clearTop()
	{
		m_MarkerTop = 0;
	}

	void StackAllocatorDoubleEnded::clearBottom()
	{
		m_MarkerBottom = 0;
	}

	U32 StackAllocatorDoubleEnded::getUnusedBytes()
	{
		return m_StackSizeBytes - m_MarkerTop - m_MarkerBottom;
	}

	U32 StackAllocatorDoubleEnded::getUsedBytes()
	{
		return m_MarkerTop + m_MarkerBottom;
	}

	U32 StackAllocatorDoubleEnded::getUsedBytesTop()
	{
		return m_MarkerTop;
	}

	U32 StackAllocatorDoubleEnded::getUsedBytesBottom()
	{
		return m_MarkerBottom;
	}

	U32 StackAllocatorDoubleEnded::getTotalBytes()
	{
		return m_StackSizeBytes;
	}
}
