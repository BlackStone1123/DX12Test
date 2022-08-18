#pragma once
#include "Helpers.h"
#include "Bindable.h"

class Surface
{
public:
	Surface(const std::wstring& filename, bool sRGB);
	virtual ~Surface();

	void Upload(Graphics& gfx, D3D12_CPU_DESCRIPTOR_HANDLE handle);
private:
	ComPtr<ID3D12Resource> mIntermediateBuffer;
	ComPtr<ID3D12Resource> mTexureBuffer;
	std::wstring mFilename;
};

class Material : public Bindable
			   , public Buffer
{
public:
	Material(Graphics& gfx);
	virtual ~Material();

	void AddTexture(std::unique_ptr<Surface>);
	virtual void Bind(Graphics& gfx) override;
	virtual void Upload(Graphics& gfx) override;

private:
	std::vector<std::unique_ptr<Surface>> mTextures;
	CD3DX12_CPU_DESCRIPTOR_HANDLE mCpuHandle;
	UINT mSize;
	UINT mCurrentTex{ 0 };
};