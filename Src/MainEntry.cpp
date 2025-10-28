#include <CoreDefine.h>
#include <MainEntry.h>
#include <System.h>
#include <SimpleIniLoader.h>
#include <WinUtility.h>
#include <cstdlib>

DEVMODE originalDevMode;

namespace fs = std::filesystem;

constexpr int screenStartLeft = 10;
constexpr int screenStartTop = 10;

bool Engine::MainEntry::LoadGameSettings(const std::string& iniPath, GameSettings& settings)
{
	std::string absolutePath = fs::absolute(iniPath).string();
    SimpleIniFile iniFile;
    try
    {
        iniFile.Load(absolutePath);
    }
    catch (const std::runtime_error& e)
    {
        std::cerr << "Error loading INI file: " << e.what() << std::endl;
		iniFile.SetValue("Settings", "title", "DefaultTitle");
		iniFile.SetValue("Settings", "width", "1920");
		iniFile.SetValue("Settings", "height", "1080");
		iniFile.SetValue("Settings", "maxSoundGroup", "5");
		iniFile.SetValue("Settings", "layerSize", "5");
		iniFile.Save(absolutePath);
    }

    settings.title = (string)iniFile.GetValue("Settings", "title", "DefaultTitle").c_str();
    settings.width = std::stoi(iniFile.GetValue("Settings", "width", "800"));
    settings.height = std::stoi(iniFile.GetValue("Settings", "height", "600"));
    settings.maxSoundGroup = std::stoi(iniFile.GetValue("Settings", "maxSoundGroup", "5"));
    settings.layerSize = std::stoi(iniFile.GetValue("Settings", "layerSize", "5"));

    return true;
}

void Engine::MainEntry::SaveCurrentResolution()
{
	ZeroMemory(&originalDevMode, sizeof(originalDevMode));
	originalDevMode.dmSize = sizeof(originalDevMode);
	if (!EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &originalDevMode)) 
	{
		std::cerr << "Failed to get current resolution" << std::endl;
	}
}

void Engine::MainEntry::ChangeResolution(int width, int height)
{
	DEVMODE devMode;
	ZeroMemory(&devMode, sizeof(devMode));
	devMode.dmSize = sizeof(devMode);
	devMode.dmPelsWidth = width;
	devMode.dmPelsHeight = height;
	devMode.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT;
	LONG result = ChangeDisplaySettings(&devMode, 0);
	if (result != DISP_CHANGE_SUCCESSFUL) 
	{
		std::cerr << "Failed to change resolution" << std::endl;
	}
}

void Engine::MainEntry::Initialize(GameSettings gameSettings)
{
	std::locale::global(std::locale("Korean"));

	const TCHAR* title = gameSettings.title;

	WNDCLASS wndClass{};

	wndClass.style = CS_HREDRAW | CS_VREDRAW;
	if (_customWNDPROC == NULL)
		wndClass.lpfnWndProc = WndProc;
	else
		wndClass.lpfnWndProc = _customWNDPROC;

	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hInstance = gameSettings.hInstance;
	wndClass.hIcon = LoadIcon(gameSettings.hInstance, IDI_APPLICATION);
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hbrBackground = static_cast<HBRUSH>(GetStockObject(WHITE_BRUSH));
	wndClass.lpszMenuName = NULL;
	wndClass.lpszClassName = title;

	RegisterClass(&wndClass);

#ifdef FINAL
	SaveCurrentResolution();
	ChangeResolution(1920, 1080);
#endif // FINAL

	RECT windowRect
	{
		screenStartLeft, 
		screenStartTop,
		screenStartLeft + gameSettings.width, 
		screenStartTop + gameSettings.height 
	};

	AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE);

#ifdef FINAL
	gameSettings.hWnd = CreateWindow
	(
		title,
		title,
		WS_POPUP | WS_SYSMENU,
		windowRect.left,
		windowRect.top,
		windowRect.right - windowRect.left,
		windowRect.bottom - windowRect.top,
		NULL,
		(HMENU)NULL,
		gameSettings.hInstance,
		NULL
	);

	SetWindowPos(gameSettings.hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
#endif // FINAL

#ifndef FINAL
	SetProcessDpiAwareness(PROCESS_PER_MONITOR_DPI_AWARE);

	gameSettings.hWnd = CreateWindow
	(
		title,
		title,
		WS_OVERLAPPEDWINDOW,
		windowRect.left,
		windowRect.top,
		windowRect.right - windowRect.left,
		windowRect.bottom - windowRect.top,
		NULL,
		(HMENU)NULL,
		gameSettings.hInstance,
		NULL
	);
	WinUtility::WindowToScrrenCenter(gameSettings.hWnd);
#endif // !FINAL


	_pSystem = System::Create(std::move(gameSettings));
}

void Engine::MainEntry::MainLoop()
{
	MSG msg;

	while (true)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			if (WM_QUIT == msg.message)
			{
				break;
			}
			else
			{
				DispatchMessage(&msg);
			}
		}
		else
		{
			if (!_pSystem->MainLoop())
				break;
		}
	}
}

void Engine::MainEntry::Terminate()
{
	if(_pSystem)
		SafeDelete(_pSystem);

#ifdef FINAL
	ChangeResolution(originalDevMode.dmPelsWidth, originalDevMode.dmPelsHeight);
#endif // FINAL

}

LRESULT Engine::MainEntry::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
