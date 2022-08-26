#include "PointLight.h"
#include "imgui.h"
#include "Graphics.h"

PointLight::PointLight( Graphics& gfx, float radius )
	: Super(gfx, PointLightCBuf())
	, mesh(gfx, radius, { 1.0f,1.0f,1.0f })
{
	Reset();
}

void PointLight::Display()
{
	auto& cbData = GetRawData();

	if( ImGui::Begin( "Light" ) )
	{
		ImGui::Text( "Position" );
		ImGui::SliderFloat( "X",&cbData.pos.x,-60.0f,60.0f,"%.1f" );
		ImGui::SliderFloat( "Y",&cbData.pos.y,-60.0f,60.0f,"%.1f" );
		ImGui::SliderFloat( "Z",&cbData.pos.z,-60.0f,60.0f,"%.1f" );
		if( ImGui::Button( "Reset" ) )
		{
			Reset();
		}
	}
	ImGui::End();
}

void PointLight::Reset()
{
	auto& cbData = GetRawData();

	cbData = {
		{ 0.0f,0.0f,0.0f },
		{ 0.7f,0.7f,0.9f },
		{ 0.05f,0.05f,0.05f },
		{ 1.0f,1.0f,1.0f },
		1.0f,
		1.0f,
		0.045f,
		0.0075f,
	};
}

void PointLight::Draw( Graphics& gfx )
{
	auto& cbData = GetRawData();

	mesh.SetPos( cbData.pos );
	mesh.Draw( gfx );
}