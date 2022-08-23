#pragma once
#include "ConstantBuffers.h"
#include "Drawable.h"
#include <DirectXMath.h>

class TransformCbuf : public Bindable
{
public:
	TransformCbuf( Graphics& gfx,const Drawable& parent, UINT slot);
	void Bind( Graphics& gfx ) override;
private:
	const Drawable& parent;
	UINT mSlot;
};

class ColorCbuf : public Bindable
{
public:
	ColorCbuf(Graphics& gfx,const DirectX::XMFLOAT3& color, UINT slot);
	void Bind(Graphics& gfx) override;
private:
	DirectX::XMFLOAT3 mColor;
	UINT mSlot;
};