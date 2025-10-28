#pragma once
#include <Actor.h>

namespace Client
{
	class TimelineCurrNote : public Engine::Actor
	{
		using Base = Engine::Actor;
	private:
		TimelineCurrNote() = default;
		virtual ~TimelineCurrNote() = default;

	public:
		virtual void BeginPlay() override;
		virtual void Tick(_duration deltaSeconds) override;
		virtual void Fixed() override;
		virtual void EndPlay() override;

	public:
		virtual void NotifyActorBlock(Engine::CollisionData data) override;
		virtual void NotifyActorBeginOverlap(Engine::CollisionData data) override;
		virtual void NotifyActorEndOverlap(Engine::CollisionData data) override;

	public:
		virtual void SetupInputComponent(Engine::InputComponent* inputComponent) override;

	public:
		virtual void SerializeIn(nlohmann::ordered_json& object) override;
		virtual void SerializeOut(nlohmann::ordered_json& object) override;

	public:
		static TimelineCurrNote* Create() { return new TimelineCurrNote; }
		void Destroy() override { Actor::Destroy(); }

	private:
		Engine::BitmapComponent* _pBitmapComponent = nullptr;

	};
}