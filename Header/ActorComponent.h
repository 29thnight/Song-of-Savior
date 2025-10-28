#pragma once
#include <CoreDefine.h>
#include <Object.h>

namespace Engine
{
	CORECLASS()
	class ActorComponent abstract : public Object
	{
	protected:
		explicit ActorComponent() = default;
		virtual ~ActorComponent() = default;

	public:
		virtual bool InitializeComponent() abstract;
		virtual void BeginPlay() abstract;
		virtual void TickComponent(_duration deltaSeconds) abstract;
		virtual void EndPlay() abstract;

	public:
		virtual void SerializeIn(nlohmann::ordered_json& object) {};
		virtual void SerializeOut(nlohmann::ordered_json& object) {};

	public:
		void Activate(bool reset) { _isActive = reset;}
		bool IsActive() const { return _isActive; }

	public:
		void SetOwner(Actor* owner) { _owner = owner; }
		Actor* GetOwner() const { return _owner; }

	//public:
	//	World* GetWorld() const;

	protected:
		virtual void Destroy() override abstract;

	protected:
		Actor* _owner{ nullptr };
		bool _isActive{ true };
	};
}