#include <CoreDefine.h>
#include <CoreManager.h>
#include <InputManager.h>
#include <InputComponent.h>

bool Engine::InputManager::Initialize()
{

	HRESULT hresult = DirectInput8Create(Management->setting.hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&_pInputSDK, nullptr);
	if (FAILED(hresult))
	{
		MessageBox(Management->setting.hWnd, L"DirectInput8Create() failed", L"Error", MB_OK);
		return false;
	}

	this->hWnd = Management->setting.hWnd;

	InitializeKeyboard();
	InitializeMouse();
	InitializeGamePad();

	return true;
}

void Engine::InputManager::InputUpdate()
{
	if(_pKeyboard)
	{
		ProcessKeyboardInput();
	}

	if(_pMouse)
	{
		ProcessMouseInput();
	}

	if(_pGamePad)
	{
		ProcessGamePadInput();
	}
}

void Engine::InputManager::RegisterComponent(_uint key, InputComponent* inputComponent)
{
	_buttonMappings.insert(key);
	_bindKeyMap[key].push_back(inputComponent);
}

void Engine::InputManager::UnRegisterComponent(_uint key, InputComponent* inputComponents)
{
	_buttonMappings.erase(key);
	for (auto iter = _bindKeyMap[key].begin(); iter != _bindKeyMap[key].end(); ++iter)
	{
		if (*iter == inputComponents)
		{
			iter = _bindKeyMap[key].erase(iter);
			break;
		}
	}

}

void Engine::InputManager::ProcessKeyboardInput()
{
	static bool isReleased = false;
	HRESULT hresult = _pKeyboard->GetDeviceState(sizeof(_keyboardState), (LPVOID)&_keyboardState);
	if(FAILED(hresult))
	{
		if(hresult == DIERR_INPUTLOST || hresult == DIERR_NOTACQUIRED)
		{
			if(!isReleased)
			{
				for(auto& key : _buttonMappings)
				{
					if (255 >= key)
					{
						bool isPressed = (_keyboardState[key] & 0x80) != 0;
						DispatchInput(InputDevice::KEYBOARD, InputType::RELEASE, key, 0.0f, isPressed);
					}
				}	
				isReleased = true;
			}
		}
		hresult = _pKeyboard->Acquire();
		if (SUCCEEDED(hresult))
		{
			/*std::cout << "Acquire Success" << std::endl;*/
			isReleased = false;
		}
	}
	else
	{
		for(auto& key : _buttonMappings)
		{
			if (255 >= key)
			{
				bool isPressed = (_keyboardState[key] & 0x80) != 0;
				bool wasPressed = (_previousKeyboardState[key] & 0x80) != 0;

				if (isPressed && !wasPressed)
				{
					DispatchInput(InputDevice::KEYBOARD, InputType::PRESS, key, 0.f, true);
				}
				if (isPressed && wasPressed)
				{
					DispatchInput(InputDevice::KEYBOARD, InputType::HELD, key, 0.f, true);
				}
				if (!isPressed && wasPressed)
				{
					DispatchInput(InputDevice::KEYBOARD, InputType::RELEASE, key, 0.f, false);
				}
				if (!isPressed && !wasPressed)
				{
					DispatchInput(InputDevice::KEYBOARD, InputType::IDLE, key, 0.f, false);
				}

				_previousKeyboardState[key] = _keyboardState[key];
			}
		}
	}
}

void Engine::InputManager::ProcessMouseInput()
{
	static bool isReleased = false;
	DIMOUSESTATE mouseState{};
	HRESULT hresult = _pMouse->GetDeviceState(sizeof(DIMOUSESTATE), (LPVOID)&mouseState);
	if(FAILED(hresult))
	{
		if(hresult == DIERR_INPUTLOST || hresult == DIERR_NOTACQUIRED)
		{
			if(!isReleased)
			{
				for (_uint i = 0; i < DIM_END; ++i)
				{
					bool isPressed = (mouseState.rgbButtons[i] & 0x80) != 0;
					DispatchInput(InputDevice::MOUSE, InputType::RELEASE, i, 0.0f, isPressed);
				}
				isReleased = true;
			}
			hresult = _pMouse->Acquire();
			if (SUCCEEDED(hresult))
			{
				/*std::cout << "Acquire Success" << std::endl;*/
				isReleased = false;
			}
		}
	}
	else
	{
		for (_uint i = 0; i < DIM_END; ++i)
        {
            bool isPressed = (mouseState.rgbButtons[i] & 0x80) != 0;
			bool wasPressed = (_previousMouseState.rgbButtons[i] & 0x80) != 0;
			if (isPressed && !wasPressed)
			{
				DispatchInput(InputDevice::MOUSE, InputType::PRESS, i, 0.f, true);
			}
			if (isPressed && wasPressed)
			{
				DispatchInput(InputDevice::MOUSE, InputType::HELD, i, 0.f, true);
			}
			if (!isPressed && wasPressed)
			{
				DispatchInput(InputDevice::MOUSE, InputType::RELEASE, i, 0.f, false);
			}
			if (!isPressed && !wasPressed)
			{
				DispatchInput(InputDevice::MOUSE, InputType::IDLE, i, 0.f, false);
			}
        }

        if (mouseState.lZ != _previousMouseState.lZ)
        {
            DispatchInput(InputDevice::MOUSE, InputType::SCROLL, DIM_SCROLL, static_cast<float>(mouseState.lZ));
        }

        if (mouseState.lX != _previousMouseState.lX || mouseState.lY != _previousMouseState.lY)
        {
			POINT mousePos;
			GetCursorPos(&mousePos);
			ScreenToClient(hWnd, &mousePos);
			//DebugPrintf("inputManager : %ld,%ld\n", mousePos.x, mousePos.y);

            DispatchInput(InputDevice::MOUSE, InputType::MOVE, DIM_MOVE, 0.0f, false, mousePos.x, mousePos.y);
        }

		_previousMouseState = mouseState;
	}
}

