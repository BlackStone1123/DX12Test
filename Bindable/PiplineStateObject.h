#pragma once
#include "Bindable.h"

enum class StencilMode 
{
	Off,
	Write,
	Mask
};

class PiplineStateObject : public Bindable
{
public:
	PiplineStateObject( 
		Graphics& gfx,
		const std::vector<D3D12_INPUT_ELEMENT_DESC>& inputLayout,
		ComPtr<ID3D12RootSignature> signature,
		ComPtr<ID3DBlob> vertexShader,
		ComPtr<ID3DBlob> pixelShader,
		StencilMode mode = StencilMode::Off
	);

	virtual void Bind( Graphics& gfx ) override;
protected:
	// Pipeline state object.
	ComPtr<ID3D12PipelineState> m_PipelineState;
	StencilMode mMode;
};