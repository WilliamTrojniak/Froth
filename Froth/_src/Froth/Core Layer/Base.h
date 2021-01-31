#pragma once
// Platform Independent Includes
#include "Froth/Platform Independence Layer/BuildDefines.h"
#include "Froth/Platform Independence Layer/PointerTypes.h"
#include "Froth/Platform Independence Layer/PrimitiveTypes.h"
#include "Froth/Platform Independence Layer/File System/File.h"


#ifdef FROTH_DEBUG
#include "Froth/Platform Independence Layer/Profiling/InstrumentationTimer.h"
#endif // FROTH_DEBUG


// Core Systems
#include "Froth/Core Layer/Assertions.h"
#include "Froth/Core Layer/Memory/DoubleBufferedAllocator.h"
#include "Froth/Core Layer/Memory/PoolAllocator.h"
#include "Froth/Core Layer/Memory/StackAllocator.h"
#include "Froth/Core Layer/Memory/StackAllocatorDoubleEnded.h"