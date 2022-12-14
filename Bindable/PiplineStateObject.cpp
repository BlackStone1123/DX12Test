#include "PiplineStateObject.h"
#include "GraphicContext.h"

PiplineStateObject::PiplineStateObject( 
	Graphics& gfx,
    const std::vector<D3D12_INPUT_ELEMENT_DESC>& inputLayout,
	ComPtr<ID3D12RootSignature> signature,
	ComPtr<ID3DBlob> vertexShader,
	ComPtr<ID3DBlob> pixelShader,
    StencilMode mode) : mMode(mode)
{
    struct PipelineStateStream
    {
        CD3DX12_PIPELINE_STATE_STREAM_ROOT_SIGNATURE pRootSignature;
        CD3DX12_PIPELINE_STATE_STREAM_INPUT_LAYOUT InputLayout;
        CD3DX12_PIPELINE_STATE_STREAM_PRIMITIVE_TOPOLOGY PrimitiveTopologyType;
        CD3DX12_PIPELINE_STATE_STREAM_VS VS;
        CD3DX12_PIPELINE_STATE_STREAM_PS PS;
        CD3DX12_PIPELINE_STATE_STREAM_DEPTH_STENCIL_FORMAT DSVFormat;
        CD3DX12_PIPELINE_STATE_STREAM_RENDER_TARGET_FORMATS RTVFormats;
        CD3DX12_PIPELINE_STATE_STREAM_DEPTH_STENCIL DepthStensil;
    } pipelineStateStream;

    D3D12_RT_FORMAT_ARRAY rtvFormats = {};
    rtvFormats.NumRenderTargets = 3;
    rtvFormats.RTFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;

    pipelineStateStream.pRootSignature = signature.Get();
    pipelineStateStream.InputLayout = { inputLayout.data() , (UINT)inputLayout.size()};
    pipelineStateStream.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
    pipelineStateStream.VS = CD3DX12_SHADER_BYTECODE(vertexShader.Get());
    pipelineStateStream.PS = CD3DX12_SHADER_BYTECODE(pixelShader.Get());
    pipelineStateStream.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
    pipelineStateStream.RTVFormats = rtvFormats;

    CD3DX12_DEPTH_STENCIL_DESC dsDesc = CD3DX12_DEPTH_STENCIL_DESC{ CD3DX12_DEFAULT{} };
    if (mode == StencilMode::Write)
    {
        dsDesc.StencilEnable = TRUE;
        dsDesc.StencilWriteMask = 0xFF;
        dsDesc.FrontFace.StencilFunc = D3D12_COMPARISON_FUNC_ALWAYS;
        dsDesc.FrontFace.StencilPassOp = D3D12_STENCIL_OP_REPLACE;
    }
    else if (mode == StencilMode::Mask)
    {
        dsDesc.StencilEnable = TRUE;
        dsDesc.StencilReadMask = 0xFF;
        dsDesc.FrontFace.StencilFunc = D3D12_COMPARISON_FUNC_NOT_EQUAL;
        dsDesc.FrontFace.StencilPassOp = D3D12_STENCIL_OP_KEEP;
    }

    pipelineStateStream.DepthStensil = dsDesc;

    D3D12_PIPELINE_STATE_STREAM_DESC pipelineStateStreamDesc = {
        sizeof(PipelineStateStream), &pipelineStateStream
    };

    auto device = GraphicContext::GetDevice(gfx);
    ThrowIfFailed(device->CreatePipelineState(&pipelineStateStreamDesc, IID_PPV_ARGS(&m_PipelineState)));
}

void PiplineStateObject::Bind( Graphics& gfx )
{
    GraphicContext::GetCommandList( gfx )->SetPipelineState(m_PipelineState.Get());

    if (mMode != StencilMode::Off)
    {
        GraphicContext::GetCommandList(gfx)->OMSetStencilRef(0xff);
    }
}
