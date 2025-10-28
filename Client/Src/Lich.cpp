#include <Actor.h>
#include <AnimationComponent.h>
#include <SimpleIniLoader.h>
#include <StateComponent.h>
#include <SoundManager.h>
#include <BasicAnimationComponent.h>
#include <TextureManager.h>
#include <world.h>
#include <TimerComponent.h>
//#include <InputComponent.h>
#include <Random.h>
#include "GameManager.h"
#include "Lich.h"
#include "NoteManager.h"

constexpr float phase1OneNoteSec = 1.2f;
constexpr float phase2OneNoteSec = 0.6f;

void Client::Lich::BeginPlay()
{
	Boss::BeginPlay();

	std::string absolutePath = "Assets/Lich/LichData.ini";
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

	_iMaxHp = std::stoi(iniFile.GetValue("LichData", "_iMaxHp", "0"));			 //보스 총 체력
	_iAttack = std::stoi(iniFile.GetValue("LichData", "_iAttack", "0"));		 //보스 일반 공격	력
	_iSkill = std::stoi(iniFile.GetValue("LichData", "_iSkill", "0"));			 //보스 1스킬 공격력
	_iBossSkill = std::stoi(iniFile.GetValue("LichData", "_iBossSkill", "0"));	 //보스 2스킬 공격력

	//_ldAttackOffset = std::stof(iniFile.GetValue("LichData", "_ldAttackOffset", "0")) * (long double)1e9;		//일반공격 모션 오프셋
	_ldSkillOffset = std::stof(iniFile.GetValue("LichData", "_ldSkillOffset", "0")) * (long double)1e9;			//스킬 모션 오프셋
	//_ldBossSkillOffset = std::stof(iniFile.GetValue("LichData", "_ldBossSkillOffset", "0")) * (long double)1e9;	//보스스킬 모션 오프셋

	_ldSkillCharging = std::stof(iniFile.GetValue("LichData", "_ldSkillCharging", "0")) * (long double)1e9;		//1스킬 차징시간
	_ldBossSkillCharging = std::stof(iniFile.GetValue("LichData", "_ldBossSkillCharging", "0")) * (long double)1e9;	//2스킬 차징시간

	//0: 전사 1: 도적 2: 소서러 3: 바드 4:전체 5: 캔슬 불가
	_iSkillCancelType = std::stoi(iniFile.GetValue("LichData", "_iSkillCancelType", "5"));
	_iBossSkillCancelType = std::stoi(iniFile.GetValue("LichData", "_iBossSkillCancelType", "2"));

	_iHp = _iMaxHp;

	_pBasicAnimationComponent->AllAddClipThisActor();
	_pBasicAnimationComponent->SetPlayClip("Idle");
	_pBasicAnimationComponent->SetRelativeScale(Mathf::Vector2{ 0.5f, 0.5f });

	_ldAttackOffset = _pBasicAnimationComponent->GetClipTime("Attack") * Mathf::SecToNano;
	_ldBossSkillOffset = _pBasicAnimationComponent->GetClipTime("Skill2") * Mathf::SecToNano;

	_SkillIndicateCount = 12;
	_BossSkillIndicateCount = 4;

	_pStateComponent = AddComponent<Engine::StateComponent>("StateComponent");

	_pStateComponent->AddState(0, "Idle");
	_pStateComponent->AddState(1, "Attack");
	_pStateComponent->AddState(2, "Skill");
	_pStateComponent->AddState(3, "Skill2");
	_pStateComponent->AddState(4, "Damage");
	_pStateComponent->AddState(5, "Dead");

	_pStateComponent->GetState(0)->OnEnter.AddDynamic(this, &Lich::Idle);

	_pStateComponent->GetState(1)->OnEnter.AddDynamic(this, &Lich::AttackEnter);
	_pStateComponent->GetState(1)->OnUpdate.AddDynamic(this, &Lich::AttackUpdate);

	_pStateComponent->GetState(2)->OnEnter.AddDynamic(this, &Lich::SkillEnter);
	_pStateComponent->GetState(2)->OnUpdate.AddDynamic(this, &Lich::SkillUpdate);

	_pStateComponent->GetState(3)->OnEnter.AddDynamic(this, &Lich::Skill2Enter);
	_pStateComponent->GetState(3)->OnUpdate.AddDynamic(this, &Lich::Charging);

	_pStateComponent->GetState(4)->OnEnter.AddDynamic(this, &Lich::Damage);
	_pStateComponent->GetState(5)->OnEnter.AddDynamic(this, &Lich::Dead);

	//AttackEffect
	_pAttackEffectComponent->AllAddClipThisActor();
	_pAttackEffectComponent->AddFrameEvent("Effect/Attack_vfx", 11, this, &Lich::SoundAttack);
	_pAttackEffectComponent_2 = AddComponent<Engine::BasicAnimationComponent>("AttackEffect_2");
	_pAttackEffectComponent_2->AllAddClipThisActor();
	_pAttackEffectComponent_2->AddFrameEvent("Effect/Attack_vfx", 11, this, &Lich::SoundAttack);
	_pAttackEffectComponent_3 = AddComponent<Engine::BasicAnimationComponent>("AttackEffect_3");
	_pAttackEffectComponent_3->AllAddClipThisActor();
	_pAttackEffectComponent_3->AddFrameEvent("Effect/Attack_vfx", 11, this, &Lich::SoundAttack);
	_pAttackEffectComponent_4 = AddComponent<Engine::BasicAnimationComponent>("AttackEffect_4");
	_pAttackEffectComponent_4->AllAddClipThisActor();
	_pAttackEffectComponent_4->AddFrameEvent("Effect/Attack_vfx", 11, this, &Lich::SoundAttack);

	_pAttackEffectComponent->AddFrameLambda("Effect/Attack_vfx", 26, [&]() {
		GetWorld()->GetCameraActor()->CameraShake(0.3f, 2.5f);
		});

	_pAttackEffectComponent_2->AddFrameLambda("Effect/Attack_vfx", 26, [&]() {
		GetWorld()->GetCameraActor()->CameraShake(0.3f, 2.5f);
		});

	_pAttackEffectComponent_3->AddFrameLambda("Effect/Attack_vfx", 26, [&]() {
		GetWorld()->GetCameraActor()->CameraShake(0.3f, 2.5f);
		});

	_pAttackEffectComponent_4->AddFrameLambda("Effect/Attack_vfx", 26, [&]() {
		GetWorld()->GetCameraActor()->CameraShake(0.3f, 2.5f);
		});

	Mathf::Vector2 warriorPosition = GetWorld()->FindActor("Warrior")->GetRootComponent()->GetRelativeLocation();
	_pAttackEffectComponent->SetRelativeLocation(warriorPosition - GetRootComponent()->GetRelativeLocation());
	Mathf::Vector2 thiefPosition = GetWorld()->FindActor("Thief")->GetRootComponent()->GetRelativeLocation();
	_pAttackEffectComponent_2->SetRelativeLocation(thiefPosition - GetRootComponent()->GetRelativeLocation());
	Mathf::Vector2 sorcererPosition = GetWorld()->FindActor("Sorcerer")->GetRootComponent()->GetRelativeLocation();
	_pAttackEffectComponent_3->SetRelativeLocation(sorcererPosition - GetRootComponent()->GetRelativeLocation());
	Mathf::Vector2 bardPosition = GetWorld()->FindActor("Bard")->GetRootComponent()->GetRelativeLocation();
	_pAttackEffectComponent_4->SetRelativeLocation(bardPosition - GetRootComponent()->GetRelativeLocation());

	_pSkillEffectComponent->AllAddClipThisActor();
	_pSkillEffectComponent->SetOrder(1);


	_pSkillEffectComponent_2 = AddComponent<Engine::BasicAnimationComponent>("SkillEffect_2");
	_pSkillEffectComponent_2->AllAddClipThisActor();
	_pSkillEffectComponent_2->SetOrder(1);

	_pSkillEffectComponent_3 = AddComponent<Engine::BasicAnimationComponent>("SkillEffect_3");
	_pSkillEffectComponent_3->AllAddClipThisActor();
	_pSkillEffectComponent_3->AddFrameEvent("Effect/Skill1_fire", 8, this, &Lich::DamegeSkill_Bard);
	_pSkillEffectComponent_3->AddFrameLambda("Effect/Skill1_fire", 7, [&]() {
		GetWorld()->GetCameraActor()->CameraShake(0.8f, 4.f);
		});

	_pSkillEffectComponent_4 = AddComponent<Engine::BasicAnimationComponent>("SkillEffect_4");
	_pSkillEffectComponent_4->AllAddClipThisActor();
	_pSkillEffectComponent_4->AddFrameEvent("Effect/Skill1_fire", 8, this, &Lich::DamegeSkill_Sorcerer);
	_pSkillEffectComponent_4->AddFrameLambda("Effect/Skill1_fire", 7, [&]() {
		GetWorld()->GetCameraActor()->CameraShake(0.8f, 4.f);
		});

	_pSkillEffectComponent_5 = AddComponent<Engine::BasicAnimationComponent>("Skill2Effect_5");
	_pSkillEffectComponent_5->AllAddClipThisActor("Assets/Wolf/Clips.csv", "Wolf");
	_pSkillEffectComponent_5->AddFrameEvent("Skill2", 15, this, &Lich::DamegeSkill1_wolf);
	_pSkillEffectComponent_5->AddFrameLambda("Skill2", 8, [&]() {
		GetWorld()->GetCameraActor()->CameraShake(0.8f, 4.f);
		});

	_pSkillEffectComponent_8 = AddComponent<Engine::BasicAnimationComponent>("Skill2Effect_8");
	_pSkillEffectComponent_8->AllAddClipThisActor("Assets/Wolf/Clips.csv", "Wolf");
	_pSkillEffectComponent_8->AddFrameEvent("Effect/skill2_vfx", _pSkillEffectComponent_8->GetClipMaxframe("Effect/skill2_vfx") - 1, this, &Lich::Skill1_wolfEnd);

	_pSkillEffectComponent_6 = AddComponent<Engine::BasicAnimationComponent>("Skill2Effect_6");
	_pSkillEffectComponent_6->AllAddClipThisActor("Assets/Dragon/Clips.csv", "Dragon");
	_pSkillEffectComponent_6->AddFrameEvent("Skill2", 10, this, &Lich::DamegeSkill2_fire);
	_pSkillEffectComponent_6->AddFrameLambda("Skill2", 12, [&]() {
		GetWorld()->GetCameraActor()->CameraShake(0.8f, 4.f);
		});

	_pSkillEffectComponent_7 = AddComponent<Engine::BasicAnimationComponent>("Skill2Effect_7");
	_pSkillEffectComponent_7->AllAddClipThisActor("Assets/Dragon/Clips.csv", "Dragon");
	_pSkillEffectComponent_7->AddFrameEvent("Effect/Skill2_fire", _pSkillEffectComponent_7->GetClipMaxframe("Effect/Skill2_fire") - 1, this, &Lich::Skill2_fireEnd);

	_pSkill2EffectComponent->AllAddClipThisActor();
	_pSkill2EffectComponent->AddFrameEvent("Effect/Skill2_light", 7, this, &Lich::DamegeSkill2_light);
	_pSkill2EffectComponent->AddFrameLambda("Effect/Skill2_light", 6, [&]() {
		GetWorld()->GetCameraActor()->CameraShake(0.8f, 4.f);
		});
	_pSkill2EffectComponent->AddFrameEvent("Effect/Skill2_light", 5, this, &Lich::SoundSkill2);

	_pSkill2EffectComponent_2 = AddComponent<Engine::BasicAnimationComponent>("Skill2Effect_2");
	_pSkill2EffectComponent_2->AllAddClipThisActor();
	_pSkill2EffectComponent_2->AddFrameEvent("Effect/Skill2_light", 7, this, &Lich::DamegeSkill2_light);
	_pSkill2EffectComponent->AddFrameLambda("Effect/Skill2_light", 6, [&]() {
			GetWorld()->GetCameraActor()->CameraShake(0.8f, 4.f);
		});
	_pSkill2EffectComponent_2->AddFrameEvent("Effect/Skill2_light", 5, this, &Lich::SoundSkill2);

	_pSkill2EffectComponent_3 = AddComponent<Engine::BasicAnimationComponent>("Skill2Effect_3");
	_pSkill2EffectComponent_3->AllAddClipThisActor();
	_pSkill2EffectComponent_3->AddFrameEvent("Effect/Skill2_light", 7, this, &Lich::DamegeSkill2_light);
	_pSkill2EffectComponent_3->AddFrameLambda("Effect/Skill2_light", 6, [&]() {
			GetWorld()->GetCameraActor()->CameraShake(0.8f, 4.f);
		});
	_pSkill2EffectComponent_3->AddFrameEvent("Effect/Skill2_light", 5, this, &Lich::SoundSkill2);

	_pSkill2EffectComponent_4 = AddComponent<Engine::BasicAnimationComponent>("Skill2Effect_4");
	_pSkill2EffectComponent_4->AllAddClipThisActor();

	_pSkill2EffectComponent_5 = AddComponent<Engine::BasicAnimationComponent>("Skill2Effect_5");
	_pSkill2EffectComponent_5->AllAddClipThisActor();
	_pSkill2EffectComponent_5->SetOrder(1);

	_pTimerManager = AddComponent<Engine::TimerComponent>("TimerComponent");
	_pTimerManager->AddTimer(10);
	_pTimerManager->GetTimer(0).onTimeOver.AddDynamic(this, &Lich::SpawnSkillEffectTargetSorcerer_magic);
	_pTimerManager->GetTimer(1).onTimeOver.AddDynamic(this, &Lich::SpawnSkillEffectTargetBard_magic);

	_pTimerManager->GetTimer(2).onTimeOver.AddDynamic(this, &Lich::SpawnSkillEffectTargetSorcerer_fire);
	_pTimerManager->GetTimer(3).onTimeOver.AddDynamic(this, &Lich::SpawnSkillEffectTargetBard_fire);

	_pTimerManager->GetTimer(4).onTimeOver.AddDynamic(this, &Lich::SpawnSkill2Effect_light1);
	_pTimerManager->GetTimer(5).onTimeOver.AddDynamic(this, &Lich::SpawnSkill2Effect_light2);
	_pTimerManager->GetTimer(6).onTimeOver.AddDynamic(this, &Lich::SpawnSkill2Effect_light3);

	MixRandomTargetArray();
}

