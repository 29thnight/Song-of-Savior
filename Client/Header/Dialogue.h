#pragma once
#include <Actor.h>

namespace Engine
{
	CORECLASS();
}

namespace Client
{
	class Dialogue : public Engine::Actor
	{
	private:
		Dialogue() = default;
		virtual ~Dialogue() = default;

	public:
		void BeginPlay() override;
		void Tick(_duration deltaSeconds) override;
		void Fixed() override;
		void EndPlay() override;
		void SetupInputComponent(Engine::InputComponent* inputComponent) override;

	public:
		void Korean();
		void English();
		void Present();
		void ChainDialogue();
		void ChainCameraShake();
		void ChainWhiteFade();
		void ChangeBossAnimation();
		void EnterPhaseTwo();
		bool IsDialogueEnd() const { return _isDialogueEnd; }

	public:
		static Dialogue* Create() { return new Dialogue; }
		void Destroy() override;

	private:
		Engine::BasicAnimationComponent* _pBossAnimationComponent{ nullptr };
		Engine::BasicAnimationComponent* _pChangeBossAnimationComponent{ nullptr };
		Engine::TimerComponent*      _pTimerComponent{ nullptr };
		Engine::BitmapComponent*     _pDialogueComponent{ nullptr };
		Engine::TextRenderComponent* _pTextComponent{ nullptr };

	private:
		bool _isDialogueEnd{ false };
	};
}
