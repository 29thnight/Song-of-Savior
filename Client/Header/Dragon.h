#pragma once
#include <MainEntry.h>
#include "Boss.h"


namespace Engine
{
	CORECLASS()
}

namespace Client
{
	class Dragon : public Boss
	{
	private:
		Dragon() = default;
		virtual ~Dragon() = default;

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
		static Dragon* Create();
		void Destroy() override;

	public:
		void Idle();
		void Attack();
		void SoundAttack();
		void Skill();
		void SoundSkill();
		void Skill2();
		void SoundSkill2();
		void Damage();
		void Dead();

	public:
		void UseSkill1() override {}
		void UseSkill2() override;
		void Skill2Play();

	private:
		Engine::BasicAnimationComponent* _pSkillEffectComponent_2 = nullptr;
		Engine::BasicAnimationComponent* _pSkillEffectComponent_3 = nullptr;
		Engine::BasicAnimationComponent* _pSkillEffectComponent_4 = nullptr;

	private:
		Engine::TimerComponent* _pTimerManager = nullptr;

	public:
		void SpawnSkillEffectTargetSorcerer_magic();
		void SpawnSkillEffectTargetSorcerer_fire();
		void SpawnSkillEffectTargetBard_fire();
		void SpawnSkillEffectTargetBard_magic();

		void DamegeSkill_Bard();
		void DamegeSkill_Sorcerer();

	public:
		void DamegeSkill2();

	};
}


