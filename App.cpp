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
	for (auto i = 0; i < 20; i++)
	{
		mDrawables.push_back(std::make_unique<Box>(
			*mWnd->Gfx(), rng, adist,
			ddist, odist, rdist
			));
	}
	//mDrawables.push_back(std::make_unique<SolidSphere>(*mWnd->Gfx(), 0.2f, DirectX::XMFLOAT3(0.5f, 0.5f, 1.0f)));

	mWnd->Gfx()->AddCamera(std::make_unique<Camera>());
	mWnd->Gfx()->AddProjector(std::make_unique<Projector>(width, height));
}

App::~App()
{

}

int App::exec()
{
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
			gfx->GetPointLight()->Draw(*gfx);

			for (int i = 0; i < mDrawables.size(); i++)
			{
				mDrawables[i]->Update(dt);
				mDrawables[i]->Draw(*gfx);
			}

			// imgui window to control simulation speed
			if (ImGui::Begin("Simulation Speed"))
			{
				ImGui::SliderFloat("Speed Factor", &mSpeedFactor, 0.0f, 4.0f);
				ImGui::Text("%.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			}
			ImGui::End();

			gfx->ShowImguiItems();
			gfx->EndFrame();
		}
	}
}
