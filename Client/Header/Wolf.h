#pragma once
#include <MainEntry.h>
#include "Boss.h"


namespace Engine
{
	CORECLASS()
}

namespace Client
{
	class Wolf : public Boss
	{
	private:
		Wolf() = default;
		virtual ~Wolf() = default;

	public:
		virtual void BeginPlay() override;
		virtual void Tick(_duration deltaSeconds) override;
		virtual void Fixed() override;
		virtual void EndPlay() override;
		virtual void ReviveInitialize() override;

	public:
		virtual void SerializeIn(nlohmann::ordered_json& object) override;
		virtual void SerializeOut(nlohmann::ordered_json& object) override;

	public:
		static Wolf* Create();
		void Destroy() override;

	private:
		int _attackHeadcount{};
		Engine::BasicAnimationComponent* _pAttackEffectComponent{ nullptr };

	public:
		void Idle();
		void AttackEnter();
		void AttackUpdate();
		void SoundAttack();
		void Skill();
		void Skill2();
		void Skill2_End();
		void SoundSkill();
		void Damage();
		void Dead();	  

	public:
		void AttackFade();

		void UseSkill1() override;
		void UseSkill2() override;

		void DamegeSkill1();
		void DamegeSkill2();
	};
}


