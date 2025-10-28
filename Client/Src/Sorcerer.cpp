#include <Actor.h>
#include <BitmapComponent.h>
#include <AnimationComponent.h>
#include <StateComponent.h>
#include <BasicAnimationComponent.h>
#include <SimpleIniLoader.h>
#include <TextureManager.h>
#include <InputComponent.h>
#include <SoundManager.h>
#include <World.h>
#include <TimeManager.h>

#include "Boss.h"
#include "NoteManager.h"
#include "Sorcerer.h"
#include "GameManager.h"
#include "HPComponent.h"
#include "ClientEnum.h"

void Client::Sorcerer::BeginPlay()
{
	Character::BeginPlay();
	//_pAnimationComponent->SetPlayClip("Idle");

	std::string absolutePath = "Assets/Sorcerer/SorcererData.ini";
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

	_iMaxHp = std::stoi(iniFile.GetValue("SorcererData", "MaxHp", "0"));
	_iAttack = std::stoi(iniFile.GetValue("SorcererData", "AttackDamage", "0"));
	_iMaxMP = std::stoi(iniFile.GetValue("SorcererData", "MaxMp", "0"));
	_iSkill = std::stoi(iniFile.GetValue("SorcererData", "SkillDamage", "0"));
	_iSkillDemandNote = std::stoi(iniFile.GetValue("SorcererData", "SkillDemandNote", "0"));
	
	_iHp = _iMaxHp;
	_iMp = _iMaxMP;

	_pSkillEffectMagicCircleComponent = AddComponent<Engine::BasicAnimationComponent>("SkillEffectMagicCircleComponent");
	_pStateComponent = AddComponent<Engine::StateComponent>("StateComponent");
	_pInputComponent = AddComponent<Engine::InputComponent>("InputComponent");

	_pBasicAnimationComponent->SetRelativeScale({ 0.4f, 0.4f });
	_pBasicAnimationComponent->SetRelativeLocation({ 0.f, 30.f });
	_pBasicAnimationComponent->AllAddClipThisActor();
	_pBasicAnimationComponent->SetPlayClip("Idle");
	
	_pStateComponent->AddState(0, "Idle");
	_pStateComponent->AddState(1, "Attack");
	_pStateComponent->AddState(2, "Skill");
	
	

	_pStateComponent->GetState(0)->OnEnter.AddDynamic(this, &Sorcerer::Idle);
	_pStateComponent->GetState(1)->OnEnter.AddDynamic(this, &Sorcerer::Attack);
	_pStateComponent->GetState(2)->OnEnter.AddDynamic(this, &Sorcerer::Skill);


	_pBasicAnimationComponent->AddFrameEvent("Attack", 7, this, &Sorcerer::SoundAttack); //공격 사운드 체크후 풀기

	_pSkillEffectComponent->AllAddClipThisActor();
	_pSkillEffectMagicCircleComponent->AllAddClipThisActor();
	_pSkillEffectComponent->SetOrder(2);
	_pSkillEffectComponent->AddFrameLambda("Effect/Thunder", 6, [&]() {
			GetWorld()->GetCameraActor()->CameraShake(0.3f, 4.f);
		});

	_pHPComponent = AddComponent<HPComponent>("HPComponent");
	_pHPComponent->SetRelativeLocation({ 0.f, 200.f });

	_pNoteManager = dynamic_cast<NoteManager*>(GetWorld()->FindActor("NoteManager"));

	_pSkillEffectComponent->AddFrameEvent("Effect/Thunder", 5, this, &Sorcerer::SoundSkill); //공격 사운드 체크후 풀기
	//케릭터 타입
	_characterType = 2;
}