void Client::Lich::Tick(_duration deltaSeconds)
{
	long double BGMPosition = Sound->GetPosition(CHANNEL_BGM);
	if (_isUseSkill2)
	{
		if(_pBasicAnimationComponent->IsClipEnd("Skill2"))
		{
			_pStateComponent->SetCurrentState(0);
			_isUseSkill2 = false;
		}
	}	   
	if (0 < lightningCount)
	{
		if(lightningStartPosition <= BGMPosition)
		{
			auto& noteSpawnQueue = Manager->GetNoteManager()->GetNoteSpawnQueue();
			if (!noteSpawnQueue.empty())
			{
				for (auto& data : noteSpawnQueue)
				{			
					BGMPosition = Sound->GetPosition(CHANNEL_BGM);
					if (data._noteTime > BGMPosition)
					{
						lightningStartPosition = data._noteTime;
						if (lightningCount == 3)
						{
							_pTimerManager->SetTimeSyncMusic(5, CHANNEL_BGM, (lightningStartPosition - BGMPosition - (long double)_pSkill2EffectComponent->GetClipTime("Effect/Skill2_light") * Mathf::SecToNano) * Mathf::NanoToSec);
						}													 
						else if(lightningCount == 2)
						{													 
							_pTimerManager->SetTimeSyncMusic(6, CHANNEL_BGM, (lightningStartPosition - BGMPosition - (long double)_pSkill2EffectComponent->GetClipTime("Effect/Skill2_light") * Mathf::SecToNano) * Mathf::NanoToSec);
						}
						break;
					}	
					lightningStartPosition = Manager->GetNoteManager()->GetCurrNotePosition();
					if (lightningCount == 3)
					{
						_pTimerManager->SetTimeSyncMusic(5, CHANNEL_BGM, (lightningStartPosition - BGMPosition - (long double)_pSkill2EffectComponent->GetClipTime("Effect/Skill2_light") * Mathf::SecToNano) * Mathf::NanoToSec);
					}
					else if (lightningCount == 2)
					{
						_pTimerManager->SetTimeSyncMusic(6, CHANNEL_BGM, (lightningStartPosition - BGMPosition - (long double)_pSkill2EffectComponent->GetClipTime("Effect/Skill2_light") * Mathf::SecToNano) * Mathf::NanoToSec);
					}
				}
			}
			else
			{
				lightningStartPosition = Manager->GetNoteManager()->GetCurrNotePosition();
				if (lightningCount == 3)
				{
					_pTimerManager->SetTimeSyncMusic(5, CHANNEL_BGM, (lightningStartPosition - BGMPosition - (long double)_pSkill2EffectComponent->GetClipTime("Effect/Skill2_light") * Mathf::SecToNano) * Mathf::NanoToSec);
				}
				else if (lightningCount == 2)
				{
					_pTimerManager->SetTimeSyncMusic(6, CHANNEL_BGM, (lightningStartPosition - BGMPosition - (long double)_pSkill2EffectComponent->GetClipTime("Effect/Skill2_light") * Mathf::SecToNano) * Mathf::NanoToSec);
				}
			}		
			--lightningCount;
		}	
	}

	if (_currPhase != 2 && BGMPosition >= (long double)80.0 * Mathf::SecToNano)
	{
		SetPhase2();
	}

	Boss::Tick(deltaSeconds);
}

