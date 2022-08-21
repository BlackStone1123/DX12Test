#pragma once
#include "CommonHeaders.h"

struct HeapAllocation
{
	CD3DX12_CPU_DESCRIPTOR_HANDLE cpuHandle;
	UINT entrySize{ 0 };

	HeapAllocation operator++(int)
	{
		HeapAllocation curr = { this->cpuHandle, entrySize };
		this->cpuHandle.Offset(entrySize);
		return curr;
	}
};

class HeapResource
{
public:
	HeapResource() = default;
	HeapResource(
		CD3DX12_CPU_DESCRIPTOR_HANDLE cpuHandle,
		CD3DX12_GPU_DESCRIPTOR_HANDLE gpuHandle,
		UINT entrySize,
		UINT count
	);
	HeapAllocation RequestResource(UINT count);
	D3D12_GPU_DESCRIPTOR_HANDLE GetResourceLocation() const;

private:
	CD3DX12_GPU_DESCRIPTOR_HANDLE mGpuHandle;
	CD3DX12_CPU_DESCRIPTOR_HANDLE mCpuHandle;
	UINT mEmptryEntrySize{ 0 };
	UINT mEntrySize;
};