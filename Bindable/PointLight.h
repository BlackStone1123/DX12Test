#pragma once
#include "SolidSphere.h"
#include "ConstantBuffers.h"

class Graphics;
struct PointLightCBuf
{
	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT3 materialColor;
	DirectX::XMFLOAT3 ambient;
	DirectX::XMFLOAT3 diffuseColor;
	float diffuseIntensity;
	float attConst;
	float attLin;
	float attQuad;
};

class PointLight : public ConstantBuffer<PointLightCBuf>
	             , public ImguiItem
{
public:
	PointLight( Graphics& gfx, float radius = 0.5f );

	virtual void Display() override;
	virtual void Reset() override;

	void Draw( Graphics& gfx );

private:
	SolidSphere mesh;
};

class PointLightReference : public Bindable
{
public:
	PointLightReference(PointLight* pPointLight, UINT slot)
		: mPointLight(pPointLight)
		, mSlot(slot)
	{

	}
	virtual void Bind(Graphics& gfx) override
	{
		if (mPointLight)
		{
			mPointLight->SetBindSlot(mSlot);
			mPointLight->Bind(gfx);
		}
	}

private:
	PointLight* mPointLight{ nullptr };
	UINT mSlot;
};