void Client::Lich::Fixed()
{
}

void Client::Lich::EndPlay()
{
}

void Client::Lich::ReviveInitialize()
{
}

void Client::Lich::SerializeIn(nlohmann::ordered_json& object)
{
}

void Client::Lich::SerializeOut(nlohmann::ordered_json& object)
{
}

Client::Lich* Client::Lich::Create()
{
	return new Lich;
}

void Client::Lich::Destroy()
{
	Super::Destroy();
}

void Client::Lich::StartPattern(int PatternNumber)
{
	_pStateComponent->SetCurrentState(PatternNumber);
}

void Client::Lich::Idle()
{
	if(2 == _currPhase)
	{
		_pBasicAnimationComponent->SetPlayClip("Phase2/Idle");
	}
	else
	{
		_pBasicAnimationComponent->SetPlayClip("Idle");
	}
}

void Client::Lich::AttackEnter()
{
	if (2 == _currPhase)
	{
		_pBasicAnimationComponent->SetPlayClip("Phase2/Attack");
	}
	else
	{
		_pBasicAnimationComponent->SetPlayClip("Attack");
	}

	switch (_randomTargetArray[_currTargetIndex]) {
		case 0:
			_pAttackEffectComponent->SetPlayClip("Effect/Attack_vfx");
			break;
		case 1:
			_pAttackEffectComponent_2->SetPlayClip("Effect/Attack_vfx");
			break;
		case 2:
			_pAttackEffectComponent_3->SetPlayClip("Effect/Attack_vfx");
			break;
		case 3:
			_pAttackEffectComponent_4->SetPlayClip("Effect/Attack_vfx");
			break;

		default:
			break;
	}
	
}

