#pragma once
#include "Bindable.h"
#include "GraphicsUtils.h"
#include "Graphics.h"
#include "GraphicContext.h"

template<typename V>
class VertexBuffer : public Bindable
	               , public Buffer
{
public:
	VertexBuffer(Graphics& gfx, const std::vector<V>& vertices)
		: stride(sizeof(V))
		, m_Vertices(vertices)
	{
	}

	virtual void Bind(Graphics& gfx) override
	{
		GraphicContext::GetCommandList(gfx)->IASetVertexBuffers(0u, 1u, &m_VertexBufferView);
	}

	virtual void Upload(Graphics& gfx) override
	{
		GraphicsUtils::UpdateBufferResource
		(
			GraphicContext::GetDevice(gfx),
			GraphicContext::GetCopyCommandList(gfx),
			&m_VertexBuffer,
			&m_IntermediateVertexBuffer,
			m_Vertices.size(),
			stride,
			m_Vertices.data()
		);
		m_VertexBuffer->SetName(L"Vertex buffer");

		// Create the vertex buffer view.
		m_VertexBufferView.BufferLocation = m_VertexBuffer->GetGPUVirtualAddress();
		m_VertexBufferView.SizeInBytes = m_Vertices.size() * stride;
		m_VertexBufferView.StrideInBytes = stride;
	}

protected:
	UINT stride;
	std::vector<V> m_Vertices;
	ComPtr<ID3D12Resource> m_IntermediateVertexBuffer;
	ComPtr<ID3D12Resource> m_VertexBuffer;
	D3D12_VERTEX_BUFFER_VIEW m_VertexBufferView;
};
