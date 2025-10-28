#include <Actor.h>
#include <AnimationComponent.h>
#include <StateComponent.h>
#include <BasicAnimationComponent.h>
#include <Delegate.h>
#include <SoundManager.h>
#include <GameManager.h>
#include "Warrior.h"
#include "Thief.h"
#include "Sorcerer.h"
#include "Bard.h"
#include "Boss.h"

void Client::Boss::BeginPlay()
{ 
	Character::BeginPlay();

	_pSkill2EffectComponent = AddComponent<Engine::BasicAnimationComponent>("Skill2EffectComponent");
	_pSkill2EffectComponent->SetTextures(&_vecTextures);

	_OnAttackFail.AddDynamic(this, &Boss::SetAttackFail);
	_OnSkillFail.AddDynamic(this, &Boss::SetSkillFail);
}

void Client::Boss::Tick(_duration deltaSeconds)
{
	Character::Tick(deltaSeconds);
}

void Client::Boss::Fixed()
{
	Character::Fixed();
}

void Client::Boss::EndPlay()
{
	Character::EndPlay();
}

void Client::Boss::Destroy()
{
	Character::Destroy();
}

void Client::Boss::StartPattern(int PatternNumber)
{
	_pStateComponent->SetCurrentState(PatternNumber);
}

long double Client::Boss::GetPattenOffset(int pattenType)
{
	switch (pattenType)
	{
	case 1:		return _ldAttackOffset;
	case 2:		return _ldSkillOffset + _ldSkillCharging;
	case 3:		return _ldBossSkillOffset + _ldBossSkillCharging;
	default:
		return 0;
	}
	
}

void Client::Boss::Dead()
{
	_pBasicAnimationComponent->StartConceal(2.0f);
	_bIsDead = true;
}

void Client::Boss::SetAttackFail(bool isFail)
{
	_bIsAttackFail = isFail;
}

void Client::Boss::SetSkillFail(int _CharacterType)
{
	if (_CharacterType == _iSkillCancelType)
	{
		_bIsSkillFail = true;
	}
	if(_CharacterType == _iBossSkillCancelType)
	{
		_bIsBossSkillFail = true;
	}
}


void Client::Boss::Charging()
{
	if (_pBasicAnimationComponent->IsCurrClip("Charging") || _pBasicAnimationComponent->IsCurrClip("Phase2/Charging"))
	{	
		long double position = Sound->GetPosition(CHANNEL_BGM);
		long double _ldChargingDuration = 0;
		if (_ldChagingTime == 0) 
		{
			_ldChagingTime = Sound->GetPosition(CHANNEL_BGM);
		}
		else 
		{
			_ldChargingDuration = position - _ldChagingTime;
		}

		if (_skillPatten == 1)
		{
			DebugPrintf("Charging : %Lf\n", _ldChargingDuration);
			Manager->UpdateCharacterHPBarBlink(_iSkill);
			_iCharginCancel = _iSkillCancelType;
			_iIndicateCount = _SkillIndicateCount;
			long double countDuration = _ldSkillCharging / (long double)_SkillIndicateCount;
			int degreeIndicate = static_cast<int>(_ldChargingDuration / countDuration);
			_iIndicateCount -= degreeIndicate;
			DebugPrintf("IndicateCount : %d\n", _iIndicateCount);

			if (_bIsSkillFail)
			{
				Manager->CancelCharacterHPBarBlink();
				_pStateComponent->SetCurrentState(0);
				_bIsSkillCharging = false;
				_bIsSkillFail = false;
				_iCharginCancel= -1;
			}

			if (_ldChargingDuration > _ldSkillCharging)
			{
				Manager->CancelCharacterHPBarBlink();
				_bIsSkillCharging = false;
				_ldChagingTime = 0;
				_iCharginCancel = -1;
				UseSkill1();
			}
		}
		else 
		{
			Manager->UpdateCharacterHPBarBlink(_iBossSkill);
			_iCharginCancel = _iBossSkillCancelType;
			_iIndicateCount = _BossSkillIndicateCount;
			long double countDuration = _ldBossSkillCharging / (long double)_BossSkillIndicateCount;
			int degreeIndicate = static_cast<int>(_ldChargingDuration / countDuration);
			_iIndicateCount -= degreeIndicate;

			if (_bIsBossSkillFail)
			{
				Manager->CancelCharacterHPBarBlink();
				_pStateComponent->SetCurrentState(0);
				_bIsSkillCharging = false;
				_bIsBossSkillFail = false;
				_iCharginCancel = -1;
			}

			if (_ldChargingDuration > _ldBossSkillCharging)
			{
				Manager->CancelCharacterHPBarBlink();
				_bIsSkillCharging = false;
				_ldChagingTime = 0;
				_iCharginCancel = -1;
				UseSkill2();
			}
		}
	}
}