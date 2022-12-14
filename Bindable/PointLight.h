#pragma once
#include "SolidSphere.h"
#include "ConstantBuffers.h"

class Graphics;
struct PointLightCBuf
{
	DirectX::XMFLOAT4 pos;
	DirectX::XMFLOAT4 materialColor;
	DirectX::XMFLOAT4 ambient;
	DirectX::XMFLOAT4 diffuseColor;
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
	virtual void Bind(Graphics& gfx) override;

	void Update();
	SolidSphere* getGeo() { return &mesh; }

private:
	Graphics* mGfx{ nullptr };
	SolidSphere mesh;
	DirectX::XMFLOAT4 mPosW{ 0.0f,0.0f,0.0f, 1.0f };
};