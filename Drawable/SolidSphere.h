#pragma once
#include "DrawableBase.h"

class SolidSphere : public DrawableBase<SolidSphere>
{
public:
	SolidSphere( Graphics& gfx,float radius, const DirectX::XMFLOAT3& color );
	void SetPos( DirectX::XMFLOAT3 pos );

	virtual void Update(float dt) override;
	virtual DirectX::XMMATRIX GetTransformXM() const override;

private:
	DirectX::XMFLOAT3 pos = { 0.0f,0.0f,0.0f };
	float mRadius{ 0.01f };
};