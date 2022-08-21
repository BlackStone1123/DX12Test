#pragma once
#include "Bindable.h"

class IndexBuffer : public Bindable
				  , public Resource
{
public:
	IndexBuffer( Graphics& gfx,const std::vector<unsigned short>& indices );
	UINT GetCount() const;

public:
	virtual void Bind(Graphics& gfx) override;
	virtual void Upload(Graphics& gfx) override;

protected:
	UINT count;
	std::vector<unsigned short> m_Indices;
	ComPtr<ID3D12Resource> m_IndexBuffer;
	ComPtr<ID3D12Resource> m_IntermediateIndexBuffer;
	D3D12_INDEX_BUFFER_VIEW m_IndexBufferView;
};