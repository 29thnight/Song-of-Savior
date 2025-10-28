#include <CoreDefine.h>
#include <InputManager.h>
#include <InputComponent.h>
#include <Actor.h>
#include <TimeManager.h>
#include <SoundManager.h>

bool Engine::InputComponent::InitializeComponent()
{
	_owner->SetupInputComponent(this);

	return true;
}

void Engine::InputComponent::TickComponent(_duration deltaSeconds)
{
	if (_inputEvents.empty())
		return;

	if (_inputMapping)
	{
		_inputMapping->GetCurrentAction()->Triggers(deltaSeconds);
	}

	if (9999.f > elapsedTick)
	{
		elapsedTick += Time->NonScaleDurationToFloat();
	}
}

void Engine::InputComponent::OnInputReceived(InputValue value)
{
	if (_owner->IsDestroyMarked())
		return;

	if(value.type == InputType::IDLE)
		return;

	if (!_inputEvents.empty())
	{
		_inputEvents.back().timeToLastInput = elapsedTick;
	}

	elapsedTick = 0.f;

	_inputEvents.push_back(value);

	if (_inputMapping)
	{
		_inputMapping->MappingContext(value);
	}

	if(_inputEvents.size() > 256)
	{
		_inputEvents.pop_front();
	}
}

void Engine::InputComponent::BindKey(_uint key)
{
	_uniqueKey.emplace_back(std::make_pair(key, this));
}

void Engine::InputComponent::BindKeyAll()
{
	{
		BindKeys(
			0x01,
			0x02,
			0x03,
			0x04,
			0x05,
			0x06,
			0x07,
			0x08,
			0x09,
			0x0A,
			0x0B,
			0x0C,
			0x0D,
			0x0E,
			0x0F,
			0x10,
			0x11,
			0x12,
			0x13,
			0x14,
			0x15,
			0x16,
			0x17,
			0x18,
			0x19,
			0x1A,
			0x1B,
			0x1C,
			0x1D,
			0x1E,
			0x1F,
			0x20,
			0x21,
			0x22,
			0x23,
			0x24,
			0x25,
			0x26,
			0x27,
			0x28,
			0x29,
			0x2A,
			0x2B,
			0x2C,
			0x2D,
			0x2E,
			0x2F,
			0x30,
			0x31,
			0x32,
			0x33,
			0x34,
			0x35,
			0x36,
			0x37,
			0x38,
			0x39,
			0x3A,
			0x3B,
			0x3C,
			0x3D,
			0x3E,
			0x3F,
			0x40,
			0x41,
			0x42,
			0x43,
			0x44,
			0x45,
			0x46,
			0x47,
			0x48,
			0x49,
			0x4A,
			0x4B,
			0x4C,
			0x4D,
			0x4E,
			0x4F,
			0x50,
			0x51,
			0x52,
			0x53,
			0x56,
			0x57,
			0x58,
			0x64,
			0x65,
			0x66,
			0x70,
			0x73,
			0x79,
			0x7B,
			0x7D,
			0x7E,
			0x8D,
			0x90,
			0x91,
			0x92,
			0x93,
			0x94,
			0x95,
			0x96,
			0x97,
			0x99,
			0x9C,
			0x9D,
			0xA0,
			0xA1,
			0xA2,
			0xA4,
			0xAE,
			0xB0,
			0xB2,
			0xB3,
			0xB5,
			0xB7,
			0xB8,
			0xC5,
			0xC7,
			0xC8,
			0xC9,
			0xCB,
			0xCD,
			0xCF,
			0xD0,
			0xD1,
			0xD2,
			0xD3,
			0xDB,
			0xDC,
			0xDD,
			0xDE,
			0xDF,
			0xE3,
			0xE5,
			0xE6,
			0xE7,
			0xE8,
			0xE9,
			0xEA,
			0xEB,
			0xEC,
			0xED,
			DIP_B,
			DIP_A,
			DIP_X,
			DIP_Y,
			DIP_DPAD_DOWN,
			DIP_DPAD_LEFT,
			DIP_DPAD_RIGHT,
			DIP_DPAD_UP,
			DIP_LEFT_SHOULDER,
			DIP_RIGHT_SHOULDER,
			DIP_LT,
			DIP_RT,
			DIP_LEFT_SHOULDER,
			DIP_RIGHT_SHOULDER,
			DIP_LEFT_THUMB,
			DIP_RIGHT_THUMB,
			DIP_MENU
		);
	}	
}

