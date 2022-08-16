#pragma once
#include "Helpers.h"
#include "Bindable.h"

class Surface: public Bindable
			 , public Buffer
{
public:
	Surface(Graphics& gfx, const std::wstring& filename, bool sRGB);
	virtual ~Surface();

	virtual void Bind(Graphics& gfx) override;
	virtual void Upload(Graphics& gfx) override;
private:
	D3D12_CPU_DESCRIPTOR_HANDLE mCpuHandle;
	D3D12_GPU_DESCRIPTOR_HANDLE mGpuHandle;

	ComPtr<ID3D12Resource> mIntermediateBuffer;
	ComPtr<ID3D12Resource> mTexureBuffer;
	std::wstring mFilename;
};
