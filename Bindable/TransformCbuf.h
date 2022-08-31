#pragma once
#include "ConstantBuffers.h"
#include "Drawable.h"
#include <DirectXMath.h>

struct TransformMat
{
	DirectX::XMMATRIX ModelMatrix;
	DirectX::XMMATRIX ModelViewMatrix;
	DirectX::XMMATRIX ModelViewProjectionMatrix;
	DirectX::XMMATRIX InverseTransposeModelMatrix;
};

class TransformCbuf : public ConstantBuffer<TransformMat>
{
public:
	TransformCbuf( Graphics& gfx,const Drawable& parent, UINT slot);
	virtual void Bind( Graphics& gfx ) override;
private:
	const Drawable& parent;
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