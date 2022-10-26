#pragma once
#include "Helpers.h"
#include "Bindable.h"

class Texture
{
public:
	Texture(const std::wstring& filename, bool sRGB);
	virtual ~Texture();

	void Upload(Graphics& gfx);
	void Bind(Graphics& gfx, D3D12_CPU_DESCRIPTOR_HANDLE handle);
private:
	ComPtr<ID3D12Resource> mIntermediateBuffer;
	ComPtr<ID3D12Resource> mTexureBuffer;
	std::wstring mFilename;
};

class Material : public Resource
{
public:
	Material(Graphics& gfx,const HeapAllocation& location);
	virtual ~Material();

	void AddTexture(std::unique_ptr<Texture>);
	virtual void Upload(Graphics& gfx) override;

private:
	std::vector<std::unique_ptr<Texture>> mTextures;
	HeapAllocation mLocation;
};