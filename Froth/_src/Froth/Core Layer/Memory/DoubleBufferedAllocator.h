#pragma once
#include "Froth/Core Layer/Memory/StackAllocator.h"

namespace Froth
{
	class DoubleBufferedAllocator
	{
	public:
		DoubleBufferedAllocator(U32 stackSizeBytes);

		// Swap the active buffer
		void swapBuffers();

		// Clear the currently active buffer
		void clearCurrentBuffer();

		// Allocate data on the currently active buffer
		void* alloc(U32 nBytes);
		void* alloc(U32 nBytes, U32 alignment);

	private:
		U32 m_CurrentStack = 0;
		U32 m_StackSizeBytes;
		StackAllocator m_Stacks[2] = { StackAllocator(m_StackSizeBytes), StackAllocator(m_StackSizeBytes) };
	};
}