#pragma once
#include "Bindable.h"
#include "Graphics.h"
#include "GraphicContext.h"
#include "imgui.h"

template<typename C>
class ConstantBuffer
{
public:
	ConstantBuffer( Graphics& gfx,const C& consts )
	{
		auto handle = gfx.AllocHeap(1);
		auto device = GraphicContext::GetDevice(gfx);

		const UINT constantBufferSize = sizeof(C);    // CB size is required to be 256-byte aligned.

		ThrowIfFailed(device->CreateCommittedResource(
			&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
			D3D12_HEAP_FLAG_NONE,
			&CD3DX12_RESOURCE_DESC::Buffer(constantBufferSize),
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&mConstantBuffer)));

		// Describe and create a constant buffer view.
		D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc = {};
		cbvDesc.BufferLocation = mConstantBuffer->GetGPUVirtualAddress();
		cbvDesc.SizeInBytes = constantBufferSize;
		device->CreateConstantBufferView(&cbvDesc, handle.first);

		// Map and initialize the constant buffer. We don't unmap this until the
		// app closes. Keeping things mapped for the lifetime of the resource is okay.
		CD3DX12_RANGE readRange(0, 0);        // We do not intend to read from this resource on the CPU.
		ThrowIfFailed(mConstantBuffer->Map(0, &readRange, reinterpret_cast<void**>(&m_pCbvDataBegin)));
		memcpy(m_pCbvDataBegin, &consts, constantBufferSize);
	}

	void Update(Graphics& gfx, const C& consts)
	{
		memcpy(m_pCbvDataBegin, &consts, sizeof(consts));
	}

protected:
	ComPtr<ID3D12Resource> mConstantBuffer;
	UINT8* m_pCbvDataBegin;
};

class ChangeIndex : public Bindable, public ImguiItem
{
public:
	ChangeIndex(Graphics& gfx): mCBuf(gfx, mIndex)
	{
		gfx.AddImguiItem(this);
	}

	virtual void Bind(Graphics& gfx) override
	{
		mCBuf.Update(gfx, mIndex);
	}

	virtual void Display() override
	{
		if (ImGui::Begin("Projector"))
		{
			static bool check = true;
			ImGui::Checkbox("checkbox", &check);
			mIndex.index = check ? 0 : 1;
		}
		ImGui::End();
	}
private:
	struct CData
	{
		UINT index{ 0 };
		float padding[63];
	} mIndex;
	ConstantBuffer<CData> mCBuf;
};