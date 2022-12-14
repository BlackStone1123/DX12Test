#include "IndexBuffer.h"
#include "GraphicsUtils.h"
#include "GraphicContext.h"
#include "BindableCodex.h"

IndexBuffer::IndexBuffer( Graphics& gfx,const std::string& tag, const std::vector<unsigned short>& indices )
	: count( (UINT)indices.size() )
    , mTag(tag)
    , m_Indices(indices)
{
}

void IndexBuffer::Bind( Graphics& gfx )
{
    GraphicContext::GetCommandList(gfx)->IASetIndexBuffer(&m_IndexBufferView);
}

void IndexBuffer::Upload(Graphics& gfx)
{
    if (mUpLoaded)
        return;

    // Upload index buffer data.
    GraphicsUtils::UpdateBufferResource
    (
        GraphicContext::GetDevice(gfx),
        GraphicContext::GetCopyCommandList(gfx),
        &m_IndexBuffer,
        &m_IntermediateIndexBuffer,
        m_Indices.size(),
        sizeof(unsigned short),
        m_Indices.data()
    );

    m_IndexBuffer->SetName(L"Index buffer");
    // Create index buffer view.
    m_IndexBufferView.BufferLocation = m_IndexBuffer->GetGPUVirtualAddress();
    m_IndexBufferView.Format = DXGI_FORMAT_R16_UINT;
    m_IndexBufferView.SizeInBytes = (UINT)m_Indices.size() * sizeof(UINT16);
    mUpLoaded = true;
}

UINT IndexBuffer::GetCount() const
{
	return count;
}

std::shared_ptr<IndexBuffer> IndexBuffer::Resolve(
    Graphics& gfx,
    const std::string& tag,
    const std::vector<unsigned short>& indices)
{
    assert(tag != "?");
    return Codex::Resolve<IndexBuffer>(gfx, tag, indices);
}