void Engine::InputComponent::UnbindAll()
{
	for (auto& key : _uniqueKey)
	{
		InputMgr->UnRegisterComponent(key.first, key.second);
	}

	_uniqueKey.clear();
	_inputActions.clear();
	_inputEvents.clear();
	_inputMapping = nullptr;
	elapsedTick = 0.f;
}

void Engine::InputComponent::SetInputMapping(InputMapping* inputMapping)
{
	_inputMapping = inputMapping;
	_inputMapping->SetInputComponent(this);
}

void Engine::InputComponent::AttachInputManager()
{
	for (auto& key : _uniqueKey)
	{
		InputMgr->RegisterComponent(key.first, key.second);
	}
}

void Engine::InputComponent::SetVibration(_float leftMotor, _float rightMotor, _float duration)
{
	_vibration.wLeftMotorSpeed = static_cast<WORD>(leftMotor * USHRT_MAX);
	_vibration.wRightMotorSpeed = static_cast<WORD>(rightMotor * USHRT_MAX);
	_vibrationDuration = duration;
}

void Engine::InputComponent::InvokeVibration(_duration deltaSeconds)
{
	if (_vibrationDuration > 0.f)
	{
		_vibrationDuration -= Time->DurationToFloat(deltaSeconds);
	}
	else
	{
		_vibration.wLeftMotorSpeed = 0;
		_vibration.wRightMotorSpeed = 0;
	}

	XInputSetState(0, &_vibration);
}

Mathf::Vector2 Engine::InputComponent::IsLeftAxisMove() const
{
	if (_inputEvents.empty())
		return UnitVector::Zero;

	float leftStickX{};
	float leftStickY{};

	for (auto iter = _inputEvents.rbegin(); iter != _inputEvents.rend(); iter++)
	{
		if (0 != iter->timeToLastInput)
		{
			break;
		}
		else if (iter->type == InputType::AXIS)
		{
			if(DIP_LX == iter->key)
			{
				leftStickX = iter->value;
			}

			if (DIP_LY == iter->key)
			{
				leftStickY = iter->value;
			}

			//if(0.f == leftStickX || 0.f == leftStickY)
			//{
			//	continue;
			//}

			//DebugPrintf("%f, %f\n", leftStickX, leftStickY);
			return Mathf::Vector2(leftStickX,leftStickY);
		}
	}

	return UnitVector::Zero;
}

Mathf::Vector2 Engine::InputComponent::IsRightAxisMove() const
{
	if (_inputEvents.empty())
		return UnitVector::Zero;

	float rightStickX{};
	float rightStickY{};

	for (auto iter = _inputEvents.rbegin(); iter != _inputEvents.rend(); iter++)
	{
		if (0 != iter->timeToLastInput)
		{
			break;
		}
		else if (iter->type == InputType::AXIS)
		{
			if (DIP_RX == iter->key)
			{
				rightStickX = iter->value;
			}

			if (DIP_RY == iter->key)
			{
				rightStickY = iter->value;
			}

			//if (0.f == rightStickX || 0.f == rightStickY)
			//{
			//	continue;
			//}

			//DebugPrintf("%f, %f\n", rightStickX, rightStickY);
			return Mathf::Vector2(rightStickX, rightStickY);
		}
	}

	return UnitVector::Zero;
}

bool Engine::InputComponent::IsKeyPress(_uint key) const
{
	if (_inputEvents.empty())
		return false;

	for (auto iter = _inputEvents.rbegin(); iter != _inputEvents.rend(); ++iter)
	{
		if (0 != iter->timeToLastInput)
		{
			break;
		}
		else if(iter->key == key && iter->type == InputType::PRESS)
		{
			return true;
		}
	}

	return false;
}

bool Engine::InputComponent::IsKeyHold(_uint key) const
{
	if (_inputEvents.empty())
		return false;

	for (auto iter = _inputEvents.rbegin(); iter != _inputEvents.rend(); iter++)
	{
		if (0 != iter->timeToLastInput)
		{
			break;
		}
		else if (iter->key == key && iter->type == InputType::HELD)
		{
			return true;
		}
	}

	return false;
}

