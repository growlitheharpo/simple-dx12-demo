#pragma once

#include "types.h"

enum class DescriptorHeapType : int32
{
	Cbv_Srv_Uav,
	Sampler,
	RTV,
	DSV,

	Count,
};