void Engine::InputManager::ProcessGamePadInput()
{
	if (XInputGetState(_gamePadIndex, _pGamePad) == ERROR_SUCCESS)
	{
		for (_uint i = 0; i < DIP_MAX; i++)
		{
			bool isPressed = (_pGamePad->Gamepad.wButtons & (1 << i)) != 0;
			if (isPressed && !_gamePadButtonStateMap[i])
            {
                DispatchInput(InputDevice::GAMEPAD, InputType::PRESS, i, 0.f, true);
            }
            else if (isPressed && _gamePadButtonStateMap[i])
            {
                DispatchInput(InputDevice::GAMEPAD, InputType::HELD, i, 0.f, true);
            }
            else if (!isPressed && _gamePadButtonStateMap[i])
            {
                DispatchInput(InputDevice::GAMEPAD, InputType::RELEASE, i, 0.f, false);
            }
			else if (!isPressed && !_gamePadButtonStateMap[i])
			{
				DispatchInput(InputDevice::GAMEPAD, InputType::IDLE, i, 0.f, false);
			}

			_gamePadButtonStateMap[i] = isPressed;
		}

		float gamePadLX = static_cast<float>(_pGamePad->Gamepad.sThumbLX) / GAMEPAD_NORMALIZE;
		if (abs(_pGamePad->Gamepad.sThumbLX) > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
		{
			DispatchInput(InputDevice::GAMEPAD, InputType::AXIS, DIP_LX, gamePadLX);
			_gamePadButtonStateMap[DIP_LX] = true;
		}
		else if(_gamePadButtonStateMap[DIP_LX] == true)
		{
			DispatchInput(InputDevice::GAMEPAD, InputType::RELEASE, DIP_LX, gamePadLX);
			_gamePadButtonStateMap[DIP_LX] = false;
		}

		float gamePadLY = static_cast<float>(_pGamePad->Gamepad.sThumbLY) / GAMEPAD_NORMALIZE;
		if (abs(_pGamePad->Gamepad.sThumbLY) > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
		{
			DispatchInput(InputDevice::GAMEPAD, InputType::AXIS, DIP_LY, -gamePadLY);	
			_gamePadButtonStateMap[DIP_LY] = true;
		}
		else if (_gamePadButtonStateMap[DIP_LY] == true)
		{
			DispatchInput(InputDevice::GAMEPAD, InputType::RELEASE, DIP_LY, -gamePadLY);
			_gamePadButtonStateMap[DIP_LY] = false;
		}

		float gamePadRX = static_cast<float>(_pGamePad->Gamepad.sThumbRX) / GAMEPAD_NORMALIZE;
		if (abs(_pGamePad->Gamepad.sThumbRX) > XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)
		{
			DispatchInput(InputDevice::GAMEPAD, InputType::AXIS, DIP_RX, gamePadRX);
			_gamePadButtonStateMap[DIP_RX] = true;
		}
		else if (_gamePadButtonStateMap[DIP_RX] == true)
		{
			DispatchInput(InputDevice::GAMEPAD, InputType::RELEASE, DIP_RX, gamePadRX);
			_gamePadButtonStateMap[DIP_RX] = false;
		}

		float gamePadRY = static_cast<float>(_pGamePad->Gamepad.sThumbRY) / GAMEPAD_NORMALIZE;
		if (abs(_pGamePad->Gamepad.sThumbRY) > XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)
		{
			DispatchInput(InputDevice::GAMEPAD, InputType::AXIS, DIP_RY, -gamePadRY);
			_gamePadButtonStateMap[DIP_RY] = true;
		}
		else if (_gamePadButtonStateMap[DIP_RY] == true)
		{
			DispatchInput(InputDevice::GAMEPAD, InputType::RELEASE, DIP_RY, -gamePadRY);
			_gamePadButtonStateMap[DIP_RY] = false;
		}

		float gamePadLT = static_cast<float>(_pGamePad->Gamepad.bLeftTrigger);
		if (gamePadLT > XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
		{
			DispatchInput(InputDevice::GAMEPAD, InputType::TRIGGER, DIP_LT, gamePadLT);
			_gamePadButtonStateMap[DIP_LT] = true;
		}
		else if (_gamePadButtonStateMap[DIP_LT] == true)
		{
			DispatchInput(InputDevice::GAMEPAD, InputType::RELEASE, DIP_LT, gamePadLT);
			_gamePadButtonStateMap[DIP_LT] = false;
		}

		float gamePadRT = static_cast<float>(_pGamePad->Gamepad.bRightTrigger);
		if (gamePadRT > XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
		{
			DispatchInput(InputDevice::GAMEPAD, InputType::TRIGGER, DIP_RT, gamePadRT);
			_gamePadButtonStateMap[DIP_RT] = true;
		}
		else if (_gamePadButtonStateMap[DIP_RT] == true)
		{
			DispatchInput(InputDevice::GAMEPAD, InputType::RELEASE, DIP_RT, gamePadRT);
			_gamePadButtonStateMap[DIP_RT] = false;
		}
	}
}

void Engine::InputManager::DispatchInput(InputDevice Device, InputType Type, _uint Key, _float Value, bool State, long x, long y)
{
	_uint uniqueKey = (static_cast<_uint>(Device) << 16 | Key);
	if (_bindKeyMap.find(uniqueKey) != _bindKeyMap.end())
	{
		for (auto& inputComponent : _bindKeyMap[uniqueKey])
		{
			inputComponent->OnInputReceived(InputValue{ Device, Type, uniqueKey, Value, State, x, y });
		}
	}
}

void Engine::InputManager::InitializeKeyboard()
{
	HRESULT hresult = _pInputSDK->CreateDevice(GUID_SysKeyboard, &_pKeyboard, nullptr);
	if(SUCCEEDED(hresult))
	{
		hresult = _pKeyboard->SetDataFormat(&c_dfDIKeyboard);
		if(SUCCEEDED(hresult))
		{
			hresult = _pKeyboard->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
			if(SUCCEEDED(hresult))
			{
				_pKeyboard->Acquire();
				if(FAILED(hresult))
				{
					MessageBox(hWnd, L"Keyboard Acquire() failed", L"Error", MB_OK);
					_pKeyboard->Release();
					_pKeyboard = nullptr;
				}
			}
			else
			{
				MessageBox(hWnd, L"Keyboard SetCooperativeLevel() failed", L"Error", MB_OK);
				_pKeyboard->Release();
				_pKeyboard = nullptr;
			}
		}
		else
		{
			MessageBox(hWnd, L"Keyboard SetDataFormat() failed", L"Error", MB_OK);
			_pKeyboard->Release();
			_pKeyboard = nullptr;
		}
	}
	else
	{
		MessageBox(hWnd, L"CreateDevice() failed", L"Error", MB_OK);
		_pKeyboard->Release();
		_pKeyboard = nullptr;
	}
}

void Engine::InputManager::InitializeMouse()
{
	HRESULT hresult = _pInputSDK->CreateDevice(GUID_SysMouse, &_pMouse, nullptr);
	if(SUCCEEDED(hresult))
	{
		hresult = _pMouse->SetDataFormat(&c_dfDIMouse);
		if(SUCCEEDED(hresult))
		{
			hresult = _pMouse->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
			if(SUCCEEDED(hresult))
			{
				_pMouse->Acquire();
				if(FAILED(hresult))
				{
					MessageBox(hWnd, L"Mouse Acquire() failed", L"Error", MB_OK);
					_pMouse->Release();
					_pMouse = nullptr;
				}
			}
			else
			{
				MessageBox(hWnd, L"Mouse SetCooperativeLevel() failed", L"Error", MB_OK);
				_pMouse->Release();
				_pMouse = nullptr;
			}
		}
		else
		{
			MessageBox(hWnd, L"Mouse SetDataFormat() failed", L"Error", MB_OK);
			_pMouse->Release();
			_pMouse = nullptr;
		}
	}
	else
	{
		MessageBox(hWnd, L"CreateDevice() failed", L"Error", MB_OK);
		_pMouse->Release();
		_pMouse = nullptr;
	}
}

void Engine::InputManager::InitializeGamePad()
{
	_pGamePad = new XINPUT_STATE();
	ZeroMemory(_pGamePad, sizeof(XINPUT_STATE));
	for (_uint i = 0; i < XUSER_MAX_COUNT; ++i)
	{
		DWORD result = XInputGetState(i, _pGamePad);
		if (result == ERROR_SUCCESS)
		{
			_gamePadIndex = i;
			break;
		}
	}
}

void Engine::InputManager::ButtonMapping(_uint key)
{
	_buttonMappings.insert(key);
}

void Engine::InputManager::ButtonUnMapping(_uint key)
{
	_buttonMappings.erase(key);
}

void Engine::InputManager::Destroy()
{
	if (_pGamePad)
    {
		ZeroMemory(_pGamePad, sizeof(XINPUT_STATE));
		delete _pGamePad;
        _pGamePad = nullptr;
    }
}
