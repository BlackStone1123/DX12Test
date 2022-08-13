#pragma once
#include "Bindable.h"

class Shader : public Bindable
{
public:
	Shader( Graphics& gfx,const std::wstring& path );
	void Bind( Graphics& gfx ) override;
	ComPtr<ID3DBlob> GetBytecode() const;
protected:
	ComPtr<ID3DBlob> pBytecodeBlob;
};