#pragma once

// IN_USE and NOT_IN_USE are defined globally in premake.
// #define IN_USE 2
// #define NOT_IN_USE 4

#define USING(x) ((IN_USE / x) == 1)

// Add features here

#define DX12_DEBUG_LAYER DEBUGGING
