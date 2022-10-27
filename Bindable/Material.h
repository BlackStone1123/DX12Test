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
	           , public Bindable
{
public:
	Material(Graphics& gfx, UINT textureCount);
	virtual ~Material();

	void AddTexture(std::unique_ptr<Texture>);
	virtual void Upload(Graphics& gfx) override;
	virtual void Bind(Graphics& gfx) override;

	HeapResource& source() { return mSource; }

private:
	std::vector<std::unique_ptr<Texture>> mTextures;
	HeapResource mSource;
	UINT mCount;
};