#include <Actor.h>
#include <AnimationComponent.h>
#include <BasicAnimationComponent.h>
#include <SimpleIniLoader.h>
#include <StateComponent.h>
#include <SoundManager.h>
#include <TimerComponent.h>
#include <World.h>
//#include <InputComponent.h>
#include "GameManager.h"
#include "Dragon.h"

void Client::Dragon::BeginPlay()
{
	Boss::BeginPlay();

	std::string absolutePath = "Assets/Dragon/DragonData.ini";
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

	_iMaxHp = std::stoi(iniFile.GetValue("DragonData", "_iMaxHp", "0"));			 //보스 총 체력
	_iAttack = std::stoi(iniFile.GetValue("DragonData", "_iAttack", "0"));		 //보스 일반 공격	력
	_iSkill = std::stoi(iniFile.GetValue("DragonData", "_iSkill", "0"));			 //보스 1스킬 공격력
	_iBossSkill = std::stoi(iniFile.GetValue("DragonData", "_iBossSkill", "0"));	 //보스 2스킬 공격력

	_ldSkillCharging = std::stof(iniFile.GetValue("DragonData", "_ldSkillCharging", "0")) * (long double)1e9;		//1스킬 차징시간
	_ldBossSkillCharging = std::stof(iniFile.GetValue("DragonData", "_ldBossSkillCharging", "0")) * (long double)1e9;	//2스킬 차징시간

	//0: 전사 1: 도적 2: 소서러 3: 바드 4:전체 5: 캔슬 불가
	_iSkillCancelType = std::stoi(iniFile.GetValue("DragonData", "_iSkillCancelType", "0")); //일반스킬 로그 캔슬
	_iBossSkillCancelType = std::stoi(iniFile.GetValue("DragonData", "_iBossSkillCancelType", "0"));	 //보스스킬 소서러 캔슬

	_iHp = _iMaxHp;

	_iSkillCancelType = 5; //일반스킬 캔슬 안됨
	_iBossSkillCancelType = 2; //보스스킬 소서러 캔슬

	_SkillIndicateCount = 12;
	_BossSkillIndicateCount = 4;

	_pBasicAnimationComponent->AllAddClipThisActor();
	_pBasicAnimationComponent->SetPlayClip("Idle");
	_pBasicAnimationComponent->AddFrameLambda("Attack", 15, [&]() {
			GetWorld()->GetCameraActor()->CameraShake(0.5f, 2.5f);
		});

	_ldAttackOffset = _pBasicAnimationComponent->GetClipTime("Attack") * Mathf::SecToNano;
	_ldSkillOffset = 0;
	_ldBossSkillOffset = _pBasicAnimationComponent->GetClipTime("Skill2") * Mathf::SecToNano;

	//_pBasicAnimationComponent->AddFrameEvent("Skill2", 6, this, &Dragon::Skill2Play);
	_pBasicAnimationComponent->AddFrameEvent("Skill2", 10, this, &Dragon::DamegeSkill2);

	_pBasicAnimationComponent->AddFrameEvent("Attack", 16, this, &Dragon::SoundAttack);

	_pSkillEffectComponent->AllAddClipThisActor();
	_pSkillEffectComponent->SetOrder(-1);

	_pSkill2EffectComponent->AllAddClipThisActor();
	_pSkill2EffectComponent->SetOrder(1);
	_pSkill2EffectComponent->SetRelativeLocation({ -560.f, 0.f });
	_pSkill2EffectComponent->AddFrameLambda("Effect/Skill2_fire", 12, [&]() {
			GetWorld()->GetCameraActor()->CameraShake(0.8f, 2.5f);
		});

	//불기둥의 마법진
	_pSkillEffectComponent->AllAddClipThisActor();
	_pSkillEffectComponent->SetOrder(-1);

	_pSkillEffectComponent_2 = AddComponent<Engine::BasicAnimationComponent>("SkillEffect_2");
	_pSkillEffectComponent_2->AllAddClipThisActor();
	_pSkillEffectComponent_2->SetOrder(-1);

	//불기둥 본체
	_pSkillEffectComponent_3 = AddComponent<Engine::BasicAnimationComponent>("SkillEffect_3");
	_pSkillEffectComponent_3->AllAddClipThisActor();
	_pSkillEffectComponent_3->AddFrameEvent("Effect/Skill1_fire", 8, this, &Dragon::DamegeSkill_Bard);
	_pSkillEffectComponent_3->AddFrameLambda("Effect/Skill1_fire", 7, [&]() {
			GetWorld()->GetCameraActor()->CameraShake(0.8f, 4.f);
		});

	_pSkillEffectComponent_4 = AddComponent<Engine::BasicAnimationComponent>("SkillEffect_4");
	_pSkillEffectComponent_4->AllAddClipThisActor();
	_pSkillEffectComponent_4->AddFrameEvent("Effect/Skill1_fire", 8, this, &Dragon::DamegeSkill_Sorcerer);
	_pSkillEffectComponent_4->AddFrameLambda("Effect/Skill1_fire", 7, [&]() {
			GetWorld()->GetCameraActor()->CameraShake(0.8f, 4.f);
		});

	_pStateComponent = AddComponent<Engine::StateComponent>("StateComponent");
	_pStateComponent->AddState(0, "Idle");
	_pStateComponent->AddState(1, "Attack");
	_pStateComponent->AddState(2, "Skill");
	_pStateComponent->AddState(3, "Skill2");
	_pStateComponent->AddState(4, "Damage");
	_pStateComponent->AddState(5, "Dead");

	_pStateComponent->GetState(0)->OnEnter.AddDynamic(this, &Dragon::Idle);

	_pStateComponent->GetState(1)->OnEnter.AddDynamic(this, &Dragon::Attack);

	_pStateComponent->GetState(2)->OnEnter.AddDynamic(this, &Dragon::Skill);

	_pStateComponent->GetState(3)->OnEnter.AddDynamic(this, &Dragon::Skill2);
	_pStateComponent->GetState(3)->OnUpdate.AddDynamic(this, &Dragon::Charging);

	_pStateComponent->GetState(4)->OnEnter.AddDynamic(this, &Dragon::Damage);

	_pStateComponent->GetState(5)->OnEnter.AddDynamic(this, &Dragon::Dead);

	_pTimerManager = AddComponent<Engine::TimerComponent>("TimerComponent");

	_pTimerManager->AddTimer(6);
	_pTimerManager->GetTimer(0).onTimeOver.AddDynamic(this, &Dragon::SpawnSkillEffectTargetSorcerer_magic);
	_pTimerManager->GetTimer(1).onTimeOver.AddDynamic(this, &Dragon::SpawnSkillEffectTargetBard_magic);
	_pTimerManager->GetTimer(2).onTimeOver.AddDynamic(this, &Dragon::SpawnSkillEffectTargetSorcerer_fire);
	_pTimerManager->GetTimer(3).onTimeOver.AddDynamic(this, &Dragon::SpawnSkillEffectTargetBard_fire);
}

