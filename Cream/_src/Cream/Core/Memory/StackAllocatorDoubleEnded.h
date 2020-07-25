#pragma once
#include "Cream/Core/Base.h"

namespace Cream
{
	class StackAllocatorDoubleEnded
	{
	public:
		typedef U32 Marker;

		explicit StackAllocatorDoubleEnded(U32 sizeBytes);
		~StackAllocatorDoubleEnded();

		// Allocates a new block of the given size from stack top
		void* allocTop(U32 sizeBytes);
		void* allocTop(U32 sizeBytes, U32 alignment);

		// Allocates a new block of the given size from stack bottom
		void* allocBottom(U32 sizeBytes);
		void* allocBottom(U32 sizeBytes, U32 alignment);

		// Returns a marker to the current top of the stack
		Marker getMarkerTop();
		Marker getMarkerBottom();

		// Rolls the stack back to a previous marker
		// Returns a bool indicating succes
		bool freeToMarkerTop(Marker marker);
		bool freeToMarkerBottom(Marker marker);

		// Clears entire stack (Rolls back to zero)
		void clear();
		void clearTop();
		void clearBottom();

		// Returns unused number of bytes
		U32 getUnusedBytes();

		// Returns used number of bytes
		U32 getUsedBytes();
		U32 getUsedBytesTop();
		U32 getUsedBytesBottom();

		// Returns total memory in bytes allocated by the Stack Allocator
		U32 getTotalBytes();

	private:
		U32 m_StackSizeBytes;
		Marker m_MarkerTop;
		Marker m_MarkerBottom;
		intptr_t m_Pointer;
	};
}