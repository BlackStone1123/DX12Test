#pragma once

#include "Helpers.h"
#include "CommonHeaders.h"

class GraphicsUtils
{
public:
	static ComPtr<IDXGIAdapter4> GetAdapter();
	static ComPtr<ID3D12Device2> CreateDevice(ComPtr<IDXGIAdapter4> adapter);
	static ComPtr<ID3D12CommandQueue> CreateCommandQueue(ComPtr<ID3D12Device2> device, D3D12_COMMAND_LIST_TYPE type);
	static ComPtr<IDXGISwapChain4> CreateSwapChain(HWND hWnd,
		ComPtr<ID3D12CommandQueue> commandQueue,
		uint32_t width, uint32_t height, uint32_t bufferCount);
	static ComPtr<ID3D12DescriptorHeap> CreateDescriptorHeap(ComPtr<ID3D12Device2> device,
		D3D12_DESCRIPTOR_HEAP_TYPE type, uint32_t numDescriptors, D3D12_DESCRIPTOR_HEAP_FLAGS flags = D3D12_DESCRIPTOR_HEAP_FLAGS::D3D12_DESCRIPTOR_HEAP_FLAG_NONE);
	static ComPtr<ID3D12CommandAllocator> CreateCommandAllocator(ComPtr<ID3D12Device2> device,
		D3D12_COMMAND_LIST_TYPE type);
	static ComPtr<ID3D12GraphicsCommandList2> CreateCommandList(ComPtr<ID3D12Device2> device,
		ComPtr<ID3D12CommandAllocator> commandAllocator, D3D12_COMMAND_LIST_TYPE type);
	static ComPtr<ID3D12Fence> CreateFence(ComPtr<ID3D12Device2> device);
	static HANDLE CreateEventHandle();
	static bool CheckTearingSupport();
	static void EnableDebugLayer();
	static bool IsFenceComplete(uint64_t fenceValue, ComPtr<ID3D12Fence> fenceObj);

	static void UpdateBufferResource
	(
		ComPtr<ID3D12Device2> device,
		ComPtr<ID3D12GraphicsCommandList2> commandList,
		ID3D12Resource** pDestinationResource,
		ID3D12Resource** pIntermediateResource,
		size_t numElements, size_t elementSize, const void* bufferData,
		D3D12_RESOURCE_FLAGS flags = D3D12_RESOURCE_FLAG_NONE
	);
};