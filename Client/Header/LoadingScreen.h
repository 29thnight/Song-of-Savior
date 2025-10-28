#pragma once
#include <Actor.h>

#include "ClientEnum.h"
#include <Random.h>

namespace Engine
{
	CORECLASS();
}

namespace Client
{
	class LoadingScreen : public Engine::Actor
	{
	private:
		explicit LoadingScreen() = default;
		virtual ~LoadingScreen() = default;

	public:
		// Actor��(��) ���� ��ӵ�
		void BeginPlay() override;
		void Tick(_duration deltaSeconds) override;
		void Fixed() override;
		void EndPlay() override;
		void ReviveInitialize() override;

	public:
		static LoadingScreen* Create() { return new LoadingScreen; }
		void Destroy() override { Actor::Destroy(); }

	private:
		Engine::TimerComponent* _pTimerComponent{ nullptr };
		Engine::TextRenderComponent* _pLoadingText{ nullptr };
		Engine::BasicAnimationComponent* _pLoadingScreen{ nullptr };

	private:
		void TextChange();

	private:
		int _textPointCount{ 0 };
		Random<int> _random{ 0, 3 };
	};
}