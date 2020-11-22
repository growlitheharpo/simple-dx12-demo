#pragma once

// Equivalent to D3D12_COMMAND_LIST_TYPE
enum class CommandQueueType : int
{
	Direct,
	Bundle,
	Compute,
	Copy,
	VideoDecode,
	VideoProcess,
	VideoEncode,
};
