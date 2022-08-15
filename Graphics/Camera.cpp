#include "Camera.h"
#include "imgui.h"

namespace dx = DirectX;

DirectX::XMMATRIX Camera::GetMatrix() const
{
	const auto pos = dx::XMVector3Transform(
		dx::XMVectorSet( 0.0f,0.0f,-r,0.0f ),
		dx::XMMatrixRotationRollPitchYaw( phi,-theta,0.0f )
	);
	return dx::XMMatrixLookAtLH(
		pos,dx::XMVectorZero(),
		dx::XMVectorSet( 0.0f,1.0f,0.0f,0.0f )
	) * dx::XMMatrixRotationRollPitchYaw(
		pitch,-yaw,roll
	);
}

void Camera::Display()
{
	if( ImGui::Begin( "Camera" ) )
	{
		ImGui::Text( "Position" );
		ImGui::SliderFloat( "R",&r,1.0f,80.0f,"%.1f" );
		ImGui::SliderAngle( "Theta",&theta,-180.0f,180.0f );
		ImGui::SliderAngle( "Phi",&phi,-89.0f,89.0f );

		ImGui::Text( "Orientation" );
		ImGui::SliderAngle( "Roll",&roll,-180.0f,180.0f );
		ImGui::SliderAngle( "Pitch",&pitch,-180.0f,180.0f );
		ImGui::SliderAngle( "Yaw",&yaw,-180.0f,180.0f );
		if( ImGui::Button( "Reset" ) )
		{
			Reset();
		}
	}
	ImGui::End();
}

void Camera::Reset()
{
	r = 20.0f;
	theta = 0.0f;
	phi = 0.0f;
	pitch = 0.0f;
	yaw = 0.0f;
	roll = 0.0f;
}

Projector::Projector(UINT width, UINT height)
	: mScreenWidth(width), mScreenHeight(height)
{

}
DirectX::XMMATRIX Projector::GetMatrix() const
{
	return mPerspective ?
		dx::XMMatrixPerspectiveFovLH(dx::XMConvertToRadians(mFov), mAspectRatio, mNear, mFar) :
		dx::XMMatrixOrthographicLH(mViewWidth, mViewHeight, mNear, mFar);
}

void Projector::Display()
{
	if (ImGui::Begin("Projector"))
	{
		ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
		if (ImGui::BeginTabBar("MyTabBar", tab_bar_flags))
		{
			if (ImGui::BeginTabItem("Perspective"))
			{
				mPerspective = true;
				ImGui::SliderFloat("Fov", &mFov, 1.0f, 179.0f);
				ImGui::SliderFloat("AspectRatio", &mAspectRatio, 0.1f, (float)mScreenWidth / mScreenHeight);
				ImGui::SliderFloat("NearZ", &mNear, 0.1f, 500.0f);
				ImGui::SliderFloat("FarZ", &mFar, 10.0f, 500.0f);
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Ortho"))
			{
				mPerspective = false;
				ImGui::SliderFloat("ViewWidth", &mViewWidth, 1.0f, 100.0f);
				ImGui::SliderFloat("ViewHeight", &mViewHeight, 1.0f,100.0f);
				ImGui::SliderFloat("NearZ", &mNear, 0.1f, 500.0f);
				ImGui::SliderFloat("FarZ", &mFar, 10.0f, 500.0f);
				ImGui::EndTabItem();
			}
			ImGui::EndTabBar();
		}
	}
	ImGui::End();
}
void Projector::Reset()
{

}