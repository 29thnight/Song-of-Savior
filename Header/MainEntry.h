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
		virtual void CreateWorld() abstract; //Ŭ���̾�Ʈ���� ������ ������ �����ϱ� ���� �Լ�
		World* ContainWorld() const { return _pWorld; }

		bool LoadGameSettings(const std::string& iniPath, GameSettings& settings); 
		void SaveCurrentResolution(); // ���� �ý��� �ػ� ������ ���� �Լ� -> ���� �ػ� ����
		void ChangeResolution(int width, int height); // ���� �ý��� �ػ� ������ ���� �Լ�
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