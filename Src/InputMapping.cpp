#include "InputMapping.h"
#include <InputComponent.h>

void Engine::InputMapping::ChangeAction(IInputAction* inputAction)
{
	if (_currentAction != nullptr)
	{
		_currentAction->Completed();
	}

	_currentAction = inputAction;
	_currentAction->Entered();
}

void Engine::InputMapping::CompareAndTriggerAction(_uint key, InputType type, _duration deltaSeconds)
{
    for (auto& action : _actions)
    {
        if (_currentAction != nullptr && action == _currentAction)
        {
            if (type == InputType::PRESS && _inputComponent->IsKeyPress(key))
            {
                action->Triggers(deltaSeconds);
            }
            else if (type == InputType::HELD && _inputComponent->IsKeyHold(key))
            {
                action->Triggers(deltaSeconds);
            }
            else if (type == InputType::RELEASE && _inputComponent->IsKeyRelease(key))
            {
                action->Triggers(deltaSeconds);
            }
        }
    }
}

void Engine::InputMapping::Destroy()
{
}
