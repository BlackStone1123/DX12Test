#include "IndexBuffer.h"
#include "GraphicsUtils.h"
#include "GraphicContext.h"

IndexBuffer::IndexBuffer( Graphics& gfx,const std::vector<unsigned short>& indices )
	: count( (UINT)indices.size() )
    , m_Indices(indices)
{

}

void IndexBuffer::Bind( Graphics& gfx )
{
    GraphicContext::GetCommandList(gfx)->IASetIndexBuffer(&m_IndexBufferView);
}

void IndexBuffer::upload(Graphics& gfx)
{
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
    m_IndexBufferView.SizeInBytes = m_Indices.size() * sizeof(UINT16);
}

UINT IndexBuffer::GetCount() const
{
	return count;
}
