#include "TransformCbuf.h"
#include "GraphicContext.h"

TransformCbuf::TransformCbuf( Graphics& gfx,const Drawable& parent )
	: parent( parent )
{}

void TransformCbuf::Bind( Graphics& gfx )
{
	auto matrix = gfx.GetProjection();
	matrix = parent.GetTransformXM() * matrix;
	GraphicContext::GetCommandList(gfx)->SetGraphicsRoot32BitConstants(0, sizeof(DirectX::XMMATRIX) / 4, &matrix, 0);
}
