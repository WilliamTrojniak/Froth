#pragma once
#include "Cream/Core/Base.h"

namespace Cream
{

	// For dynamic memory allocations in a LIFO manner
	class StackAllocator
	{
	public:

		// Represents current top of the stack
		typedef U32 Marker;

		explicit StackAllocator(U32 stackSizeBytes);
		~StackAllocator();

		// Allocates a new block of the given size from stack top
		void* alloc(U32 sizeBytes);
		void* alloc(U32 sizeBytes, U32 alignment);

		// Returns a marker to the current top of the stack
		Marker getMarker();

		// Rolls the stack back to a previous marker
		bool freeToMarker(Marker marker);

		// Clears entire stack (Rolls back to zero)
		void clear();

		// Returns unused number of bytes
		U32 getUnusedBytes();

		// Returns used number of bytes
		U32 getUsedBytes();

		// Returns total memory in bytes allocated by the Stack Allocator
		U32 getTotalBytes();

	private:
		U32 m_StackSizeBytes;
		Marker m_Marker;
		intptr_t m_Pointer;
	};
}

