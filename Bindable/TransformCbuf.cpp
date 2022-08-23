#include "TransformCbuf.h"
#include "GraphicContext.h"

TransformCbuf::TransformCbuf( Graphics& gfx,const Drawable& parent, UINT slot )
	: parent( parent )
	, mSlot(slot)
{}

void TransformCbuf::Bind( Graphics& gfx )
{
	auto matrix = parent.GetTransformXM() * gfx.GetViewMatrix() * gfx.GetProjection();
	GraphicContext::GetCommandList(gfx)->SetGraphicsRoot32BitConstants(mSlot, sizeof(DirectX::XMMATRIX) / 4, &matrix, 0);
}

ColorCbuf::ColorCbuf(Graphics& gfx, const DirectX::XMFLOAT3& color, UINT slot)
	:mSlot(slot)
	,mColor(color)
{

}
void ColorCbuf::Bind(Graphics& gfx)
{
	GraphicContext::GetCommandList(gfx)->SetGraphicsRoot32BitConstants(mSlot, sizeof(DirectX::XMFLOAT4) / 4, &mColor, 0);
}