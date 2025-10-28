#pragma once
#include <CoreDefine.h>

namespace Engine
{
	class InputComponent;
	interface ISetupInputComponent
	{
		virtual void SetupInputComponent(InputComponent* inputComponent) abstract;
	};
}