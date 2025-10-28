#pragma once
//플레이어 케릭터 및 보스 케릭터의 기본 클래스
#include <MainEntry.h>
#include <Actor.h>

namespace Engine
{
	CORECLASS()
}

namespace Client
{
	class HPComponent;
	class Character abstract : public Engine::Actor
	{
		using Base = Engine::Actor;

	protected:
		Character() = default;
		virtual ~Character() = default;

	private:
		DECLARE_DELEGATE(OnTakeDamage, int);
		DECLARE_DELEGATE(OnHeal, int);
		DECLARE_DELEGATE(OnChargeMana, int);

	public:
		virtual void BeginPlay() override;
		virtual void Tick(_duration deltaSeconds) override;
		virtual void Fixed() override;
		virtual void EndPlay() override;
		virtual void ReviveInitialize() override abstract;

	public:
		void Destroy() override abstract;
		void Damage(int damage);
		void Heal(int heal);
		void ChargeMana(int mana);
		void ReInitializeHPBar();
		void SetHPIndexBlink(int willBeDecreseHP);
		void CancelHPIndexBlink();
		void SettingKey(_uint skillKey);
		virtual void Dead();
		string GetKeySetting();

	public:
		OnTakeDamage _OnTakeDamage;
		OnHeal		 _OnHeal;
		OnChargeMana _OnChargeMana;

		_Property(bool, IsDead);
		_Get(IsDead) { return _bIsDead; }
		_Set(IsDead) { _bIsDead = value; }

		_Property(int, HP);
		_Get(HP) { return _iHp; }
		_Set(HP) { _iHp = value; }

		_Property(int, MaxHP);
		_Get(MaxHP) { return _iMaxHp; }
		_Set(MaxHP) { _iMaxHp = value; }

		_Property(int, MP);
		_Get(MP) { return _iMp; }
		_Set(MP) { _iMp = value; }

		_Property(int, MaxMP);
		_Get(MaxMP) { return _iMaxMP; }
		_Set(MaxMP) { _iMaxMP = value; }

		ReadOnly_Property(int, AttackDamage);
		_Get(AttackDamage) { return _iAttack; }

		ReadOnly_Property(int, SkillDamage);
		_Get(SkillDamage) { return _iSkill; }

		ReadOnly_Property(bool, IsSkillOn);			//UI에 스킬 사용 가능 여부를 알려주기 위한 프로퍼티
		_Get(IsSkillOn) { return _bIsSkillOn; }

		ReadOnly_Property(int, Type);
		_Get(Type) { return _characterType; }

		//나중에 내립시다
		int _iBlockCount = { -1 };
		long double _ldAttackOffset = { 0 };
		long double _ldSkillOffset = { 0 };

	protected:
		//Engine::AnimationComponent* _pAnimationComponent{ nullptr };
		Engine::BasicAnimationComponent* _pBasicAnimationComponent{ nullptr };
		Engine::StateComponent* _pStateComponent{ nullptr };
		HPComponent* _pHPComponent{ nullptr };

		Engine::BasicAnimationComponent* _pAttackEffectComponent{ nullptr };
		Engine::BasicAnimationComponent* _pSkillEffectComponent{ nullptr };

	protected:
		bool _bIsDead = { false };
		int _iHp = { 0 };
		int _iMaxHp = { 0 };
		int _iMp = { 0 };
		int _iMaxMP = { 0 };
		int _iAttack = { 0 };
		int _iSkill = { 0 };

		bool _bIsSkillOn = { false };	//UI에 스킬 사용 가능 여부를 알려주기 위한 프로퍼티
		int _characterType = { -1 };  //케릭터 타입

	protected:
		_uint _skillKey{ 0 };
		
	};
}

