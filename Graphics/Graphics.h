#pragma once

#include "Helpers.h"
#include "CommonHeaders.h"

// STD header files
#include <vector>
#include <chrono>

class CommandQueue;
class Graphics
{
	friend class GraphicContext;
public:
	Graphics(HWND hwnd, UINT width, UINT height);
	Graphics(const Graphics&) = delete;
	Graphics& operator=(const Graphics&) = delete;
	~Graphics();

	void BeginFrame(float r, float g, float b, float a = 1.0f);
	void EndFrame();
	void Resize(UINT width, UINT height);
	void DrawIndexed(UINT count);

	DirectX::XMMATRIX GetProjection() const;

private:

	void Flush();
	void UpdateRenderTargetViews(UINT frameCount);
	void ResizeDepthBuffer();
	void ResizeRenderTarget();

	ComPtr<ID3D12GraphicsCommandList2> getCopyCommandList();
	ComPtr<ID3D12GraphicsCommandList2> getDrawCommandList();

private:
	// DirectX 12 Objects
	ComPtr<ID3D12Device2> mDevice;
	ComPtr<IDXGISwapChain4> mSwapChain;
	std::vector<ComPtr<ID3D12Resource>> mBackBuffers;

	// Desc Heap
	ComPtr<ID3D12DescriptorHeap> mRTVDescriptorHeap;
	ComPtr<ID3D12DescriptorHeap> mDSVDescriptorHeap;
	ComPtr<ID3D12DescriptorHeap> mSRVDescriptorHeap;

	// Command Queue
	std::unique_ptr<CommandQueue> mCopyCommandQueue;
	std::unique_ptr<CommandQueue> mDrawCommandQueue;

	// Depth buffer.
	ComPtr<ID3D12Resource> m_DepthBuffer;

	D3D12_VIEWPORT m_Viewport;
	D3D12_RECT m_ScissorRect;

	// Scalar Numbers
	UINT mRTVDescriptorSize;
	UINT mCurrentBackBufferIndex;
	UINT mNumFrames{ 3 };
	UINT mWidth{ 0 };
	UINT mHeight{ 0 };

	// Flags
	bool vSync{ true };
	bool mTearingSupported{ false };
};