bool Engine::InputComponent::IsKeyRelease(_uint key) const
{
	if (_inputEvents.empty())
		return false;

	for (auto iter = _inputEvents.rbegin(); iter != _inputEvents.rend(); iter++)
	{
		if (0 != iter->timeToLastInput)
		{
			break;
		}
		else if (iter->key == key && iter->type == InputType::RELEASE)
		{
			return true;
		}
	}

	return false;
}

bool Engine::InputComponent::IsKeyPressAnyKey() const
{
	if (_inputEvents.empty())
		return false;

	for (auto iter = _inputEvents.rbegin(); iter != _inputEvents.rend(); iter++)
	{
		if (0 != iter->timeToLastInput)
		{
			break;
		}
		else if (iter->type == InputType::PRESS)
		{
			return true;
		}
	}

	return false;
}

Engine::InputComponent::MousePosition Engine::InputComponent::GetMousePosition()
{
	if (_inputEvents.empty())
		return {0, 0};

	for (auto iter = _inputEvents.rbegin(); iter != _inputEvents.rend(); iter++)
	{
		if (0 != iter->timeToLastInput)
		{
			break;
		}
		else if (iter->type == InputType::MOVE)
		{
			_mousePosition.x = iter->x;
			_mousePosition.y = iter->y;

			//DebugPrintf("inputComponent : %ld,%ld\n", _mousePosition.x, _mousePosition.y);

			return _mousePosition;
		}
	}

	return { 0, 0 };
}

float Engine::InputComponent::GetMouseWheel()
{
	if (_inputEvents.empty())
		return 0.0f;

	for (auto iter = _inputEvents.rbegin(); iter != _inputEvents.rend(); ++iter)
	{
		if (0 != iter->timeToLastInput)
		{
			break;
		}
		else if (iter->type == InputType::SCROLL)
		{			 
			return iter->value;
		}
	}

	return 0.0f;
}

bool Engine::InputComponent::IsKeyEventTriggerNow(_uint key, InputType type) const
{
	if (_inputEvents.empty())
		return false;

	if (_inputEvents.back().timeToLastInput > 0.f)
		return false;

	if (_inputEvents.back().key == key && _inputEvents.back().type == type)
	{
		return true;
	}

	return false;
}

bool Engine::InputComponent::IsKeyEventTriggered(_uint key, InputType type) const
{
	if (_inputEvents.empty())
		return false;

	for (auto& inputEvent : _inputEvents)
	{
		if (inputEvent.key == key && inputEvent.type == type)
			return true;
	}

	return false;
}

bool Engine::InputComponent::IsLastInputOverTime(_float time) const
{
	if (_inputEvents.empty())
		return false;

	if (0.f == _inputEvents.back().timeToLastInput && elapsedTick > time)
	{
		return true;
	}

	return false;
}

bool Engine::InputComponent::IsKeyEventTriggeredOverTime(_uint key, InputType type, _float time) const
{
	if (_inputEvents.empty())
		return false;

	for (auto iter = _inputEvents.rbegin();
		iter != _inputEvents.rend(); iter++)
	{
		if (iter->key == key && iter->type == type)
		{
			return elapsedTick > time;
		}
	}

	return false;
}

bool Engine::InputComponent::IsKeyEventTriggeredLessTime(_uint key, InputType type, _float time) const
{
	if (_inputEvents.empty())
		return false;

	for (auto iter = _inputEvents.rbegin();
		iter != _inputEvents.rend(); iter++)
	{
		if (iter->key == key && iter->type == type)
		{
			return elapsedTick < time;
		}
	}

	return false;
}

void Engine::InputComponent::TriggerAction(IInputAction* action, TriggerEvent event, const InputActionValue& value)
{
	for (auto& [key, function] : _inputActions) 
    {
		std::tuple<IInputAction*, TriggerEvent, void*> key = key;
        if (std::get<0>(key) == action && std::get<1>(key) == event)
        {
            function(value);
        }
    }
}

Engine::InputComponent* Engine::InputComponent::Create()
{
	return new InputComponent;
}

void Engine::InputComponent::Destroy()
{
	for (auto& key : _uniqueKey)
	{
		InputMgr->UnRegisterComponent(key.first, key.second);
	}

	_uniqueKey.clear();
	_inputActions.clear();
	_inputEvents.clear();
	_inputMapping = nullptr;
	elapsedTick = 0.f;
}
