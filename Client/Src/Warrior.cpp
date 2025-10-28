#include <Actor.h>
#include <BitmapComponent.h>
#include <AnimationComponent.h>
#include <StateComponent.h>
#include <SimpleIniLoader.h>
#include <TextureManager.h>
#include <InputComponent.h>
#include <BasicAnimationComponent.h>
#include <World.h>
#include <SoundManager.h>
#include <TimerComponent.h>
#include <TimeManager.h>

#include "Boss.h"
#include "NoteManager.h"
#include "Warrior.h"
#include "GameManager.h"
#include "HPComponent.h"
#include "ClientEnum.h"

void Client::Warrior::BeginPlay()
{
	Character::BeginPlay();

	std::string absolutePath = "Assets/Warrior/WarriorData.ini";
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

	_iMaxHp = std::stoi(iniFile.GetValue("WarriorData", "MaxHp", "0"));
	_iAttack = std::stoi(iniFile.GetValue("WarriorData", "AttackDamage", "0"));
	_iMaxMP = std::stoi(iniFile.GetValue("WarriorData", "MaxMp", "0"));
	_iSkillDemandNote = std::stoi(iniFile.GetValue("WarriorData", "SkillDemandNote", "0"));
	_iSkill = std::stoi(iniFile.GetValue("WarriorData", "SkillDamage", "0"));

	_iHp = _iMaxHp;
	_iMp = _iMaxMP;

	_pBasicAnimationComponent->SetRelativeScale({ 0.4f, 0.4f });
	_pBasicAnimationComponent->SetRelativeLocation({ 0.f, 10.f });
	_pBasicAnimationComponent->AllAddClipThisActor();
	_pBasicAnimationComponent->SetPlayClip("Idle");
	_pBasicAnimationComponent->AddFrameEvent("Attack", 9, this, &Warrior::VFXAttack);
	_pBasicAnimationComponent->AddFrameEvent("Attack", 10, this, &Warrior::SoundAttack); //공격 사운드 체크후 풀기

	_pAttackEffectComponent = AddComponent<Engine::BasicAnimationComponent>("AttackEffectComponent");
	_pAttackEffectComponent->AllAddClipThisActor();
	_pAttackEffectComponent->SetRelativeScale({ 0.8f, 0.8f });

	_pStateComponent = AddComponent<Engine::StateComponent>("StateComponent");
	_pInputComponent = AddComponent<Engine::InputComponent>("InputComponent");
	_pTimerComponent = AddComponent<Engine::TimerComponent>("TimerComponent");
	_pTimerComponent->GetTimer(0).onTimeOver.AddDynamic(this, &Warrior::DeleteSkill);

	_pSkillEffectComponent->AllAddClipThisActor();
	_pSkillEffectComponent->SetRelativeScale({ 0.7f, 0.7f });
	_pSkillEffectComponent->AddFrameEvent("Effect/SkillEnter", 12, this, &Warrior::SkillQuake);
	_pSkillEffectComponent->SetOrder(2);

	_pHPComponent = AddComponent<HPComponent>("HPComponent");
	_pHPComponent->SetRelativeLocation({ 0.f, 200.f });

	_pStateComponent->AddState(0, "Idle");
	_pStateComponent->AddState(1, "Attack");
	_pStateComponent->AddState(2, "Skill");
	

	_pStateComponent->GetState(0)->OnEnter.AddDynamic(this, &Warrior::Idle);
	_pStateComponent->GetState(1)->OnEnter.AddDynamic(this, &Warrior::Attack);
	_pStateComponent->GetState(2)->OnEnter.AddDynamic(this, &Warrior::Skill);
	

	_pNoteManager = dynamic_cast<NoteManager*>(GetWorld()->FindActor("NoteManager"));

	//케릭터 타입
	_characterType = 0;
}

