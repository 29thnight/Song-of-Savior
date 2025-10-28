#include <Actor.h>
#include <AnimationComponent.h>
#include <BasicAnimationComponent.h>
#include <SimpleIniLoader.h>
#include <StateComponent.h>
#include <World.h>
#include <SoundManager.h>
#include <CameraActor.h>
//#include <InputComponent.h>
#include "GameManager.h"
#include "Wolf.h"

constexpr float oneNoteSec = 1.084337349f;

void Client::Wolf::BeginPlay()
{
	Boss::BeginPlay();

	std::string absolutePath = "Assets/Wolf/WolfData.ini";
	SimpleIniFile iniFile;
	try
	{
		iniFile.Load(absolutePath.c_str());
	}
	catch (const std::runtime_error& e)
	{
		std::cerr << "Error loading INI file: " << e.what() << std::endl;
		return;
	}

	_iMaxHp = std::stoi(iniFile.GetValue("WolfData", "_iMaxHp", "0"));			 //보스 총 체력
	_iAttack = std::stoi(iniFile.GetValue("WolfData", "_iAttack", "0"));		 //보스 일반 공격	력
	_iSkill = std::stoi(iniFile.GetValue("WolfData", "_iSkill", "0"));			 //보스 1스킬 공격력
	_iBossSkill = std::stoi(iniFile.GetValue("WolfData", "_iBossSkill", "0"));	 //보스 2스킬 공격력

	//_ldAttackOffset = std::stof(iniFile.GetValue("WolfData", "_ldAttackOffset", "0"));			//일반공격 모션 오프셋
	//_ldSkillOffset = std::stof(iniFile.GetValue("WolfData", "_ldSkillOffset", "0"));			//스킬 모션 오프셋
	//_ldBossSkillOffset = std::stof(iniFile.GetValue("WolfData", "_ldBossSkillOffset", "0"));	//보스스킬 모션 오프셋

	_ldSkillCharging = std::stof(iniFile.GetValue("WolfData", "_ldSkillCharging", "0")) * (long double)1e9;		//1스킬 차징시간
	_ldBossSkillCharging = std::stof(iniFile.GetValue("WolfData", "_ldBossSkillCharging", "0")) * (long double)1e9;	//2스킬 차징시간

	//0: 전사 1: 도적 2: 소서러 3: 바드 4:전체 5: 캔슬 불가
	_iSkillCancelType = std::stoi(iniFile.GetValue("WolfData", "_iSkillCancelType", "0")); //일반스킬 로그 캔슬
	_iBossSkillCancelType = std::stoi(iniFile.GetValue("WolfData", "_iBossSkillCancelType", "0"));	 //보스스킬 소서러 캔슬

	_iHp = _iMaxHp;

	_SkillIndicateCount = 1;
	_BossSkillIndicateCount = 2;

	_iSkillCancelType = 1; //일반스킬 로그 캔슬
	_iBossSkillCancelType = 1; //보스스킬 로그 캔슬

	_pBasicAnimationComponent->AllAddClipThisActor();
	_pBasicAnimationComponent->SetPlayClip("Idle");

	//_pBasicAnimationComponent->AddFrameEvent("Attack" ,1, this,&Wolf::SoundAttack);

	_pBasicAnimationComponent->AddFrameLambda("Skill1", 10, [&]() {
				Sound->PlaySound("Wolves_Skill1", (int)SoundChannel::CHANNEL_SFX_5, SoundLoop::NOT_LOOP);
				GetWorld()->GetCameraActor()->CameraShake(0.8f, 4.f);
		});

	_pBasicAnimationComponent->AddFrameLambda("Skill2", 8, [&]() {
				GetWorld()->GetCameraActor()->CameraShake(0.8f, 4.f);
		});

	_pBasicAnimationComponent->AddFrameEvent("Skill1", _pBasicAnimationComponent->GetClipMaxframe("Skill1") - 1, this, &Wolf::DamegeSkill1);

	_pBasicAnimationComponent->AddFrameEvent("Skill2", 15, this, &Wolf::DamegeSkill2);
	_pSkillEffectComponent = AddComponent<Engine::BasicAnimationComponent>("Skill2Effect");
	_pSkillEffectComponent->AllAddClipThisActor();

	_pAttackEffectComponent = AddComponent<Engine::BasicAnimationComponent>("AttackEffectComponent");
	_pAttackEffectComponent->AllAddClipThisActor();
	_pAttackEffectComponent->SetRelativeScale({ -0.8f, -0.8f });
	_pAttackEffectComponent->AddFrameEvent("Effect/attack_vfx", _pAttackEffectComponent->GetClipMaxframe("Effect/attack_vfx") - 3, this, &Wolf::AttackFade);

	_ldAttackOffset = _pBasicAnimationComponent->GetClipTime("Attack") * Mathf::SecToNano;
	_ldSkillOffset = _pBasicAnimationComponent->GetClipTime("Skill1") * Mathf::SecToNano;
	_ldBossSkillOffset = _pBasicAnimationComponent->GetClipTime("Skill2") * Mathf::SecToNano;

	_pStateComponent = AddComponent<Engine::StateComponent>("StateComponent");

	_pStateComponent->AddState(0, "Idle");
	_pStateComponent->AddState(1, "Attack");
	_pStateComponent->AddState(2, "Skill");
	_pStateComponent->AddState(3, "Skill2");
	_pStateComponent->AddState(4, "Damage");
	_pStateComponent->AddState(5, "Dead");

	_pStateComponent->GetState(0)->OnEnter.AddDynamic(this, &Wolf::Idle);

	_pStateComponent->GetState(1)->OnEnter.AddDynamic(this, &Wolf::AttackEnter);
	_pStateComponent->GetState(1)->OnUpdate.AddDynamic(this, &Wolf::AttackUpdate);

	_pStateComponent->GetState(2)->OnEnter.AddDynamic(this, &Wolf::Skill);
	_pStateComponent->GetState(2)->OnUpdate.AddDynamic(this, &Wolf::Charging);

	_pStateComponent->GetState(3)->OnEnter.AddDynamic(this, &Wolf::Skill2);
	_pStateComponent->GetState(3)->OnUpdate.AddDynamic(this, &Wolf::Charging);

	_pStateComponent->GetState(4)->OnEnter.AddDynamic(this, &Wolf::Damage);
	_pStateComponent->GetState(5)->OnEnter.AddDynamic(this, &Wolf::Dead);

}

