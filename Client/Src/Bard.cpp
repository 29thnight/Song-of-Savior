#include <Actor.h>
#include <BitmapComponent.h>
#include <AnimationComponent.h>
#include <StateComponent.h>
#include <SimpleIniLoader.h>
#include <World.h>
#include <TextureManager.h>
#include <InputComponent.h>
#include <BasicAnimationComponent.h>
#include <TimerComponent.h>
#include <SoundManager.h>
#include <TimeManager.h>

#include "Boss.h"
#include "Bard.h"
#include "NoteManager.h"
#include "GameManager.h"
#include "HPComponent.h"
#include "ClientEnum.h"

void Client::Bard::BeginPlay()
{
	Character::BeginPlay();
	_pStateComponent = AddComponent<Engine::StateComponent>("StateComponent");
	_pInputComponent = AddComponent<Engine::InputComponent>("InputComponent");

	std::string absolutePath = "Assets/Bard/BardData.ini";
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

	_iMaxHp = std::stoi(iniFile.GetValue("BardData", "MaxHp", "0"));
	_iAttack = std::stoi(iniFile.GetValue("BardData", "AttackDamage", "0"));
	_iMaxMP = std::stoi(iniFile.GetValue("BardData", "MaxMp", "0"));
	_iSkillDemandNote = std::stoi(iniFile.GetValue("BardData", "SkillDemandNote", "0"));
	
	_iHp = _iMaxHp;
	_iMp = _iMaxMP;

	_pBasicAnimationComponent->SetRelativeScale({ 0.4f, 0.4f });
	_pBasicAnimationComponent->SetRelativeLocation({ 15.f, 25.f });
	_pBasicAnimationComponent->AllAddClipThisActor();
	_pBasicAnimationComponent->SetPlayClip("Idle");
	_pBasicAnimationComponent->AddFrameEvent("Attack",13, this, &Bard::SoundAttack);

	_pAttackEffectComponent->AllAddClipThisActor();
	_pAttackEffectComponent->SetRelativeLocation({ -15.f, -230.f });
	_pAttackEffectComponent->SetOrder(1);
	_pSkillEffectComponent->AddFrameEvent("Effect/Heal_vfx", 0, this, &Bard::SoundSkill);

	_pSkillEffectComponent->AllAddClipThisActor();
	_pSkillEffectComponent->SetRelativeLocation({ -15.f, -230.f });
	_pSkillEffectComponent->SetOrder(1);

	_pMagicCircleEffectComponent = AddComponent<Engine::BasicAnimationComponent>("MagicCircleEffectComponent");
	_pMagicCircleEffectComponent->AllAddClipThisActor();
	_pMagicCircleEffectComponent->SetRelativeLocation({ -15.f, -230.f });
	_pMagicCircleEffectComponent->SetOrder(-1);

	_pStateComponent->AddState(0, "Idle");
	_pStateComponent->AddState(1, "Attack");
	_pStateComponent->AddState(2, "Skill");
	_pStateComponent->AddState(3, "Damage");
	_pStateComponent->AddState(4, "Dead");

	_pStateComponent->GetState(0)->OnEnter.AddDynamic(this, &Bard::Idle);
	_pStateComponent->GetState(1)->OnEnter.AddDynamic(this, &Bard::Attack);
	_pStateComponent->GetState(2)->OnEnter.AddDynamic(this, &Bard::Skill);
	_pStateComponent->GetState(2)->OnExit.AddDynamic(this, &Bard::SkillExit);
	/*_pStateComponent->GetState(3)->OnEnter.AddDynamic(this, &Bard::Damage);
	_pStateComponent->GetState(4)->OnEnter.AddDynamic(this, &Bard::Dead);*/

	_pNoteManager = dynamic_cast<NoteManager*>(GetWorld()->FindActor("NoteManager"));

	_pHPComponent = AddComponent<HPComponent>("HPComponent");
	_pHPComponent->SetRelativeLocation({ 0.f, 200.f });

	_pTimerComponent = AddComponent<Engine::TimerComponent>("TimerComponent");
	_pTimerComponent->GetTimer(0).onTimeOver.AddDynamic(this, &Bard::Surprised);
	//케릭터 타입
	_characterType = 3;
}