void Client::Warrior::Tick(_duration deltaSeconds)
{
	Character::Tick(deltaSeconds);
	
	//DebugPrintf("Warrior SoundChannel::CHANNEL_SFX_1 Volume : %f\n", Sound->GetTargetVolume((int)SoundChannel::CHANNEL_SFX_1));

	if (Manager->GetCurrentStage() == Stage::Dialouge)
	{
		return;
	}

	if (!_bIsDead)
	{
		if (_iMp >= _iSkillDemandNote)	 //스킬사용가능 여부 업데이트
		{
			_bIsSkillOn = true;
		}
		else
		{
			_bIsSkillOn = false;
		}

		if (_pInputComponent->IsKeyPress(_skillKey) && !Sound->GetPause(CHANNEL_BGM))
		{
			if (0 < _iBlockCount)
			{
				return;
			}
			else if (_iMp >= _iSkillDemandNote)
			{
				_pStateComponent->SetCurrentState(2);
				_pTimerComponent->SetTime(0, 5.f);
			}
		}
		if (_pNoteManager->IsPerfect)
		{
			if (_pStateComponent->GetCurrentState() == _pStateComponent->GetState(2))
			{
				if (_iBlockCount == 0) {
					_pStateComponent->SetCurrentState(0);
				}
			}
			else {
				_pStateComponent->SetCurrentState(1);
			}
		}
		else if (_pNoteManager->IsGreat)
		{
			if (_pStateComponent->GetCurrentState() == _pStateComponent->GetState(2))
			{
				if (_iBlockCount == 0) {
					_pStateComponent->SetCurrentState(0);
				}
			}
			else {
				_pStateComponent->SetCurrentState(1);
			}

		}
		else if (_pNoteManager->IsGood)
		{
			if (_pStateComponent->GetCurrentState() == _pStateComponent->GetState(2))
			{
				if (_iBlockCount == 0) {
					_pStateComponent->SetCurrentState(0);
				}
			}
			else {
				_pStateComponent->SetCurrentState(1);
			}
		}
		else
		{
			if (_pStateComponent->GetCurrentState() == _pStateComponent->GetState(2))
			{
				if (_iBlockCount == 0) {
					_pStateComponent->SetCurrentState(0);
				}
			}
			else {
				_pStateComponent->SetCurrentState(0);
			}
		}
		if (_iNotePoint > _iSkillDemandNote)
		{
			_iNotePoint = _iSkillDemandNote;
		}
	}

	if (0 < _iBlockCount)
	{
		Manager->CancelBossAttack();
	}

	if (0 == _iBlockCount)
	{
		if (_pSkillEffectComponent->IsClipPlay("Effect/SkillExit"))
		{
			_iBlockCount = -1;
			DebugPrintf("iBlock_Disable 1\n");
		}
		else if (_pSkillEffectComponent->IsClipEnd("Effect/SkillExit"))
		{
			_pSkillEffectComponent->SetPlayClip("Effect/SkillExit");
		}
	}
}

void Client::Warrior::Fixed()
{
}

void Client::Warrior::EndPlay()
{
	_pStateComponent->SetCurrentState(0);
}

void Client::Warrior::ReviveInitialize()
{
	_pStateComponent->SetCurrentState(0);
	_iBlockCount = -1;
	_pNoteManager = dynamic_cast<NoteManager*>(GetWorld()->FindActor("NoteManager"));
	Super::ReviveInitialize();
}

void Client::Warrior::DeleteSkill()
{
	_iBlockCount = -1;
	if (_pSkillEffectComponent->IsClipEnd("Effect/SkillExit"))
	{
		_pSkillEffectComponent->SetPlayClip("Effect/SkillExit");
	}
	DebugPrintf("iBlock_Disable 2\n");
}



void Client::Warrior::SetupInputComponent(Engine::InputComponent* inputComponent)
{
	if (_skillKey == 0)
	{
		_skillKey = DIK_R;
	}

	inputComponent->BindKeys(DIK_R, DIK_RIGHT, DIP_DPAD_RIGHT);
	inputComponent->AttachInputManager();
}

Client::Warrior* Client::Warrior::Create()
{
	return new Warrior;
}

void Client::Warrior::Destroy()
{
}

void Client::Warrior::Idle()
{
	if (_pBasicAnimationComponent->IsFrameEnd())
	{
		_pBasicAnimationComponent->SetPlayClip("Idle");
	}
}

void Client::Warrior::Attack()
{
	if (_pBasicAnimationComponent->IsClipEnd("Attack"))
	{
		_pBasicAnimationComponent->SetPlayClip("Attack");
		Manager->DealDamageToBoss(_iAttack);
	}
}

void Client::Warrior::VFXAttack()
{
	_pAttackEffectComponent->SetPlayClip("Effect/Attack");
}

void Client::Warrior::SoundAttack()
{
	Sound->PlaySound("Warrior_attack", (int)SoundChannel::CHANNEL_SFX_1,SoundLoop::NOT_LOOP);
}

void Client::Warrior::Skill()
{
	if (-1 == _iBlockCount && _pSkillEffectComponent->IsClipEnd("Effect/SkillEnter"))
	{
		_iMp -= _iSkillDemandNote;
		_pBasicAnimationComponent->SetPlayClip("Skill");
		_pSkillEffectComponent->SetPlayClip("Effect/SkillEnter");
		Sound->PlaySound("Warrior_Skill", (int)SoundChannel::CHANNEL_SFX_1,SoundLoop::NOT_LOOP);

		Mathf::Vector2 bossPosition = Manager->GetCurrentBoss()->GetRootComponent()->GetRelativeLocation();
		Mathf::Vector2 warriorPosition = GetRootComponent()->GetRelativeLocation();
		Mathf::Vector2 halfPosition = (bossPosition + warriorPosition) * 0.5f;
		Mathf::Vector2 spawnPosition = Mathf::Abs(warriorPosition - halfPosition);

		_pSkillEffectComponent->SetRelativeLocation({ spawnPosition.x, 0.f });
		_iBlockCount = _iSkill;

		DebugPrintf("SKILL\n");
		DebugPrintf("BlockCount : %d\n", _iBlockCount);

		_pStateComponent->SetCurrentState(0);
	}
}



void Client::Warrior::SkillQuake()
{
	Engine::CameraActor* camera = GetWorld()->GetCameraActor();
	camera->CameraShake(0.5f, 3.5f);
}