void Client::Wolf::Tick(_duration deltaSeconds)
{
	Boss::Tick(deltaSeconds);
	
	if (_pStateComponent->GetCurrentState() == _pStateComponent->GetState(3))
	{
		if (_pBasicAnimationComponent->IsCurrClip("Skill2") && _pBasicAnimationComponent->IsClipEnd("Skill2"))
		{
			StartPattern(0);
		}
	}
}

void Client::Wolf::Fixed()
{
}

void Client::Wolf::EndPlay()
{
}

void Client::Wolf::ReviveInitialize()
{
}

void Client::Wolf::SerializeIn(nlohmann::ordered_json& object)
{
}

void Client::Wolf::SerializeOut(nlohmann::ordered_json& object)
{
}

Client::Wolf* Client::Wolf::Create()
{
	return new Wolf;
}

void Client::Wolf::Destroy()
{
}

void Client::Wolf::Idle()
{
	_pBasicAnimationComponent->SetPlayClip("Idle");
}

void Client::Wolf::AttackEnter()
{
	switch (_attackHeadcount)
	{
	case 0:
		_pBasicAnimationComponent->SetPlayClip("Attack");
		Sound->PlaySound("Wolves_attack", (int)SoundChannel::CHANNEL_SFX_5, SoundLoop::NOT_LOOP);
		_pAttackEffectComponent->SetOpacity(1.f);
		_pAttackEffectComponent->SetPlayClip("Effect/attack_vfx");
		_attackHeadcount++;
		break;
	case 1:
		_pBasicAnimationComponent->SetPlayClip("Attack");
		Sound->PlaySound("Wolves_attack", (int)SoundChannel::CHANNEL_SFX_5, SoundLoop::NOT_LOOP);
		_pAttackEffectComponent->SetOpacity(1.f);
		_pAttackEffectComponent->SetPlayClip("Effect/attack_vfx");
		_attackHeadcount++;
		break;
	case 2:
		_pBasicAnimationComponent->SetPlayClip("Attack");
		Sound->PlaySound("Wolves_attack", (int)SoundChannel::CHANNEL_SFX_5, SoundLoop::NOT_LOOP);
		_pAttackEffectComponent->SetOpacity(1.f);
		_pAttackEffectComponent->SetPlayClip("Effect/attack_vfx");
		_attackHeadcount = 0;
		break;
	}
}

void Client::Wolf::AttackUpdate()
{
	if (_pBasicAnimationComponent->IsFrameEnd())
	{
		StartPattern(0);
		Manager->DealDamageToCharacter(CharacterType::Warrior, _iAttack);
		DebugPrintf("Wolf Attack!!!\n");
	}
}

void Client::Wolf::SoundAttack()
{
	Sound->PlaySound("Wolves_attack", (int)SoundChannel::CHANNEL_SFX_5, SoundLoop::NOT_LOOP);
}

void Client::Wolf::Skill()
{
	_skillPatten = 1;
	Manager->AddHPBarBlinkTarget((int)CharacterType::Warrior);
	Manager->AddHPBarBlinkTarget((int)CharacterType::Thief);
	_pBasicAnimationComponent->SetPlayClip("Charging");
	_ldChagingTime = 0;
}

void Client::Wolf::Skill2()
{
	_skillPatten = 2;
	Manager->AddHPBarBlinkTarget((int)CharacterType::Warrior);
	Manager->AddHPBarBlinkTarget((int)CharacterType::Thief);
	Manager->AddHPBarBlinkTarget((int)CharacterType::Sorcerer);
	Manager->AddHPBarBlinkTarget((int)CharacterType::Bard);
	_pBasicAnimationComponent->SetPlayClip("Charging");
	_ldChagingTime = 0;
}


void Client::Wolf::Damage()
{
}

void Client::Wolf::Dead()
{
	Boss::Dead();
}

void Client::Wolf::AttackFade()
{
	_pAttackEffectComponent->SetOpacity(0);
}

void Client::Wolf::UseSkill1()
{
	_pBasicAnimationComponent->SetPlayClip("Skill1");
}

void Client::Wolf::UseSkill2()
{
	_pBasicAnimationComponent->SetPlayClip("Skill2");
	Sound->PlaySound("Wolves_Skill2", (int)SoundChannel::CHANNEL_SFX_5, SoundLoop::NOT_LOOP);

	_pSkillEffectComponent->SetPlayClip("Effect/skill2_vfx");
}

void Client::Wolf::DamegeSkill1()
{
	DebugPrintf("Wolf 포효!\n");
	Manager->DealSkillDamageToCharacter(CharacterType::Warrior, _iSkill);
	Manager->DealSkillDamageToCharacter(CharacterType::Thief, _iSkill);
	Manager->ClearHPBarBlinkTargets();
	StartPattern(0);
}

void Client::Wolf::DamegeSkill2()
{
	DebugPrintf("Wolf 몸통박치기!\n");
	Manager->DealSkillDamageToCharacter(CharacterType::All, _iBossSkill);
	Manager->ClearHPBarBlinkTargets();
}
