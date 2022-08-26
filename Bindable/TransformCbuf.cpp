#include "TransformCbuf.h"
#include "GraphicContext.h"

TransformCbuf::TransformCbuf( Graphics& gfx,const Drawable& parent, UINT slot )
	: Super(gfx, TransformMat())
	, parent( parent )
{
	SetBindSlot(slot);
}

void TransformCbuf::Bind( Graphics& gfx )
{
	auto model = parent.GetTransformXM();
	auto view = gfx.GetViewMatrix();
	auto projection = gfx.GetProjection();

	TransformMat& mat = GetRawData();
	mat.ModelMatrix = model;
	mat.ModelViewMatrix = model * view;
	mat.InverseTransposeModelViewMatrix = XMMatrixTranspose(XMMatrixInverse(nullptr, mat.ModelViewMatrix));
	mat.ModelViewProjectionMatrix = model * view * projection;

	Super::Bind(gfx);
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