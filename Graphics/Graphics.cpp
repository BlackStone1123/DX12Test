#include "Graphics.h"
#include "GraphicsUtils.h"
#include "CommandQueue.h"

Graphics::Graphics(HWND hwnd, UINT width, UINT height)
    : mWidth(width)
    , mHeight(height)
    , m_ScissorRect(CD3DX12_RECT(0, 0, LONG_MAX, LONG_MAX))
    , m_Viewport(CD3DX12_VIEWPORT(0.0f, 0.0f, static_cast<float>(width), static_cast<float>(height)))
{
    GraphicsUtils::EnableDebugLayer();
    auto adapter = GraphicsUtils::GetAdapter();
    mDevice = GraphicsUtils::CreateDevice(adapter);

    mDrawCommandQueue = std::make_unique<CommandQueue>(mDevice, D3D12_COMMAND_LIST_TYPE_DIRECT);
    mCopyCommandQueue = std::make_unique<CommandQueue>(mDevice, D3D12_COMMAND_LIST_TYPE_COPY);

    mSwapChain = GraphicsUtils::CreateSwapChain(hwnd, mDrawCommandQueue->GetD3D12CommandQueue(), width, height, mNumFrames);
    mCurrentBackBufferIndex = mSwapChain->GetCurrentBackBufferIndex();

    mRTVDescriptorHeap = GraphicsUtils::CreateDescriptorHeap(mDevice, D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_RTV, mNumFrames);
    mRTVDescriptorSize = mDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
    mDSVDescriptorHeap = GraphicsUtils::CreateDescriptorHeap(mDevice, D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_DSV, 1);

    UpdateRenderTargetViews(mNumFrames);
    ResizeDepthBuffer();

    mTearingSupported = GraphicsUtils::CheckTearingSupport();
}

Graphics::~Graphics()
{
    Flush();
}

void Graphics::Flush()
{
    mDrawCommandQueue->Flush();
    mCopyCommandQueue->Flush();
}

void Graphics::UpdateRenderTargetViews(UINT frameCount)
{
    mBackBuffers.resize(frameCount);

    CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(mRTVDescriptorHeap->GetCPUDescriptorHandleForHeapStart());

    for (size_t i = 0; i < frameCount; ++i)
    {
        ComPtr<ID3D12Resource> backBuffer;
        ThrowIfFailed(mSwapChain->GetBuffer(i, IID_PPV_ARGS(&backBuffer)));

        mDevice->CreateRenderTargetView(backBuffer.Get(), nullptr, rtvHandle);

        mBackBuffers[i] = backBuffer;

        rtvHandle.Offset(mRTVDescriptorSize);
    }
}

