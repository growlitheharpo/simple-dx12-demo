#pragma once

#define DECLARE_EQUIVALENT_ENUM(x, y)       \
	constexpr bool operator==(x lhs, y rhs) \
	{                                       \
		return int(lhs) == int(rhs);        \
	}

#define TEST_EQUIVALENT_ENUM(x, y) static_assert(x == y, "Enum values are not equal! Casting will fail.")
