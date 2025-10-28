#include <CoreManager.h>
#include <TimeManager.h>
#include <GraphicsManager.h>
#include <TextureManager.h>
#include <FontManager.h>
#include <InputManager.h>
#include <SoundManager.h>
#include <World.h>
#include <System.h>
#include <VideoManager.h>

bool Engine::System::MainLoop()
{
	if(!Management->IsFinalDestroy())
	{
		Management->Tick();
		Management->Fixed(50);
		Management->Render();
		Management->EndPlay();
		Management->DestroyPoint();
		return true;
	}

	return false;
}

bool Engine::System::Initialize(GameSettings&& gameSettings)
{
	if(!gameSettings.pWorld)
    {
		MessageBoxEx(nullptr, L"World is nullptr", L"Error", MB_OK, MB_ICONERROR);
		return false;
    }

	Management->Initialize(std::move(gameSettings));
	Management->BeginPlay();
	Time->Initialize();
	FontMgr->Initialize();
	FontMgr->LoadFonts();
	Graphics->Initialize();
	InputMgr->Initialize();

    auto start = std::chrono::high_resolution_clock::now();
	TextureMgr->LoadTexture(L"Assets");
    auto end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<float> duration = end - start;
	std::cout << "Texture Load Time: " << duration.count() << "s\n";

	Sound->Initialize(Management->setting.maxSoundGroup);
	Sound->LoadSound("Assets/Sound");
	int layerSize = Management->setting.layerSize;
	Graphics->AddBrush("Default",D2D1::ColorF(D2D1::ColorF::Red));
	Management->setting.pWorld->InitializeWorld(layerSize);
	Management->setting.pWorld->BeginPlay();

	//리소스	로딩이 끝나면 윈도우를 보여준다.
	ShowWindow(Management->setting.hWnd, SW_SHOWNORMAL);
	UpdateWindow(Management->setting.hWnd);

	return true;
}

void Engine::System::Destroy()
{
	ManagementTerminate();
	TimeTerminate();
	FontTerminate();
	TextureTerminate();
	InputTerminate();
	GraphicsTerminate();
	SoundTerminate();
}

Engine::System* Engine::System::Create(GameSettings&& gameSettings)
{
	System* pSystem = new System();
	if (pSystem->Initialize(std::move(gameSettings)))
		return pSystem;

	SafeDelete(pSystem);
	return nullptr;
}
