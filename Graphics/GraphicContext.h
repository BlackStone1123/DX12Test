#pragma once

#include "CommonHeaders.h"
#include "Graphics.h"

class GraphicContext
{
public:
    static ComPtr<ID3D12GraphicsCommandList2> GetCommandList(Graphics& gfx);
    static ComPtr<ID3D12Device2> GetDevice(Graphics& gfx);
    static ComPtr<ID3D12GraphicsCommandList2> GetCopyCommandList(Graphics& gfx);
};