void Graphics::BeginFrame(float r, float g, float b, float a)
{
    auto commandList = mDrawCommandQueue->GetCommandList(true);
    mCopyCommandQueue->GetCommandList(true);

    auto backBuffer = mBackBuffers[mCurrentBackBufferIndex];

    // Clear the render target.
    CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition
    (
        backBuffer.Get(),
        D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET
    );

    commandList->ResourceBarrier(1, &barrier);
    FLOAT clearColor[] = { r, g, b, a };

    CD3DX12_CPU_DESCRIPTOR_HANDLE rtv(mRTVDescriptorHeap->GetCPUDescriptorHandleForHeapStart(), mCurrentBackBufferIndex, mRTVDescriptorSize);
    CD3DX12_CPU_DESCRIPTOR_HANDLE dsv(mDSVDescriptorHeap->GetCPUDescriptorHandleForHeapStart());

    commandList->ClearRenderTargetView(rtv, clearColor, 0, nullptr);
    commandList->ClearDepthStencilView(dsv, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

    commandList->RSSetViewports(1, &m_Viewport);
    commandList->RSSetScissorRects(1, &m_ScissorRect);
    commandList->OMSetRenderTargets(1, &rtv, FALSE, &dsv);
}

void Graphics::EndFrame()
{
    auto commandList = getDrawCommandList();
    auto backBuffer = mBackBuffers[mCurrentBackBufferIndex];

    CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition
    (
        backBuffer.Get(),
        D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT
    );
    commandList->ResourceBarrier(1, &barrier);

    mCopyCommandQueue->ExecuteCommandList(getCopyCommandList());
    mCopyCommandQueue->Flush();

    mDrawCommandQueue->ExecuteCommandList(commandList);

    UINT syncInterval = vSync ? 1 : 0;
    UINT presentFlags = mTearingSupported && !vSync ? DXGI_PRESENT_ALLOW_TEARING : 0;
    ThrowIfFailed(mSwapChain->Present(syncInterval, presentFlags));

    mCurrentBackBufferIndex = mSwapChain->GetCurrentBackBufferIndex();
}

void Graphics::Resize(UINT width, UINT height)
{
    if (mWidth != width || mHeight != height)
    {
        // Don't allow 0 size swap chain back buffers.
        mWidth = std::max(1u, width);
        mHeight = std::max(1u, height);
        m_Viewport = CD3DX12_VIEWPORT(0.0f, 0.0f,
            static_cast<float>(width), static_cast<float>(height));

        // Flush the GPU queue to make sure the swap chain's back buffers
        // are not being referenced by an in-flight command list.
        Flush();
        ResizeRenderTarget();
        ResizeDepthBuffer();
    }
}

void Graphics::ResizeDepthBuffer()
{
    D3D12_CLEAR_VALUE optimizedClearValue = {};
    optimizedClearValue.Format = DXGI_FORMAT_D32_FLOAT;
    optimizedClearValue.DepthStencil = { 1.0f, 0 };

    ThrowIfFailed(mDevice->CreateCommittedResource(
        &CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
        D3D12_HEAP_FLAG_NONE,
        &CD3DX12_RESOURCE_DESC::Tex2D(DXGI_FORMAT_D32_FLOAT, mWidth, mHeight,
            1, 0, 1, 0, D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL),
        D3D12_RESOURCE_STATE_DEPTH_WRITE,
        &optimizedClearValue,
        IID_PPV_ARGS(&m_DepthBuffer)
    ));

    // Update the depth-stencil view.
    D3D12_DEPTH_STENCIL_VIEW_DESC dsv = {};
    dsv.Format = DXGI_FORMAT_D32_FLOAT;
    dsv.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
    dsv.Texture2D.MipSlice = 0;
    dsv.Flags = D3D12_DSV_FLAG_NONE;

    mDevice->CreateDepthStencilView(m_DepthBuffer.Get(), &dsv,
        mDSVDescriptorHeap->GetCPUDescriptorHandleForHeapStart());
}

void Graphics::ResizeRenderTarget()
{
    for (int i = 0; i < mNumFrames; ++i)
    {
        mBackBuffers[i].Reset();
    }
    DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
    ThrowIfFailed(mSwapChain->GetDesc(&swapChainDesc));
    ThrowIfFailed(mSwapChain->ResizeBuffers(mNumFrames, mWidth, mHeight,
        swapChainDesc.BufferDesc.Format, swapChainDesc.Flags));

    mCurrentBackBufferIndex = mSwapChain->GetCurrentBackBufferIndex();

    UpdateRenderTargetViews(mNumFrames);
}

void Graphics::DrawIndexed(UINT count)
{
    getDrawCommandList()->DrawIndexedInstanced(count, 1, 0, 0, 0);
}

ComPtr<ID3D12GraphicsCommandList2> Graphics::getCopyCommandList()
{
    return mCopyCommandQueue->GetCommandList(false);
}

ComPtr<ID3D12GraphicsCommandList2> Graphics::getDrawCommandList()
{
    return mDrawCommandQueue->GetCommandList(false);
}

DirectX::XMMATRIX Graphics::GetProjection() const
{
    auto project = DirectX::XMMatrixPerspectiveFovLH(0.6f, 1, 0.1f, 500.0f);

    const DirectX::XMVECTOR eyePosition = DirectX::XMVectorSet(0, 0, -20, 1);
    const DirectX::XMVECTOR focusPoint = DirectX::XMVectorSet(0, 0, 0, 1);
    const DirectX::XMVECTOR upDirection = DirectX::XMVectorSet(0, 1, 0, 0);
    auto view = DirectX::XMMatrixLookAtLH(eyePosition, focusPoint, upDirection);
    return  view * project * DirectX::XMMatrixScaling((float)mHeight / mWidth, 1.0f, 1.0f);
}