void Client::Dragon::Tick(_duration deltaSeconds)
{
	Boss::Tick(deltaSeconds);
	if (!_bIsDead)
	{
		if (_pStateComponent->GetCurrentState() == _pStateComponent->GetState(2))
		{
			if (_pBasicAnimationComponent->IsClipEnd("Skill1")) 
			{
				_pStateComponent->SetCurrentState(0);
			}
		}
		else if (_pStateComponent->GetCurrentState() == _pStateComponent->GetState(3))
		{
			if (!_pBasicAnimationComponent->IsCurrClip("Charging"))
			{
				if (_pBasicAnimationComponent->IsClipEnd("Skill2"))
				{
					_pStateComponent->SetCurrentState(0);
				}
			}
		}
		else if (_pStateComponent->GetCurrentState() == _pStateComponent->GetState(1))
		{
			if (_pBasicAnimationComponent->IsClipEnd("Attack")) {
				_pStateComponent->SetCurrentState(0);
				DebugPrintf("Dragon Attack");
				Manager->DealDamageToCharacter(CharacterType::Warrior, _iAttack);

			}
		}
	}
}

void Client::Dragon::Fixed()
{
}

void Client::Dragon::EndPlay()
{
}

void Client::Dragon::ReviveInitialize()
{
}

void Client::Dragon::SerializeIn(nlohmann::ordered_json& object)
{
}

void Client::Dragon::SerializeOut(nlohmann::ordered_json& object)
{
}

Client::Dragon* Client::Dragon::Create()
{
	return new Dragon;
}

void Client::Dragon::Destroy()
{
	Super::Destroy();
}

void Client::Dragon::Idle()
{
	_pBasicAnimationComponent->SetPlayClip("Idle");
}

void Client::Dragon::Attack()
{
	_pBasicAnimationComponent->SetPlayClip("Attack");
}

void Client::Dragon::SoundAttack()
{
	Sound->PlaySound("Dragon_attack", (int)SoundChannel::CHANNEL_SFX_5, SoundLoop::NOT_LOOP);
}

constexpr float oneNoteSec = 0.526315789;

void Client::Dragon::Skill()
{
	_skillPatten = 1;
	_bIsSkillFail = false;
	Manager->AddHPBarBlinkTarget((int)CharacterType::Bard);
	Manager->AddHPBarBlinkTarget((int)CharacterType::Sorcerer);
	_pBasicAnimationComponent->SetPlayClip("Charging");

	_pTimerManager->SetTimeSyncMusic(0, CHANNEL_BGM, oneNoteSec * 4.f, false);
	_pTimerManager->SetTimeSyncMusic(1, CHANNEL_BGM, oneNoteSec * 6.f, false);

	_pTimerManager->SetTimeSyncMusic(2, CHANNEL_BGM, oneNoteSec * 8.f, false);
	_pTimerManager->SetTimeSyncMusic(3, CHANNEL_BGM, oneNoteSec * 10.f, false);

	_ldChagingTime = 0;
}

