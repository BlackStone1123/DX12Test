#include "App.h"
#include "Graphics.h"
#include "Window.h"
#include "Box.h"

App::App(HINSTANCE hIns, int width, int height)
	: mWnd(std::make_unique<Window>(hIns, width, height, L"DX12≤‚ ‘≥Ã–Ú"))
{
	std::mt19937 rng(std::random_device{}());
	std::uniform_real_distribution<float> adist(0.0f, 3.1415f * 2.0f);
	std::uniform_real_distribution<float> ddist(0.0f, 3.1415f * 2.0f);
	std::uniform_real_distribution<float> odist(0.0f, 3.1415f * 0.3f);
	std::uniform_real_distribution<float> rdist(6.0f, 20.0f);
	for (auto i = 0; i < 20; i++)
	{
		boxes.push_back(std::make_unique<Box>(
			*mWnd->Gfx(), rng, adist,
			ddist, odist, rdist
			));
	}
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
			gfx->BeginFrame(0.4f, 0.6f, 0.9f, 1.0f);
			for (int i = 0; i < boxes.size(); i++)
			{
				boxes[i]->Update(*gfx, 0.01f);
				boxes[i]->Draw(*gfx);
			}
			gfx->EndFrame();
		}
	}
}
