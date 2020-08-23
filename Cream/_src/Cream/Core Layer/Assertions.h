#pragma once
#include "Cream/Platform Independence Layer/BuildDefines.h"

#if defined(CREAM_ENABLE_ASSERTIONS)
#ifndef CREAM_DEBUGBREAK
#error "Platform doesn't support debugbreak yet!"
#endif
#define CREAM_ASSERT(expr) if(expr){}else{ CREAM_DEBUGBREAK();}
#else
#define CREAM_ASSERT
#endif