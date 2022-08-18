#include "Projector.h"
#include "imgui.h"

namespace dx = DirectX;

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