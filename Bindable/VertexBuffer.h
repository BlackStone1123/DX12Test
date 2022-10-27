#pragma once
#include "Bindable.h"
#include "GraphicsUtils.h"
#include "Graphics.h"
#include "GraphicContext.h"
#include "BindableCodex.h"

template<typename V>
class VertexBuffer : public Bindable
	               , public Resource
{
public:
	VertexBuffer(Graphics& gfx, const std::string& tag, const std::vector<V>& vertices)
		: stride(sizeof(V))
		, mTag(tag)
		, m_Vertices(vertices)
	{
	}

	virtual void Bind(Graphics& gfx) override
	{
		GraphicContext::GetCommandList(gfx)->IASetVertexBuffers(0u, 1u, &m_VertexBufferView);
	}

	virtual void Upload(Graphics& gfx) override
	{
		if (mUpLoaded)
			return;

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
		m_VertexBufferView.SizeInBytes = (UINT)m_Vertices.size() * stride;
		m_VertexBufferView.StrideInBytes = stride;
		mUpLoaded = true;
	}

	static std::shared_ptr<VertexBuffer> Resolve(Graphics& gfx, const std::string& tag,
		const std::vector<V>& vertices)
	{
		assert(tag != "?");
		return Codex::Resolve<VertexBuffer>(gfx, tag, vertices);
	}

	template<typename...Ignore>
	static std::string GenerateUID(const std::string& tag, Ignore&&...ignore)
	{
		using namespace std::string_literals;
		return typeid(VertexBuffer).name() + "#"s + tag;
	}

protected:
	UINT stride;
	std::vector<V> m_Vertices;
	std::string mTag;

	ComPtr<ID3D12Resource> m_IntermediateVertexBuffer;
	ComPtr<ID3D12Resource> m_VertexBuffer;
	D3D12_VERTEX_BUFFER_VIEW m_VertexBufferView;
};
