#include "CommandQueue.h"
#include "GraphicsUtils.h"

CommandQueue::CommandQueue(ComPtr<ID3D12Device2> device, D3D12_COMMAND_LIST_TYPE type)
	: m_Device(device), m_CommandListType(type)
{
	m_d3d12CommandQueue = GraphicsUtils::CreateCommandQueue(device, type);
	m_d3d12Fence = GraphicsUtils::CreateFence(device);
	m_FenceEvent = GraphicsUtils::CreateEventHandle();
	m_d3d12CommandQueue->SetName(type == D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT ? L"DirectQueue" : L"CopyQueue");
}

CommandQueue::~CommandQueue()
{
	::CloseHandle(m_FenceEvent);
}

ComPtr<ID3D12CommandQueue> CommandQueue::GetD3D12CommandQueue() const
{
	return m_d3d12CommandQueue;
}

ComPtr<ID3D12GraphicsCommandList2> CommandQueue::GetCommandList(bool reset)
{
	if (!reset)
	{
		return m_CommandList;
	}

	Microsoft::WRL::ComPtr<ID3D12CommandAllocator> commandAllocator;

	if (!m_AllocatorQueue.empty() && IsFenceComplete(m_AllocatorQueue.front().fenceValue))
	{
		commandAllocator = m_AllocatorQueue.front().commandAllocator;
		m_AllocatorQueue.pop();

		ThrowIfFailed(commandAllocator->Reset());
	}
	else
	{
		commandAllocator = GraphicsUtils::CreateCommandAllocator(m_Device, m_CommandListType);
	}

	if (m_CommandList == nullptr)
	{
		m_CommandList = GraphicsUtils::CreateCommandList(m_Device, commandAllocator, m_CommandListType);
		m_CommandList->SetName(m_CommandListType == D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT ? L"DirectList" : L"CopyList");
	}

	ThrowIfFailed(m_CommandList->Reset(commandAllocator.Get(), nullptr));
	ThrowIfFailed(m_CommandList->SetPrivateDataInterface(__uuidof(ID3D12CommandAllocator), commandAllocator.Get()));

	return m_CommandList;
}

uint64_t CommandQueue::ExecuteCommandList(ComPtr<ID3D12GraphicsCommandList2> commandList)
{
	ThrowIfFailed(commandList->Close());
	ID3D12CommandList* const commandLists[] = { commandList.Get() };

	ID3D12CommandAllocator* commandAllocator;
	UINT dataSize = sizeof(commandAllocator);
	ThrowIfFailed(commandList->GetPrivateData(__uuidof(ID3D12CommandAllocator), &dataSize, &commandAllocator));

	m_d3d12CommandQueue->ExecuteCommandLists(1, commandLists);
	uint64_t fenceValue = Signal();

	m_AllocatorQueue.emplace(CommandAllocatorEntry{ fenceValue, commandAllocator });

	commandAllocator->Release();

	return fenceValue;
}

uint64_t CommandQueue::Signal()
{
	uint64_t fenceValueForSignal = ++m_FenceValue;
	ThrowIfFailed(m_d3d12CommandQueue->Signal(m_d3d12Fence.Get(), fenceValueForSignal));

	return fenceValueForSignal;
}

bool CommandQueue::IsFenceComplete(uint64_t fenceValue)
{
	return GraphicsUtils::IsFenceComplete(fenceValue, m_d3d12Fence);
}

void CommandQueue::WaitForFenceValue(uint64_t fenceValue)
{
	if (!IsFenceComplete(fenceValue))
	{
		if (m_FenceEvent)
		{
			// Is this function thread safe?
			m_d3d12Fence->SetEventOnCompletion(fenceValue, m_FenceEvent);
			::WaitForSingleObject(m_FenceEvent, DWORD_MAX);
		}
	}
}

void CommandQueue::Flush()
{
	WaitForFenceValue(m_FenceValue);
}