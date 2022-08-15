#include "GraphicContext.h"

ComPtr<ID3D12GraphicsCommandList2> GraphicContext::GetCommandList(Graphics& gfx)
{
	return gfx.GetDrawCommandList();
}

ComPtr<ID3D12Device2> GraphicContext::GetDevice(Graphics& gfx)
{
	return gfx.mDevice;
}

ComPtr<ID3D12GraphicsCommandList2> GraphicContext::GetCopyCommandList(Graphics& gfx)
{
	return gfx.GetCopyCommandList();
}