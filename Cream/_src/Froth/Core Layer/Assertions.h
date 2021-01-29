#pragma once
#include "Froth/Platform Independence Layer/BuildDefines.h"

#if defined(FROTH_ENABLE_ASSERTIONS)
#ifndef FROTH_DEBUGBREAK
#error "Platform doesn't support debugbreak yet!"
#endif
#define FROTH_ASSERT(expr) if(expr){}else{ FROTH_DEBUGBREAK();}
#else
#define FROTH_ASSERT
#endif