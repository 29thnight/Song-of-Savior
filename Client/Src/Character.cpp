#include <AnimationComponent.h>
#include <BasicAnimationComponent.h>
#include <StateComponent.h>
#include "Boss.h"
#include "Character.h"
#include "HPComponent.h"

void Client::Character::BeginPlay()
{
	Base::BeginPlay();

	_pBasicAnimationComponent = AddComponent<Engine::BasicAnimationComponent>("BasicAnimationComponent");
	_pAttackEffectComponent = AddComponent<Engine::BasicAnimationComponent>("AttackEffectComponent");
	_pSkillEffectComponent = AddComponent<Engine::BasicAnimationComponent>("SkillEffectComponent");

	_OnTakeDamage.AddDynamic(this, &Character::Damage);
	_OnHeal.AddDynamic(this, &Character::Heal);
	_OnChargeMana.AddDynamic(this, &Character::ChargeMana);
}

void Client::Character::Tick(_duration deltaSeconds)
{
	Base::Tick(deltaSeconds);
}

void Client::Character::Fixed()
{
}

void Client::Character::EndPlay()
{
}

void Client::Character::ReviveInitialize()
{
	_pBasicAnimationComponent->SetOpacity(1.f);
}

void Client::Character::Destroy()
{
	Base::Destroy();
}

void Client::Character::Damage(int damage)
{
	_pBasicAnimationComponent->SetAlphaMaskEffect(true);
	_pBasicAnimationComponent->SetAlphaMaskOpacity(0.5f);
	_pBasicAnimationComponent->SetAlphaMaskDuration(0.2f);

	_iHp -= damage;
	if (_iHp <= 0)
	{
		_iHp = 0;
		if (dynamic_cast<Boss*>(this) == nullptr) {
			Dead();
		}
	}

	DebugPrintf("%s take damage : %d\n", _name.c_str(), damage);
	DebugPrintf("%s hp / Max hp : %ld / %ld\n", _name.c_str(), _iHp, _iMaxHp);
}
void Client::Character::Heal(int heal)
{
	_iHp += heal;
	if (_iHp > _iMaxHp)
	{
		_iHp = _iMaxHp;
	}

	DebugPrintf("%s heal : %d\n", _name.c_str(), heal);
	DebugPrintf("%s hp / Max hp : %ld / %ld\n", _name.c_str(), _iHp, _iMaxHp);
}

void Client::Character::ChargeMana(int mana)
{
	_iMp += mana;
	if (_iMp > _iMaxMP)
	{
		_iMp = _iMaxMP;
	}

	DebugPrintf("%s charge mana : %d\n", _name.c_str(), mana);
	DebugPrintf("%s mp / Max mp : %ld / %ld\n", _name.c_str(), _iMp, _iMaxMP);
}

void Client::Character::ReInitializeHPBar()
{
	_pHPComponent->ReInitializeComponent();
}

void Client::Character::SetHPIndexBlink(int willBeDecreseHP)
{
	_pHPComponent->SetHPIndexBlink(willBeDecreseHP);
}

void Client::Character::CancelHPIndexBlink()
{
	_pHPComponent->CancelHPIndexBlink();
}

void Client::Character::SettingKey(_uint skillKey)
{
	_skillKey = skillKey;
}

void Client::Character::Dead()
{
	_pBasicAnimationComponent->StartConceal(1.0f);
	_bIsDead = true;
	DebugPrintf("%s is dead\n", _name.c_str());
}

string Client::Character::GetKeySetting()
{
	switch (_skillKey)
	{
	case DIK_Q:
		return "Q";
		break;
	case DIK_W:
		return "W";
		break;
	case DIK_E:
		return "E";
		break;
	case DIK_R:
		return "R";
		break;
	case DIK_UP:
	case DIP_DPAD_UP:
		return "ก่";
		break;
	case DIK_DOWN:
	case DIP_DPAD_DOWN:
		return "ก้";
		break;
	case DIK_LEFT:
	case DIP_DPAD_LEFT:
		return "ก็";
		break;
	case DIK_RIGHT:
	case DIP_DPAD_RIGHT:
		return "กๆ";
		break;
	default:
		break;
	}
}
