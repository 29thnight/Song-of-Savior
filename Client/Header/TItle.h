#pragma once
#include <Actor.h>

namespace Engine
{
	CORECLASS();
}

namespace Client
{
	class Title : public Engine::Actor
	{
	private:
		Title() = default;
		virtual ~Title() = default;

	public:
		void BeginPlay() override;
		void Tick(_duration deltaSeconds) override;
		void Fixed() override;
		void EndPlay() override;
		void SetupInputComponent(Engine::InputComponent* inputComponent) override;

	public:
		static Title* Create() { return new Title; }
		void Destroy() override;

	private:
		Engine::BitmapComponent* _pBackgroundComponent{ nullptr };
		//Engine::BitmapComponent* _pRayFilterComponent{ nullptr };
		//Engine::BitmapComponent* _pTitleComponent{ nullptr };
		Engine::InputComponent* _pInputComponent{ nullptr };
		Engine::BasicAnimationComponent* _pAnimationComponent{ nullptr };

	private:
		bool _isSceneChangeTrigger{ false };
		bool _isVideoSkip{ false };
	};
};