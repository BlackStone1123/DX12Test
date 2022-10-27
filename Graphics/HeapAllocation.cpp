#include "HeapAllocation.h"

HeapResource::HeapResource(
	CD3DX12_CPU_DESCRIPTOR_HANDLE cpuHandle,
	CD3DX12_GPU_DESCRIPTOR_HANDLE gpuHandle,
	UINT entrySize,
	UINT count)
	: mCpuHandle(cpuHandle)
	, mGpuHandle(gpuHandle)
	, mEmptryEntrySize(count)
	, mEntrySize(entrySize)
{

}

HeapAllocation HeapResource::RequestResource(UINT count)
{
	assert("no enough space for requested source number" && mEmptryEntrySize >= count);

	HeapAllocation alloc = { mCpuHandle, mEntrySize };
	mEmptryEntrySize -= count;
	mCpuHandle.Offset(count, mEntrySize);

	return alloc;
}

D3D12_GPU_DESCRIPTOR_HANDLE HeapResource::GetResourceLocation() const
{
	return mGpuHandle;
}

void HeapResource::SetBindSlot(UINT slot)
{
	mSlot = slot;
}

UINT HeapResource::GetBindSlot() const
{
	return mSlot;
}