void Client::Lich::AttackUpdate()
{
	if (2 == _currPhase)
	{
		if (_pBasicAnimationComponent->IsClipEnd("Phase2/Attack"))
		{
			if (_currTargetIndex > 3)
			{
				MixRandomTargetArray();
				_currTargetIndex = 0;
			}
			else
			{
				Manager->DealDamageToCharacter(static_cast<CharacterType>(_randomTargetArray[_currTargetIndex]), _iAttack);
				DebugPrintf("Lich 공격!!!\n");
				_currTargetIndex++;
			}
			_pStateComponent->SetCurrentState(0);
		}
	}
	else
	{
		if (_pBasicAnimationComponent->IsClipEnd("Attack"))
		{
			if (_currTargetIndex > 3)
			{
				MixRandomTargetArray();
				_currTargetIndex = 0;
			}
			else
			{
				Manager->DealDamageToCharacter(static_cast<CharacterType>(_randomTargetArray[_currTargetIndex]), _iAttack);
				DebugPrintf("Lich 공격!!!\n");
				_currTargetIndex++;
			}
			_pStateComponent->SetCurrentState(0);
		}
	}

	
}

void Client::Lich::SoundAttack()
{
	Sound->PlaySound("FInal_attack", SoundChannel::CHANNEL_SFX_5, SoundLoop::NOT_LOOP);
}

