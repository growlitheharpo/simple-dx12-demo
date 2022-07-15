#pragma once

#include "gfx/core/fwd.h"

#define DECLARE_EQUIVALENT_ENUM(x, y)       \
	constexpr bool operator==(x lhs, y rhs) \
	{                                       \
		return int(lhs) == int(rhs);        \
	}                                       \
	constexpr bool operator==(y lhs, x rhs) \
	{                                       \
		return int(lhs) == int(rhs);        \
	}

#define TEST_EQUIVALENT_ENUM(x, y) static_assert(x == y, "Enum values are not equal! Casting will fail.")

#define ALLOW_GFX_ACCESS()       \
	ALL_GFX_CLASSES(friend class);
