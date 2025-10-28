#pragma once
#include <Actor.h>
#include <Random.h>

namespace Engine
{
	CORECLASS()
}

namespace Client
{
	class BossManager;
	class BossHP : public Engine::Actor
	{
		friend class BossManager;
		using Base = Engine::Actor;
	private:
		BossHP() = default;
		virtual ~BossHP() = default;

	public:
		virtual void BeginPlay() override;
		virtual void Tick(_duration deltaSeconds) override;
		virtual void Fixed() override;
		virtual void EndPlay() override;

	public:
		void SetHeadBitmap();
		void SetIndicatorText(int ChargingCount);
		void SetVisibleIndicator(bool bVisible);
		void UpdateHpScale();

	public:
		static BossHP* Create() { return new BossHP; }
		void Destroy() override { Actor::Destroy(); }

	private:
		Engine::BitmapComponent* _pCurrHPBitmap{ nullptr };
		Engine::BitmapComponent* _pFullHPBitmap{ nullptr };
		Engine::BitmapComponent* _pBossHeadBitmap{ nullptr };
		Engine::BitmapComponent* _pIndicatorBitmap{ nullptr };

		Engine::TextRenderComponent* _pIndicatorText{ nullptr };

		BossManager* _pBossManager{ nullptr };
		Random<float> _randomAngle{ -30.f, 30.f };
	};
}