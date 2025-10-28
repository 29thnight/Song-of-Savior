#pragma once
#include <Actor.h>

#include "ClientEnum.h"

namespace Engine
{
	CORECLASS();
}

namespace Client
{
	class BackGround : public Engine::Actor
	{
	private:
		explicit BackGround() = default;
		virtual ~BackGround() = default;
	public:
		// Actor을(를) 통해 상속됨
		void BeginPlay() override;
		void Tick(_duration deltaSeconds) override;
		void Fixed() override;
		void EndPlay() override;
		void ReviveInitialize() override;
		void SetStageBackground(Stage stage);
		void ChangeDarkBackground();

	public:
		static BackGround* Create() { return new BackGround; }
		void Destroy() override { Actor::Destroy(); }

	private:
		Engine::TimerComponent*  _pTimerComponent{ nullptr };
		Engine::BitmapComponent* _pPrimaryBitmapComponent{ nullptr };
		Engine::BitmapComponent* _pSecondryBitmapComponent{ nullptr };
		Engine::BoxComponent*	 _pBoxComponent{ nullptr };

	private:
		Stage currentStage{ Stage::Title };
		bool _isDarkChange{ false };
	};
}