#pragma once
#include <CoreDefine.h>

namespace Engine
{
	interface IInputAction
	{
		virtual void Entered() abstract;
		virtual void Triggers(_duration deltaSeconds) abstract;
		virtual void Completed() abstract;
	};
}