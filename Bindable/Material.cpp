#include "Material.h"
#include "DirectXTex.h"
#include "CommonHeaders.h"
#include "Graphics.h"
#include "GraphicContext.h"
#include <filesystem>
#include "GraphicsUtils.h"
#include "imgui.h"

namespace fs = std::filesystem;
using namespace DirectX;

Texture::Texture(const std::wstring& fileName, bool sRGB)
    :mFilename(fileName)
{
    if (!fs::exists(fs::path(fileName)))
    {
        throw std::exception("File not found.");
    }
}

Texture::~Texture()
{
}

void Texture::Upload(Graphics& gfx)
{
    TexMetadata  metadata;
    ScratchImage scratchImage;
    auto filePath = fs::path(mFilename);

    if (filePath.extension() == ".dds")
    {
        //ThrowIfFailed(LoadFromDDSFile(fileName.c_str(), DDS_FLAGS_FORCE_RGB, &metadata, scratchImage));
    }
    else if (filePath.extension() == ".hdr")
    {
        //ThrowIfFailed(LoadFromHDRFile(fileName.c_str(), &metadata, scratchImage));
    }
    else if (filePath.extension() == ".tga")
    {
        //ThrowIfFailed(LoadFromTGAFile(fileName.c_str(), &metadata, scratchImage));
    }
    else
    {
        ThrowIfFailed(LoadFromWICFile(mFilename.c_str(), WIC_FLAGS_FORCE_RGB, &metadata, scratchImage));
    }

    D3D12_RESOURCE_DESC textureDesc = {};
    switch (metadata.dimension)
    {
    case TEX_DIMENSION_TEXTURE1D:
        textureDesc = CD3DX12_RESOURCE_DESC::Tex1D(metadata.format, static_cast<UINT64>(metadata.width),
            static_cast<UINT16>(metadata.arraySize));
        break;
    case TEX_DIMENSION_TEXTURE2D:
        textureDesc = CD3DX12_RESOURCE_DESC::Tex2D(metadata.format, static_cast<UINT64>(metadata.width),
            static_cast<UINT>(metadata.height),
            static_cast<UINT16>(metadata.arraySize));
        break;
    case TEX_DIMENSION_TEXTURE3D:
        textureDesc = CD3DX12_RESOURCE_DESC::Tex3D(metadata.format, static_cast<UINT64>(metadata.width),
            static_cast<UINT>(metadata.height),
            static_cast<UINT16>(metadata.depth));
        break;
    default:
        throw std::exception("Invalid texture dimension.");
        break;
    }

    ComPtr<ID3D12Device2> d3d12Device = GraphicContext::GetDevice(gfx);

    ThrowIfFailed(d3d12Device->CreateCommittedResource(
        &CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT), D3D12_HEAP_FLAG_NONE, &textureDesc,
        D3D12_RESOURCE_STATE_COMMON, nullptr, IID_PPV_ARGS(&mTexureBuffer)));
    mTexureBuffer->SetName(mFilename.data());

    std::vector<D3D12_SUBRESOURCE_DATA> subresources(scratchImage.GetImageCount());
    const Image* pImages = scratchImage.GetImages();
    for (int i = 0; i < scratchImage.GetImageCount(); ++i)
    {
        auto& subresource = subresources[i];
        subresource.RowPitch = pImages[i].rowPitch;
        subresource.SlicePitch = pImages[i].slicePitch;
        subresource.pData = pImages[i].pixels;
    }

    GraphicsUtils::CopyTextureSubresource(
        d3d12Device,
        GraphicContext::GetCopyCommandList(gfx),
        mTexureBuffer.Get(),
        mIntermediateBuffer.GetAddressOf(),
        0, (UINT)subresources.size(), subresources.data()
    );
}

void Texture::Bind(Graphics& gfx, D3D12_CPU_DESCRIPTOR_HANDLE handle)
{
    ComPtr<ID3D12Device2> d3d12Device = GraphicContext::GetDevice(gfx);

    D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc;
    ZeroMemory(&srvDesc, sizeof(srvDesc));
    srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MipLevels = 1;
    srvDesc.Texture2D.MostDetailedMip = 0;
    srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;

    d3d12Device->CreateShaderResourceView(mTexureBuffer.Get(), &srvDesc, handle);
}

Material::Material(Graphics& gfx,UINT textureCount)
    : mSource(gfx.AllocResource(textureCount))
    , mCount(textureCount)
{
}

Material::~Material()
{

}

void Material::AddTexture(std::unique_ptr<Texture> sur)
{
    mTextures.push_back(std::move(sur));
}

void Material::Upload(Graphics& gfx)
{
    if (mUpLoaded)
        return;

    auto currentHandle = mSource.RequestResource(mCount);

    for (auto& tex : mTextures)
    {
        tex->Upload(gfx);
        tex->Bind(gfx, currentHandle.cpuHandle);
        currentHandle++;
    }
    mUpLoaded = true;
}

void Material::Bind(Graphics& gfx)
{
    GraphicContext::GetCommandList(gfx)->SetGraphicsRootDescriptorTable(mSource.GetBindSlot(), mSource.GetResourceLocation());
}