void Client::Lich::SkillEnter()
{
	if(_currPhase == 1)
	{
		_skillPatten = 1;
		_bIsSkillFail = false;
		_pBasicAnimationComponent->SetPlayClip("Skill1");

		_pTimerManager->SetTimeSyncMusic(0, CHANNEL_BGM, phase1OneNoteSec * 4.f, false);
		_pTimerManager->SetTimeSyncMusic(1, CHANNEL_BGM, phase1OneNoteSec * 6.f, false);

		_pTimerManager->SetTimeSyncMusic(2, CHANNEL_BGM, phase1OneNoteSec * 8.f, false);
		_pTimerManager->SetTimeSyncMusic(3, CHANNEL_BGM, phase1OneNoteSec * 10.f, false);
	}
	else if (_currPhase == 2)
	{
		_skillPatten = 1;
		_bIsSkillFail = false;
		_pBasicAnimationComponent->SetPlayClip("Phase2/Charging");
		_ldChagingTime = 0;
	}
}

void Client::Lich::SkillUpdate()
{
	if (_currPhase == 1)
	{
		if (_pBasicAnimationComponent->IsClipEnd("Skill1"))
		{
			StartPattern(0);
		}
	}
	else if (_currPhase == 2)
	{
		if (_pBasicAnimationComponent->IsCurrClip("Charging") || _pBasicAnimationComponent->IsCurrClip("Phase2/Charging"))
		{
			long double position = Sound->GetPosition(CHANNEL_BGM);
			long double _ldChagingDuration = 0;
			if (_ldChagingTime == 0) {
				_ldChagingTime = Sound->GetPosition(CHANNEL_BGM);
			}
			else
			{
				_ldChagingDuration = position - _ldChagingTime;
			}

			if (_skillPatten == 1)
			{
				DebugPrintf("Charging : %Lf\n", _ldChagingDuration);
				Manager->UpdateCharacterHPBarBlink(_iSkill);
				_iCharginCancel = _iSkillCancelType;
				_iIndicateCount = _SkillIndicateCount;
				long double countDuration = _ldSkillCharging / (long double)_SkillIndicateCount;
				int degredIndicate = _ldChagingDuration / countDuration;
				_iIndicateCount -= degredIndicate;

				if (_bIsSkillFail)
				{
					Manager->CancelCharacterHPBarBlink();
					_pStateComponent->SetCurrentState(0);
					_bIsSkillCharging = false;
					_bIsSkillFail = false;
					_iCharginCancel = -1;
				}

				if (_ldChagingDuration > _ldSkillCharging)
				{
					Manager->CancelCharacterHPBarBlink();
					_bIsSkillCharging = false;
					_ldChagingTime = 0;
					_iCharginCancel = -1;
					UseSkill1();
				}
			}
		}
	}
}

