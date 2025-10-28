#pragma once
#include <CoreDefine.h>
#include <Singleton.h>

namespace Engine
{
	CORECLASS()
	class InputManager : public CoreBase, public Singleton<InputManager>
	{
	private:
		friend class Singleton;

	private:
		InputManager() = default;
		~InputManager() = default;

	public:
		using BindKeyMap = std::unordered_map<_uint, std::vector<InputComponent*>>;
		using ButtonMappings = std::unordered_set<_uint>;
		using GamePadButtonStateMap = std::unordered_map<_uint, bool>;

	public:
		bool Initialize();
		void InputUpdate();
		void RegisterComponent(_uint key, InputComponent* inputComponent);
		void UnRegisterComponent(_uint key, InputComponent* inputComponents);
		XINPUT_STATE* ReturnGamePad() { return _pGamePad; }
		IDirectInputDevice8W* ReturnKeyboard() { return _pKeyboard.get(); }

	public:
		virtual void SerializeIn(nlohmann::ordered_json& object) {};
		virtual void SerializeOut(nlohmann::ordered_json& object) {};

	public:
		void ProcessKeyboardInput();
		void ProcessMouseInput();
		void ProcessGamePadInput();

	public:
		void DispatchInput(InputDevice Device, InputType Type, _uint Key, _float Value = 0.f, bool State = false, long x = 0, long y = 0);

	private:
		void InitializeKeyboard();
		void InitializeMouse();
		void InitializeGamePad();

	private:
		void ButtonMapping(_uint key);
		void ButtonUnMapping(_uint key);

	public:
		void Destroy() override;

	private:
		SmartPtr<IDirectInput8W>		_pInputSDK{ nullptr };
		SmartPtr<IDirectInputDevice8W>	_pKeyboard{ nullptr };
		SmartPtr<IDirectInputDevice8W>	_pMouse{ nullptr };

	private:
		DIMOUSESTATE  _previousMouseState{};
		XINPUT_STATE* _pGamePad{ nullptr };
		DWORD		  _gamePadIndex = 0;
		HINSTANCE	   hInstance{ nullptr };
		HWND		   hWnd{ nullptr };

	private:
		_byte		 _keyboardState[256]{};
		_byte		 _previousKeyboardState[256]{};

	private:
		BindKeyMap				_bindKeyMap;
		ButtonMappings			_buttonMappings;
		GamePadButtonStateMap	_gamePadButtonStateMap;
	};
}

inline Engine::InputManager* InputMgr = Engine::InputManager::GetInstance();
inline std::function<void()> InputTerminate = []() { InputMgr->Terminate(); };