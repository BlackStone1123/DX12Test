#include "App.h"
#include "Graphics.h"
#include "Window.h"
#include "Box.h"
#include "SolidSphere.h"
#include "imgui.h"
#include "ImguiManager.h"
#include "Camera.h"
#include "Projector.h"
#include "PointLight.h"
#include "Material.h"
#include "Pass.h"

App::App(HINSTANCE hIns, int width, int height)
	: mImguiManager(std::make_unique<ImguiManager>())
	, mWnd(std::make_unique<Window>(hIns, width, height, L"DX12≤‚ ‘≥Ã–Ú"))
{
	std::mt19937 rng(std::random_device{}());
	std::uniform_real_distribution<float> adist(0.0f, 3.1415f * 2.0f);
	std::uniform_real_distribution<float> ddist(0.0f, 3.1415f * 2.0f);
	std::uniform_real_distribution<float> odist(0.0f, 3.1415f * 0.3f);
	std::uniform_real_distribution<float> rdist(6.0f, 20.0f);

	mWnd->Gfx()->AddPointLight(std::make_unique<PointLight>(*mWnd->Gfx()));

	auto mat = std::make_shared<Material>(*mWnd->Gfx(), 3);
	mat->AddTexture(std::make_unique<Texture>(L"../../../Assets/Texture/container2.png", true));
	mat->AddTexture(std::make_unique<Texture>(L"../../../Assets/Texture/pexels-david-bartus-963278.jpg", true));
	mat->AddTexture(std::make_unique<Texture>(L"../../../Assets/Texture/container2_specular.png", true));

	auto mat2 = std::make_shared<Material>(*mWnd->Gfx(), 3);
	mat2->AddTexture(std::make_unique<Texture>(L"../../../Assets/Texture/img-hero_dog.png", true));
	mat2->AddTexture(std::make_unique<Texture>(L"../../../Assets/Texture/shutterstock_673465372.jpg", true));
	mat2->AddTexture(std::make_unique<Texture>(L"../../../Assets/Texture/container2_specular.png", true));

	for (auto i = 0; i < 20; i++)
	{
		mDrawables.push_back(std::make_unique<Box>(
			*mWnd->Gfx(), rng, adist,
			ddist, odist, rdist
			));
		mDrawables.back()->AttachMaterial(i%2 == 0 ? mat : mat2);
		mBorders.push_back(std::make_unique<Border>(*mWnd->Gfx()));
	}

	mWnd->Gfx()->AddCamera(std::make_unique<Camera>());
	mWnd->Gfx()->AddProjector(std::make_unique<Projector>(width, height));
}

App::~App()
{

}

int App::exec()
{
	SolidColorPass solidPass( *mWnd->Gfx(), { 1.0f,1.0f,1.0f });
	PhongShaderPass phongPass( *mWnd->Gfx());
	BorderPass borderPass( *mWnd->Gfx(), { 0.0f,1.0f,0.0f});
	bool enableBorder = false;

	while (true)
	{
		// process all messages pending, but to not block for new messages
		if (const auto ecode = mWnd->ProcessMessages())
		{
			// if return optional has value, means we're quitting so return exit code
			return *ecode;
		}

		if (auto gfx = mWnd->Gfx())
		{
			const auto dt = mTimer.Mark() * mSpeedFactor;

			gfx->BeginFrame(0.4f, 0.6f, 0.9f, 1.0f);
			gfx->GetPointLight()->Update();

			solidPass.AttachDrawable(gfx->GetPointLight()->getGeo());
			solidPass.Execute(*gfx);

			for (int i = 0; i < mDrawables.size(); i++)
			{
				mDrawables[i]->Update(dt);
				phongPass.AttachDrawable(mDrawables[i].get());
			}
			phongPass.SetPointLight(gfx->GetPointLight());
			phongPass.Execute(*gfx);

			if (enableBorder)
			{
				for (int i = 0; i < mDrawables.size(); i++)
				{
					mBorders[i]->setTransformXM(mDrawables[i]->GetTransformXM());
					borderPass.AttachDrawable(mBorders[i].get());
				}
				borderPass.Execute(*gfx);
			}

			// imgui window to control simulation speed
			bool pOpen = true;
			if (ImGui::Begin("Control Panel", &pOpen))
			{
				ImGui::SliderFloat("Speed Factor", &mSpeedFactor, 0.0f, 4.0f);
				ImGui::Text("%.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
				ImGui::Checkbox("enable border", &enableBorder);

				gfx->ShowImguiItems();
			}
			ImGui::End();

			gfx->EndFrame();
		}
	}
}