void Client::Lich::Skill2Enter()
{
	if (_currPhase == 1)
	{
		_skillPatten = 2;
		_pBasicAnimationComponent->SetPlayClip("Charging");
		_ldChagingTime = 0;
	}
	else if (_currPhase == 2)
	{
		_skillPatten = 2;
		_pBasicAnimationComponent->SetPlayClip("Phase2/Charging");
		_ldChagingTime = 0;
	}
}

void Client::Lich::SoundSkill2()
{
	Sound->PlaySound("Sorcerer_skill", (int)SoundChannel::CHANNEL_SFX_5, SoundLoop::NOT_LOOP);
}

void Client::Lich::Damage()
{

}

void Client::Lich::Dead()
{
	Boss::Dead();
}

void Client::Lich::MixRandomTargetArray()
{
	for (size_t i = 0; i < 4; ++i)
	{
		int randIndex = _random_03.Generate();
		DebugPrintf("random0~3 : %d\n", randIndex);

		std::swap(_randomTargetArray[i], _randomTargetArray[randIndex]);
	}
}

void Client::Lich::SetPhase2()
{
	_currPhase = 2;
	_ldSkillOffset = _pSkillEffectComponent_5->GetClipTime("Skill1") * Mathf::SecToNano;
	_ldSkillCharging = phase2OneNoteSec * 2.f * Mathf::SecToNano;

	_ldBossSkillOffset = _pSkillEffectComponent_6->GetClipTime("Skill2") * Mathf::SecToNano;
	_ldBossSkillCharging = phase2OneNoteSec * 4.f * Mathf::SecToNano;

	_iSkillCancelType = 1;
	_iBossSkillCancelType = 2;

	StartPattern(0);
}

void Client::Lich::SetPhase1()
{					   
	std::string absolutePath = "Assets/Lich/LichData.ini";
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

	_currPhase = 1;

	_ldSkillOffset = 0;
	_ldSkillCharging = 0;

	_ldBossSkillOffset = _pBasicAnimationComponent->GetClipTime("Skill2") * Mathf::SecToNano;
	_ldBossSkillCharging = std::stof(iniFile.GetValue("LichData", "_ldBossSkillCharging", "0")) * (long double)1e9;

	_iSkillCancelType = std::stoi(iniFile.GetValue("LichData", "_iSkillCancelType", "5"));
	_iBossSkillCancelType = std::stoi(iniFile.GetValue("LichData", "_iBossSkillCancelType", "2"));

	StartPattern(0);
}

void Client::Lich::SpawnSkillEffectTargetSorcerer_magic()
{
	_pSkillEffectComponent_2->Visible = true;
	_pSkillEffectComponent_2->SetPlayClip("Effect/Skill1_magic");
	Mathf::Vector2 sorcererPosition = GetWorld()->FindActor("Sorcerer")->GetRootComponent()->GetRelativeLocation();
	_pSkillEffectComponent_2->SetRelativeLocation(sorcererPosition - GetRootComponent()->GetRelativeLocation());
	_pSkillEffectComponent_2->AddRelativeLocation({ -5.f, -120.f });
}

void Client::Lich::SpawnSkillEffectTargetBard_magic()
{
	_pSkillEffectComponent->Visible = true;
	_pSkillEffectComponent->SetPlayClip("Effect/Skill1_magic");
	Mathf::Vector2 bardPosition = GetWorld()->FindActor("Bard")->GetRootComponent()->GetRelativeLocation();
	_pSkillEffectComponent->SetRelativeLocation(bardPosition - GetRootComponent()->GetRelativeLocation());
	_pSkillEffectComponent->AddRelativeLocation({ -5.f, -120.f});
}

void Client::Lich::SpawnSkillEffectTargetSorcerer_fire()
{
	_pSkillEffectComponent_4->Visible = true;
	_pSkillEffectComponent_4->SetPlayClip("Effect/Skill1_fire");
	Sound->PlaySound("Dragon_skill1", (int)SoundChannel::CHANNEL_SFX_5, SoundLoop::NOT_LOOP);
	Mathf::Vector2 sorcererPosition = GetWorld()->FindActor("Sorcerer")->GetRootComponent()->GetRelativeLocation();
	_pSkillEffectComponent_4->SetRelativeLocation(sorcererPosition - GetRootComponent()->GetRelativeLocation());
	_pSkillEffectComponent_4->AddRelativeLocation({ -5.f, -100.f });
}