void Client::Dragon::Skill2()
{
	_skillPatten = 2;
	_ldChagingTime = 0;
	Manager->AddHPBarBlinkTarget((int)CharacterType::Warrior);
	Manager->AddHPBarBlinkTarget((int)CharacterType::Thief);
	_pBasicAnimationComponent->SetPlayClip("Charging");
}

void Client::Dragon::Damage()
{
}

void Client::Dragon::Dead()
{
	Boss::Dead();
}


void Client::Dragon::UseSkill2()
{
	if (!_bIsSkillFail)
	{
		_pBasicAnimationComponent->SetPlayClip("Skill2");
	}
}

void Client::Dragon::Skill2Play()
{
	_pSkill2EffectComponent->SetPlayClip("Effect/Skill2_fire");
	Sound->PlaySound("Dragon_skill2", (int)SoundChannel::CHANNEL_SFX_5, SoundLoop::NOT_LOOP);
}

void Client::Dragon::SpawnSkillEffectTargetSorcerer_magic()
{
	_pSkillEffectComponent_2->Visible = true;
	_pSkillEffectComponent_2->SetPlayClip("Effect/Skill1_magic");
	Mathf::Vector2 sorcererPosition = GetWorld()->FindActor("Sorcerer")->GetRootComponent()->GetRelativeLocation();
	_pSkillEffectComponent_2->SetRelativeLocation(sorcererPosition - GetRootComponent()->GetRelativeLocation());
	_pSkillEffectComponent_2->AddRelativeLocation({ 0.f, -180.f });
}

void Client::Dragon::SpawnSkillEffectTargetSorcerer_fire()
{
	_pSkillEffectComponent_4->Visible = true;
	_pSkillEffectComponent_4->SetPlayClip("Effect/Skill1_fire");
	Sound->PlaySound("Dragon_skill1", (int)SoundChannel::CHANNEL_SFX_5, SoundLoop::NOT_LOOP);
	Mathf::Vector2 sorcererPosition = GetWorld()->FindActor("Sorcerer")->GetRootComponent()->GetRelativeLocation();
	_pSkillEffectComponent_4->SetRelativeLocation(sorcererPosition - GetRootComponent()->GetRelativeLocation());
	_pSkillEffectComponent_4->AddRelativeLocation({ 0.f, -210.f });
}

void Client::Dragon::SpawnSkillEffectTargetBard_magic()
{
	_pSkillEffectComponent->Visible = true;
	_pSkillEffectComponent->SetPlayClip("Effect/Skill1_magic");
	Mathf::Vector2 bardPosition = GetWorld()->FindActor("Bard")->GetRootComponent()->GetRelativeLocation();
	_pSkillEffectComponent->SetRelativeLocation(bardPosition - GetRootComponent()->GetRelativeLocation());
	_pSkillEffectComponent->AddRelativeLocation({ 0.f, -180.f });
}


void Client::Dragon::SpawnSkillEffectTargetBard_fire()
{
	_pSkillEffectComponent_3->Visible = true;
	_pSkillEffectComponent_3->SetPlayClip("Effect/Skill1_fire");
	Sound->PlaySound("Dragon_skill1", (int)SoundChannel::CHANNEL_SFX_5, SoundLoop::NOT_LOOP);
	Mathf::Vector2 bardPosition = GetWorld()->FindActor("Bard")->GetRootComponent()->GetRelativeLocation();
	_pSkillEffectComponent_3->SetRelativeLocation(bardPosition - GetRootComponent()->GetRelativeLocation());
	_pSkillEffectComponent_3->AddRelativeLocation({ 0.f, -210.f });
}


void Client::Dragon::DamegeSkill_Bard()
{
	Manager->DealSkillDamageToCharacter(CharacterType::Bard, 4);
	DebugPrintf("Dragon 불기둥!\n");
}

void Client::Dragon::DamegeSkill_Sorcerer()
{
	Manager->DealSkillDamageToCharacter(CharacterType::Sorcerer, 4);
	DebugPrintf("Dragon 불기둥!\n");
}

void Client::Dragon::DamegeSkill2()
{
	Skill2Play();
	Manager->DealSkillDamageToCharacter(CharacterType::Warrior, _iBossSkill);
	Manager->DealSkillDamageToCharacter(CharacterType::Thief, _iBossSkill);
	Manager->ClearHPBarBlinkTargets();
	DebugPrintf("Dragon 브레스\n");
}
