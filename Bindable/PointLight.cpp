#include "PointLight.h"
#include "imgui.h"
#include "Graphics.h"

PointLight::PointLight( Graphics& gfx, float radius )
	: Super(gfx, PointLightCBuf())
	, mesh(gfx, radius, { 1.0f,1.0f,1.0f })
	, mGfx(&gfx)
{
	Reset();
}

void PointLight::Display()
{
	auto& cbData = GetRawData();

	if (ImGui::CollapsingHeader("Light"))
	{
		ImGui::Text("Position");
		ImGui::SliderFloat("X", &mPosW.x, -60.0f, 60.0f, "%.1f");
		ImGui::SliderFloat("Y", &mPosW.y, -60.0f, 60.0f, "%.1f");
		ImGui::SliderFloat("Z", &mPosW.z, -60.0f, 60.0f, "%.1f");

		ImGui::Text("Intensity/Color");
		ImGui::SliderFloat("Intensity", &cbData.diffuseIntensity, 0.01f, 2.0f, "%.2f");
		ImGui::ColorEdit3("Diffuse Color", &cbData.diffuseColor.x);
		ImGui::ColorEdit3("Ambient", &cbData.ambient.x);

		ImGui::Text("Falloff");
		ImGui::SliderFloat("Constant", &cbData.attConst, 0.05f, 10.0f, "%.2f");
		ImGui::SliderFloat("Linear", &cbData.attLin, 0.0001f, 4.0f, "%.4f");
		ImGui::SliderFloat("Quadratic", &cbData.attQuad, 0.0000001f, 10.0f, "%.7f");

		if (ImGui::Button("Reset"))
		{
			Reset();
		}
	}
}

void PointLight::Reset()
{
	auto& cbData = GetRawData();

	cbData = {
		{ 0.0f,0.0f,0.0f, 1.0f },
		{ 0.7f,0.7f,0.9f, 1.0f },
		{ 0.2f,0.2f,0.2f,1.0f },
		{ 1.0f,1.0f,1.0f,1.0f },
		1.0f,
		1.0f,
		0.001f,
		0.001f,
	};
	mPosW = { 0.0f,0.0f,0.0f, 1.0f };
}

void PointLight::Bind(Graphics& gfx)
{
	auto& cbData = GetRawData();

	if (mGfx)
	{
		DirectX::XMVECTOR positionWS = DirectX::XMLoadFloat4(&mPosW);
		DirectX::XMVECTOR positionVS = DirectX::XMVector3TransformCoord(positionWS, mGfx->GetViewMatrix());
		DirectX::XMStoreFloat4(&cbData.pos, positionVS);
	}
	Super::Bind(gfx);
}

void PointLight::Update()
{
	mesh.SetPos(mPosW);
}