void Client::Lich::SpawnSkillEffectTargetBard_fire()
{
	_pSkillEffectComponent_3->Visible = true;
	_pSkillEffectComponent_3->SetPlayClip("Effect/Skill1_fire");
	Sound->PlaySound("Dragon_skill1", (int)SoundChannel::CHANNEL_SFX_5, SoundLoop::NOT_LOOP);
	Mathf::Vector2 bardPosition = GetWorld()->FindActor("Bard")->GetRootComponent()->GetRelativeLocation();
	_pSkillEffectComponent_3->SetRelativeLocation(bardPosition - GetRootComponent()->GetRelativeLocation());
	_pSkillEffectComponent_3->AddRelativeLocation({ -5.f, -100.f });
}

void Client::Lich::DamegeSkill_Bard()
{
	Manager->DealSkillDamageToCharacter(CharacterType::Bard, 4);
	DebugPrintf("Lich 불기둥!\n");
}

void Client::Lich::DamegeSkill_Sorcerer()
{
	Manager->DealSkillDamageToCharacter(CharacterType::Sorcerer, 4);
	DebugPrintf("Lich 불기둥!\n");
}

void Client::Lich::SpawnSkill2Effect_light1()
{
	_pSkill2EffectComponent->Visible = true;
	_pSkill2EffectComponent->SetPlayClip("Effect/Skill2_light");
	Mathf::Vector2 thiefPosition = GetWorld()->FindActor("Thief")->GetRootComponent()->GetRelativeLocation();
	Mathf::Vector2 sorcererPosition = GetWorld()->FindActor("Sorcerer")->GetRootComponent()->GetRelativeLocation();
	Mathf::Vector2 halfPosition = sorcererPosition - thiefPosition;
	halfPosition.x = halfPosition.x * 0.5f;
	halfPosition.y = 0;
	_pSkill2EffectComponent->SetRelativeLocation((thiefPosition + halfPosition) - GetRootComponent()->GetRelativeLocation());
	_pSkill2EffectComponent->AddRelativeLocation({ 0.f, -100.f });

	_pSkill2EffectComponent_5->Visible = true;
	_pSkill2EffectComponent_5->SetPlayClip("Effect/Skill2_magic");
	_pSkill2EffectComponent_5->SetRelativeLocation((thiefPosition + halfPosition) - GetRootComponent()->GetRelativeLocation());
	_pSkill2EffectComponent_5->AddRelativeLocation({ 0.f, -25 });
}

void Client::Lich::SpawnSkill2Effect_light2()
{
	_pSkill2EffectComponent_2->Visible = true;
	_pSkill2EffectComponent_2->SetPlayClip("Effect/Skill2_light");
	Mathf::Vector2 thiefPosition = GetWorld()->FindActor("Thief")->GetRootComponent()->GetRelativeLocation();
	Mathf::Vector2 sorcererPosition = GetWorld()->FindActor("Sorcerer")->GetRootComponent()->GetRelativeLocation();
	Mathf::Vector2 halfPosition = sorcererPosition - thiefPosition;
	halfPosition.x = halfPosition.x * 0.5f;
	halfPosition.y = 0;
	_pSkill2EffectComponent_2->SetRelativeLocation((thiefPosition + halfPosition) - GetRootComponent()->GetRelativeLocation());
	_pSkill2EffectComponent_2->AddRelativeLocation({ 0.f, -100.f });

	_pSkill2EffectComponent_5->Visible = true;
	_pSkill2EffectComponent_5->SetPlayClip("Effect/Skill2_magic");
	_pSkill2EffectComponent_5->SetRelativeLocation((thiefPosition + halfPosition) - GetRootComponent()->GetRelativeLocation());
	_pSkill2EffectComponent_5->AddRelativeLocation({ 0.f, -25 });
}

void Client::Lich::SpawnSkill2Effect_light3()
{
	_pSkill2EffectComponent_3->Visible = true;
	_pSkill2EffectComponent_3->SetPlayClip("Effect/Skill2_light");
	Mathf::Vector2 thiefPosition = GetWorld()->FindActor("Thief")->GetRootComponent()->GetRelativeLocation();
	Mathf::Vector2 sorcererPosition = GetWorld()->FindActor("Sorcerer")->GetRootComponent()->GetRelativeLocation();
	Mathf::Vector2 halfPosition = sorcererPosition - thiefPosition;
	halfPosition.x = halfPosition.x * 0.5f;
	halfPosition.y = 0;
	_pSkill2EffectComponent_3->SetRelativeLocation((thiefPosition + halfPosition) - GetRootComponent()->GetRelativeLocation());
	_pSkill2EffectComponent_3->AddRelativeLocation({ 0.f, -100.f });

	_pSkill2EffectComponent_5->Visible = true;
	_pSkill2EffectComponent_5->SetPlayClip("Effect/Skill2_magic");
	_pSkill2EffectComponent_5->SetRelativeLocation((thiefPosition + halfPosition) - GetRootComponent()->GetRelativeLocation());
	_pSkill2EffectComponent_5->AddRelativeLocation({ 0.f, -25 });
}