void Client::Bard::Tick(_duration deltaSeconds)
{
	Character::Tick(deltaSeconds);

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
			if (_iMp >= _iSkillDemandNote)
			{
				_pStateComponent->SetCurrentState(2);
				//_pInputComponent->InvokeVibration()
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
}

void Client::Bard::Fixed()
{
}

void Client::Bard::EndPlay()
{
}

void Client::Bard::ReviveInitialize()
{
	///\TODO: 여기에 다시 시작할 경우의 초기화 코드를 작성하면 됩니다.
	Super::ReviveInitialize();
	_pStateComponent->SetCurrentState(0);
	if (Manager->GetCurrentStage() == Stage::Dialouge)
	{
		_pTimerComponent->SetTime(0, 1.f);
	}
	_pNoteManager = dynamic_cast<NoteManager*>(GetWorld()->FindActor("NoteManager"));
	//TODO END: 여기에 다시 시작할 경우의 초기화 코드를 작성하면 됩니다.
	///\brief 단! 기획의도에 따라서, 스테이지를 처음부터 할건지 세이브 포인트가 있을 것인지에 따라서 유의점이 여러가지 있음.
}

void Client::Bard::SetupInputComponent(Engine::InputComponent* inputComponent)
{
	if (_skillKey == 0)
	{
		_skillKey = DIK_Q;
	}
	inputComponent->BindKeys(DIK_Q, DIK_DOWN, DIP_DPAD_DOWN);
	inputComponent->AttachInputManager();
}

Client::Bard* Client::Bard::Create()
{
	return new Bard;
}

void Client::Bard::Destroy()
{
}

void Client::Bard::Idle()
{
	if (_pBasicAnimationComponent->IsFrameEnd())
	{
		_pBasicAnimationComponent->SetPlayClip("Idle");
	}
}

void Client::Bard::Attack()
{
	if (_pBasicAnimationComponent->IsClipEnd("Attack"))
	{
		_pBasicAnimationComponent->SetPlayClip("Attack");
	}
}

void Client::Bard::SoundAttack()
{
	Sound->PlaySound("Bard_attack_02", SoundChannel::CHANNEL_SFX_4, SoundLoop::NOT_LOOP);
}

void Client::Bard::Skill()
{
	if (_pBasicAnimationComponent->IsClipEnd("Skill"))
	{
		_iMp -= _iSkillDemandNote;
		_pMagicCircleEffectComponent->StartReveal(0.1f);
		_pBasicAnimationComponent->SetPlayClip("Skill");
		Sound->PlaySound("SFX_bard_skill", SoundChannel::CHANNEL_SFX_4, SoundLoop::NOT_LOOP);
		_pSkillEffectComponent->SetPlayClip("Effect/Heal_vfx");
		_pAttackEffectComponent->SetPlayClip("Effect/Sparkle_vfx");
		_pMagicCircleEffectComponent->SetPlayClip("Effect/MagicCircle_vfx");
		Manager->BardHeal();
		DebugPrintf("Skill\n");
		_pStateComponent->SetCurrentState(0);
	}
}

void Client::Bard::SkillExit()
{
	_pSkillEffectComponent->StartConceal(1.1f);
	_pAttackEffectComponent->StartConceal(1.1f);
	_pMagicCircleEffectComponent->StartConceal(1.1f);
}

void Client::Bard::SoundSkill()
{
	Sound->PlaySound("Heal", SoundChannel::CHANNEL_SFX_4, SoundLoop::NOT_LOOP);
}

//void Client::Bard::Damage()
//{
//	_pBasicAnimationComponent->SetAlphaMaskEffect(true);
//	_pBasicAnimationComponent->SetAlphaMaskOpacity(0.5f);
//	_pBasicAnimationComponent->SetAlphaMaskDuration(1.5f);
//}
//
//void Client::Bard::Dead()
//{
//}

void Client::Bard::Surprised()
{
	_pBasicAnimationComponent->SetPlayClip("Surprised");
}