void Client::Sorcerer::Tick(_duration deltaSeconds)
{
	Character::Tick(deltaSeconds);


	if (Manager->GetCurrentStage() == Stage::Dialouge)
	{
		return;
	}

	bossPosition = Manager->GetCurrentBoss()->GetRootComponent()->GetRelativeLocation();

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

		if (_pInputComponent->IsKeyPress(_skillKey) && 0 == lightningCount && !Sound->GetPause(CHANNEL_BGM))
		{
			if (_iMp >= _iSkillDemandNote)
			{
				_iMp -= _iSkillDemandNote;
				_pStateComponent->SetCurrentState(2);
				DebugPrintf("Sorcerer 번개!!! 진입\n");
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

				if (0 < lightningCount)
				{
					long double BGMPosition = Sound->GetPosition(CHANNEL_BGM);
					if (lightningStartPosition <= BGMPosition)
					{
						_pSkillEffectMagicCircleComponent->SetRelativeLocation(bossPosition - GetRootComponent()->GetRelativeLocation());
						_pSkillEffectComponent->AddRelativeLocation({ 0.f, -50.f });
						_pSkillEffectMagicCircleComponent->SetPlayClip("Effect/MagicCircle");
						_pSkillEffectComponent->SetRelativeLocation(bossPosition - GetRootComponent()->GetRelativeLocation());
						_pSkillEffectComponent->AddRelativeLocation({ 0.f, -100.f });
						_pSkillEffectComponent->SetPlayClip("Effect/Thunder");
						
						Manager->DealSkillDamageToBoss(_iSkill, CharacterType::Sorcerer);
						auto& noteSpawnQueue = Manager->GetNoteManager()->GetNoteSpawnQueue();
						if (!noteSpawnQueue.empty())
						{
							for (auto& data : noteSpawnQueue)
							{
								if (data._noteTime > BGMPosition)
								{
									lightningStartPosition = data._noteTime;
									break;
								}
								lightningStartPosition = Manager->GetNoteManager()->GetCurrNotePosition();
							}
						}
						else
						{
							lightningStartPosition = Manager->GetNoteManager()->GetCurrNotePosition();
						}
						--lightningCount;
						DebugPrintf("Sorcerer 번개!!!\n");
					}
					
				}
				else {
					_pStateComponent->SetCurrentState(0);
					_isUseSkill = false;
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

	if (_pSkillEffectComponent->IsClipPlay("Effect/Thunder"))
	{
		_pSkillEffectComponent->Visible = true;
	}
	else
	{
		_pSkillEffectComponent->Visible = false;
	}


	
	if (_pSkillEffectMagicCircleComponent->IsClipPlay("Effect/MagicCircle"))
	{
		_pSkillEffectMagicCircleComponent->Visible = true;
	}
	else
	{
		_pSkillEffectMagicCircleComponent->Visible = false;
	}
	

}

void Client::Sorcerer::Fixed()
{
}

void Client::Sorcerer::EndPlay()
{
}

void Client::Sorcerer::ReviveInitialize()
{
	_pStateComponent->SetCurrentState(0);
	_pNoteManager = dynamic_cast<NoteManager*>(GetWorld()->FindActor("NoteManager"));
	Super::ReviveInitialize();
	lightningCount = 0;
}

void Client::Sorcerer::SetupInputComponent(Engine::InputComponent* inputComponent)
{
	if (_skillKey == 0)
	{
		_skillKey = DIK_W;
	}
	inputComponent->BindKeys(DIK_W, DIK_UP, DIP_DPAD_UP);
	inputComponent->AttachInputManager();
}

Client::Sorcerer* Client::Sorcerer::Create()
{
	return new Sorcerer;
}

void Client::Sorcerer::Destroy()
{
}

void Client::Sorcerer::Idle()
{
	if (_pBasicAnimationComponent->IsFrameEnd())
	{
		_pBasicAnimationComponent->SetPlayClip("Idle");
	}
}


void Client::Sorcerer::Attack()
{
	if (_pBasicAnimationComponent->IsClipEnd("Attack"))
	{
		_pBasicAnimationComponent->SetPlayClip("Attack");
		if (_pBoss != nullptr)
		{
			Manager->DealDamageToBoss(_iAttack);
		}
	}
}

void Client::Sorcerer::SoundAttack()
{
	Sound->PlaySound("Sorcerer_attack", (int)SoundChannel::CHANNEL_SFX_3, SoundLoop::NOT_LOOP); //사운드 체크 후 풀기
}

void Client::Sorcerer::Skill()
{
	_isUseSkill = true;
	_pBasicAnimationComponent->SetPlayClip("Skill");
	lightningCount = 6;

	auto& noteSpawnQueue = Manager->GetNoteManager()->GetNoteSpawnQueue();
	if (!noteSpawnQueue.empty())
	{
		long double BGMPosition = Sound->GetPosition(CHANNEL_BGM);
		for (auto& data : noteSpawnQueue)
		{
			if (data._noteTime > BGMPosition)
			{
				lightningStartPosition = data._noteTime;
				break;
			}
			lightningStartPosition = Manager->GetNoteManager()->GetCurrNotePosition();
		}
	}
	else
	{
		lightningStartPosition = Manager->GetNoteManager()->GetCurrNotePosition();
	}
}

void Client::Sorcerer::SoundSkill()
{
	Sound->PlaySound("Sorcerer_skill", (int)SoundChannel::CHANNEL_SFX_3, SoundLoop::NOT_LOOP);
}