void Client::Lich::DamegeSkill2_light()
{
	Manager->DealSkillDamageToCharacter(CharacterType::All, _iBossSkill);
	DebugPrintf("Lich 번개!!!\n");
}

void Client::Lich::DamegeSkill1_wolf()
{
	Sound->PlaySound("Wolves_Skill2", (int)SoundChannel::CHANNEL_SFX_5, SoundLoop::NOT_LOOP);
	Manager->DealSkillDamageToCharacter(CharacterType::All, 4);
	DebugPrintf("Lich 울프 몸통박치기!!!\n");
}

void Client::Lich::Skill1_wolfEnd()
{
	_pSkillEffectComponent_5->Visible = false;
	_pSkillEffectComponent_5->SetPlayClip("Idle");

	_pSkillEffectComponent_8->Visible = false;
	_pSkillEffectComponent_8->SetPlayClip("Idle");
}

void Client::Lich::DamegeSkill2_fire()
{
	Sound->PlaySound("Dragon_skill2", (int)SoundChannel::CHANNEL_SFX_5, SoundLoop::NOT_LOOP);
	DebugPrintf("Lich 드래곤 브래스!!!\n");

	Manager->DealSkillDamageToCharacter(CharacterType::Thief, 4);
	Manager->DealSkillDamageToCharacter(CharacterType::Warrior, 4);
}

void Client::Lich::Skill2_fireEnd()
{
	_pSkillEffectComponent_6->Visible = false;
	_pSkillEffectComponent_6->SetPlayClip("Idle");

	_pSkillEffectComponent_7->Visible = false;
	_pSkillEffectComponent_7->SetPlayClip("Idle");
}
		   
void Client::Lich::UseSkill1()
{
	_pStateComponent->SetCurrentState(0);
	_pSkillEffectComponent_5->Visible = true;
	_pSkillEffectComponent_5->SetPlayClip("Skill2");
	_pSkillEffectComponent_5->SetRelativeLocation({ -150.f, 0.f });

	_pSkillEffectComponent_8->Visible = true;
	_pSkillEffectComponent_8->SetPlayClip("Effect/skill2_vfx");
	_pSkillEffectComponent_8->SetRelativeLocation({ -150.f, 0.f });
}

void Client::Lich::UseSkill2()
{
	if(_currPhase == 1)
	{
		_isUseSkill2 = true;
		_pBasicAnimationComponent->SetPlayClip("Skill2");
		lightningCount = 3;

		auto& noteSpawnQueue = Manager->GetNoteManager()->GetNoteSpawnQueue();
		long double BGMPosition = Sound->GetPosition(CHANNEL_BGM);
		if (!noteSpawnQueue.empty())
		{
			for (auto& data : noteSpawnQueue)
			{
				if (data._noteTime > BGMPosition)
				{
					lightningStartPosition = data._noteTime;
					_pTimerManager->SetTimeSyncMusic(4, CHANNEL_BGM, (lightningStartPosition - BGMPosition - (long double)_pSkill2EffectComponent->GetClipTime("Effect/Skill2_light") * Mathf::SecToNano) * Mathf::NanoToSec);
					break;
				}
				lightningStartPosition = Manager->GetNoteManager()->GetCurrNotePosition();
				_pTimerManager->SetTimeSyncMusic(4, CHANNEL_BGM, (lightningStartPosition - BGMPosition - (long double)_pSkill2EffectComponent->GetClipTime("Effect/Skill2_light") * Mathf::SecToNano) * Mathf::NanoToSec);
			}
		}
		else
		{
			lightningStartPosition = Manager->GetNoteManager()->GetCurrNotePosition();
			_pTimerManager->SetTimeSyncMusic(4, CHANNEL_BGM, (lightningStartPosition - BGMPosition - (long double)_pSkill2EffectComponent->GetClipTime("Effect/Skill2_light") * Mathf::SecToNano) * Mathf::NanoToSec);
		}
	}
	else if (_currPhase == 2)
	{
		_pStateComponent->SetCurrentState(0);

		_pSkillEffectComponent_6->Visible = true;
		_pSkillEffectComponent_6->SetPlayClip("Skill2");
		_pSkillEffectComponent_6->SetRelativeLocation({ -150.f, 0 });

		_pSkillEffectComponent_7->Visible = true;
		_pSkillEffectComponent_7->SetPlayClip("Effect/Skill2_fire");
		_pSkillEffectComponent_7->SetRelativeLocation({ -150.f, 0 });
		_pSkillEffectComponent_7->AddRelativeLocation({ -560.f, 0.f });
	}
}

void Client::Lich::SpawnAttackEffect()
{
}
