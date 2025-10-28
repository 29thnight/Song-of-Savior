#pragma once
#include <Actor.h>

namespace Client
{
	class Credit : public Engine::Actor
	{
		using Base = Engine::Actor;
	protected:
		Credit() = default;
		virtual ~Credit() = default;
	public:
		virtual void BeginPlay() override;
		virtual void Tick(_duration deltaSeconds) override;
		virtual void Fixed() override;
		virtual void EndPlay() override;
		void ChainCraditIndex();
		void SetBgmPlay(bool isBgmPlay) { this->isBgmPlay = isBgmPlay; }

	public:
		static Credit* Create() { return new Credit; }
		void Destroy() override;

	private:
		Engine::TimerComponent* _pTimerComponent{ nullptr };
		Engine::BitmapComponent* _pBackgroundComponent{ nullptr };

	private:
		bool isSkip{ false };
		bool isBgmPlay{ false };
	};
}