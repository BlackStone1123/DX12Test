#pragma once
#include "Bindable.h"
#include "Graphics.h"
#include "GraphicContext.h"
#include "imgui.h"
#include <optional>

template<typename C>
class RawConstantBuffer
{
public:
	RawConstantBuffer( Graphics& gfx,const C& consts)
		:mBufferSize(sizeof(C))
	{
		auto device = GraphicContext::GetDevice(gfx);

		ThrowIfFailed(device->CreateCommittedResource(
			&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
			D3D12_HEAP_FLAG_NONE,
			&CD3DX12_RESOURCE_DESC::Buffer(mBufferSize),
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&mConstantBuffer)));

		// Map and initialize the constant buffer. We don't unmap this until the
		// app closes. Keeping things mapped for the lifetime of the resource is okay.
		CD3DX12_RANGE readRange(0, 0);        // We do not intend to read from this resource on the CPU.
		ThrowIfFailed(mConstantBuffer->Map(0, &readRange, reinterpret_cast<void**>(&m_pCbvDataBegin)));
		memcpy(m_pCbvDataBegin, &consts, mBufferSize);
	}

	void CreateConstantBufferView(Graphics& gfx, D3D12_CPU_DESCRIPTOR_HANDLE handle)
	{
		auto device = GraphicContext::GetDevice(gfx);

		// Describe and create a constant buffer view.
		D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc = {};
		cbvDesc.BufferLocation = mConstantBuffer->GetGPUVirtualAddress();
		cbvDesc.SizeInBytes = mBufferSize;
		device->CreateConstantBufferView(&cbvDesc, handle);
	}

	void Update(const C& consts)
	{
		memcpy(m_pCbvDataBegin, &consts, sizeof(consts));
	}

	D3D12_GPU_VIRTUAL_ADDRESS GetBufferVirtualAddress() const
	{
		return mConstantBuffer->GetGPUVirtualAddress();
	}

protected:
	ComPtr<ID3D12Resource> mConstantBuffer;
	UINT8* m_pCbvDataBegin;
	UINT mBufferSize{ 0 };
};

template<typename C>
class ConstantBuffer : public Bindable
{
public:
	void SetBindSlot(UINT slot)
	{
		mSlot = slot;
	}

	virtual void Bind(Graphics& gfx) override
	{
		mCBuf.Update(mData);

		if (mSlot.has_value())
		{
			GraphicContext::GetCommandList(gfx)->SetGraphicsRootConstantBufferView
			(
				mSlot.value(),
				mCBuf.GetBufferVirtualAddress()
			);
		}
	}

protected:
	using Super = ConstantBuffer<C>;
	ConstantBuffer(Graphics& gfx,const C& data, const HeapAllocation& location)
		: mCBuf(gfx, data)
		, mData(data)
	{
		mCBuf.CreateConstantBufferView(gfx, location.cpuHandle);
	}

	ConstantBuffer(Graphics& gfx,const C& data)
		: mCBuf(gfx, data)
		, mData(data)
	{
	}

	C& GetRawData() { return mData; }

private:
	RawConstantBuffer<C> mCBuf;
	std::optional<UINT> mSlot;
	C mData;
};