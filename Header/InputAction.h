#pragma once
#include <CoreDefine.h>
#include <IInputAction.h>
#include <Mathf.h>

namespace Engine
{
	struct InputActionValue
	{
		InputActionValue() = default;
		InputActionValue(bool value) :BooleanValue(value) {}
		InputActionValue(float value) :FloatValue(value) {}
		InputActionValue(const Mathf::Vector2& value) :Vector2Value(value) {}

		bool BooleanValue{ false };
		float FloatValue{ 0.0f };
		Mathf::Vector2 Vector2Value{ UnitVector::Zero };
	};

	class InputAction abstract :public CoreBase, public IInputAction
	{
	protected:
		explicit InputAction() = default;
		virtual ~InputAction() = default;

	public:
		virtual void Entered() override abstract;
		virtual void Triggers(_duration deltaSeconds) override abstract;
		virtual void Completed() override abstract;

	public:
		virtual void SerializeIn(nlohmann::ordered_json& object) {};
		virtual void SerializeOut(nlohmann::ordered_json& object) {};

	public:
		void SetTriggerValue(const InputActionValue& value) { _triggerValue = value; }
		const InputActionValue& GetTriggerValue() const { return _triggerValue; }

		bool IsEntered() const { return _isEntered; }
		bool IsTriggered() const { return _isTriggered; }
		bool IsCompleted() const { return _isCompleted; }

		void Reset()
		{
			_isEntered = false;
			_isTriggered = false;
			_isCompleted = false;
		}

	public:
		void Destroy() override
		{
			Reset();
		}

	protected:
		bool _isEntered{ false };
		bool _isTriggered{ false };
		bool _isCompleted{ false };

	protected:
		InputActionValue _triggerValue;
	};
}