#pragma once
#include <MainEntry.h>
#include "Boss.h"


namespace Engine
{
	CORECLASS()
}

namespace Client
{
	class Lich : public Boss
	{
	private:
		Lich() = default;
		virtual ~Lich() = default;

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
		static Lich* Create();
		void Destroy() override;

		virtual void StartPattern(int PatternNumber) override;

	public:
		void Idle();

		void AttackEnter();
		void AttackUpdate();

		void SoundAttack();

		void SkillEnter();
		void SkillUpdate();

		void SoundSkill();

		void Skill2Enter();

		void SoundSkill2();

		void Damage();
		void Dead();

	private:

		Engine::BasicAnimationComponent* _pAttackEffectComponent_2 = nullptr;
		Engine::BasicAnimationComponent* _pAttackEffectComponent_3 = nullptr;
		Engine::BasicAnimationComponent* _pAttackEffectComponent_4 = nullptr;



		Engine::BasicAnimationComponent* _pSkillEffectComponent_2 = nullptr; 
		Engine::BasicAnimationComponent* _pSkillEffectComponent_3 = nullptr;
		Engine::BasicAnimationComponent* _pSkillEffectComponent_4 = nullptr;

		//2페이즈 스킬1
		Engine::BasicAnimationComponent* _pSkillEffectComponent_5 = nullptr;
		Engine::BasicAnimationComponent* _pSkillEffectComponent_8 = nullptr;
		//2페이즈 스킬2
		Engine::BasicAnimationComponent* _pSkillEffectComponent_6 = nullptr;
		Engine::BasicAnimationComponent* _pSkillEffectComponent_7 = nullptr;

		Engine::BasicAnimationComponent* _pSkill2EffectComponent_2 = nullptr;
		Engine::BasicAnimationComponent* _pSkill2EffectComponent_3 = nullptr;
		Engine::BasicAnimationComponent* _pSkill2EffectComponent_4 = nullptr;
		Engine::BasicAnimationComponent* _pSkill2EffectComponent_5 = nullptr;
	
		Engine::TimerComponent* _pTimerManager = nullptr;

	private:
		Random<int>	  _random_03{0,3};
		int _randomTargetArray[4]{ 0, 1, 2, 3 };
		int _currTargetIndex{};

		int lightningCount{};
		long double lightningStartPosition{};

		bool _isUseSkill2 = false;

		int _currPhase = 1;
	private:
		void MixRandomTargetArray();

	public:
		void SetPhase2();
		void SetPhase1();

	public:
		void SpawnSkillEffectTargetSorcerer_magic();
		void SpawnSkillEffectTargetBard_magic();
		void SpawnSkillEffectTargetSorcerer_fire();
		void SpawnSkillEffectTargetBard_fire();

		void DamegeSkill_Bard();
		void DamegeSkill_Sorcerer();
	public:
		void SpawnSkill2Effect_light1();
		void SpawnSkill2Effect_light2();
		void SpawnSkill2Effect_light3();

		void DamegeSkill2_light();

	public:
		void DamegeSkill1_wolf();
		void Skill1_wolfEnd();
		void DamegeSkill2_fire();
		void Skill2_fireEnd();

	public:
		void UseSkill1() override;
		void UseSkill2() override;

	//Attack Effect
	public:
		void SpawnAttackEffect();

	};
}


