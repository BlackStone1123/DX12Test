#pragma once

#include "CommonHeaders.h"
#include <queue>

class CommandQueue
{
public:
	CommandQueue(ComPtr<ID3D12Device2> device, D3D12_COMMAND_LIST_TYPE type);
	virtual ~CommandQueue();

	ComPtr<ID3D12CommandQueue> GetD3D12CommandQueue() const;
	ComPtr<ID3D12GraphicsCommandList2> GetCommandList(bool reset = true);

	uint64_t ExecuteCommandList(ComPtr<ID3D12GraphicsCommandList2> commandList);

	uint64_t Signal();
	bool     IsFenceComplete(uint64_t fenceValue);
	void     WaitForFenceValue(uint64_t fenceValue);
	void     Flush();

private:
	// Keep track of command allocators that are "in-flight"
	struct CommandAllocatorEntry
	{
		uint64_t fenceValue;
		ComPtr<ID3D12CommandAllocator> commandAllocator;
	};

	using CommandAllocatorQueue = std::queue<CommandAllocatorEntry>;
	using CommandListQueue = std::queue< ComPtr<ID3D12GraphicsCommandList2> >;

	ComPtr<ID3D12Device2> m_Device;
	ComPtr<ID3D12CommandQueue> m_d3d12CommandQueue;
	ComPtr<ID3D12Fence> m_d3d12Fence;

	D3D12_COMMAND_LIST_TYPE m_CommandListType;
	uint64_t m_FenceValue{ 0 };
	HANDLE m_FenceEvent;

	CommandAllocatorQueue m_AllocatorQueue;
	ComPtr<ID3D12GraphicsCommandList2> m_CommandList;
};