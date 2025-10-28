#pragma once

#include <SDKDDKVer.h>
#include <windows.h>
#include <CoreDefine.h>

namespace Engine
{
	class World;
	class System;
	class MainEntry abstract
	{
	public:
		MainEntry() = default;
		virtual ~MainEntry() = default;

	public:
		virtual void CreateWorld() abstract; //클라이언트에서 월드의 생성을 강제하기 위한 함수
		World* ContainWorld() const { return _pWorld; }

		bool LoadGameSettings(const std::string& iniPath, GameSettings& settings); 
		void SaveCurrentResolution(); // 강제 시스템 해상도 변경을 위한 함수 -> 이전 해상도 저장
		void ChangeResolution(int width, int height); // 강제 시스템 해상도 변경을 위한 함수
		void Initialize(GameSettings gameSettings);
		void MainLoop();
		void Terminate();

		static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	public:
		WNDPROC _customWNDPROC{ NULL };

	protected:
		World* _pWorld{ nullptr };

	private:
		System* _pSystem{ nullptr };
	};
}