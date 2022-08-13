#pragma once
#include "Bindable.h"

class RootSignature : public Bindable
{
public:
	RootSignature( Graphics& gfx);
	void Bind( Graphics& gfx ) override;
	ComPtr<ID3D12RootSignature> GetSignature() 
	{
		return m_RootSignature;
	}

private:
	// Root signature
	ComPtr<ID3D12RootSignature> m_RootSignature;
};