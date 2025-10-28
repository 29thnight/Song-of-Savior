#pragma once

#include <Actor.h>

namespace Engine
{
	CORECLASS()
}

namespace Client
{
	class SoundBar : public Engine::Actor
	{
		
		using Base = Engine::Actor;
	private:
		SoundBar() = default;
		virtual ~SoundBar() = default;

	public:
		virtual void BeginPlay() override;
		virtual void Tick(_duration deltaSeconds) override;
		virtual void Fixed() override;
		virtual void EndPlay() override;

	public:
		virtual void SetupInputComponent(Engine::InputComponent* inputComponent) override;

	public:
		void SetVisible(bool isVisible);

	public:
		static SoundBar* Create() { return new SoundBar; }
		void Destroy() override { Actor::Destroy(); }

	private:
		Mathf::RectF _volumeBarRect;
		Engine::BitmapComponent* _pCurrVolumeBar{ nullptr };
		Engine::BitmapComponent* _pBaseVolumeBar{ nullptr };

	private:
		bool _isVisible{ false };
	};
}