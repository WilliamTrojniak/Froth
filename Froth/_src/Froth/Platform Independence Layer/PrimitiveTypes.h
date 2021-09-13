#pragma once

#define BITFIELD(x) (1<<x)

// Unsigned Integers
using	U8	= uint8_t;
using	U16	= uint16_t;
using	U32 = uint32_t;
using	U64 = uint64_t;

// Integers
using I8  = int8_t;
using I16 = int16_t;
using I32 = int32_t;
using I64 = int64_t;

// Fast Unsigned Integers
using U8F  = uint_fast8_t;
using U16F = uint_fast16_t;
using U32F = uint_fast32_t;
using U64F = uint_fast64_t;

// Fast Integers
using I8F  = int_fast8_t;
using I16F = int_fast16_t;
using I32F = int_fast32_t;
using I64F = int_fast64_t;

// Floats
using F32 = float;