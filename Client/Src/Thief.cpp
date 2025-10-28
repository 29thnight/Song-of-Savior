#include <Actor.h>
#include <BitmapComponent.h>
#include <BasicAnimationComponent.h>
#include <StateComponent.h>
#include <SimpleIniLoader.h>
#include <TextureManager.h>
#include <InputComponent.h>
#include <World.h>
#include <SoundManager.h>
#include <TimerComponent.h>
#include <TimeManager.h>
#include "Boss.h"
#include "NoteManager.h"
#include "Thief.h"
#include "GameManager.h"
#include "HPComponent.h"
#include "ClientEnum.h"

void Client::Thief::BeginPlay()
{
	Character::BeginPlay();
	//_pAnimationComponent->SetPlayClip("Idle");

	std::string absolutePath = "Assets/Thief/ThiefData.ini";
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

	_iMaxHp = std::stoi(iniFile.GetValue("ThiefData", "MaxHp", "0"));
	_iAttack = std::stoi(iniFile.GetValue("ThiefData", "AttackDamage", "0"));
	_iMaxMP = std::stoi(iniFile.GetValue("ThiefData", "MaxMp", "0"));
	_iSkillDemandNote = std::stoi(iniFile.GetValue("ThiefData", "SkillDemandNote", "0"));
	_iSkill = std::stoi(iniFile.GetValue("ThiefData", "SkillDamage", "0"));
	
	_iHp = _iMaxHp;
	_iMp = _iMaxMP;

	//_pAnimationComponent->SetPlayClip("Idle");
	_pBasicAnimationComponent->SetRelativeScale({ 0.4f, 0.4f });
	_pBasicAnimationComponent->SetRelativeLocation({ 15.f, 15.f });
	_pBasicAnimationComponent->AllAddClipThisActor();
	_pBasicAnimationComponent->SetPlayClip("Idle");

	_pStateComponent = AddComponent<Engine::StateComponent>("StateComponent");

	_pInputComponent = AddComponent<Engine::InputComponent>("InputComponent");

	_pBitmapKnifeComponent = AddComponent<Engine::BitmapComponent>("BitmapKnifeComponent");

	_pStateComponent->AddState(0, "Idle");
	_pStateComponent->AddState(1, "Attack");
	_pStateComponent->AddState(2, "Skill");
	_pStateComponent->AddState(3, "Damage");
	_pStateComponent->AddState(4, "Dead");

	_pStateComponent->GetState(0)->OnEnter.AddDynamic(this, &Thief::Idle);

	_pStateComponent->GetState(1)->OnEnter.AddDynamic(this, &Thief::Attack);
	_pStateComponent->GetState(1)->OnUpdate.AddDynamic(this, &Thief::AttackUpdate);

	_pStateComponent->GetState(2)->OnEnter.AddDynamic(this, &Thief::Skill);
	_pStateComponent->GetState(2)->OnUpdate.AddDynamic(this, &Thief::SkillUpdate);

	/*_pStateComponent->GetState(3)->OnEnter.AddDynamic(this, &Thief::Damage);

	_pStateComponent->GetState(4)->OnEnter.AddDynamic(this, &Thief::Dead);*/

	_vInitPosition = _pBasicAnimationComponent->GetRelativeLocation();

	_pHPComponent = AddComponent<HPComponent>("HPComponent");
	_pHPComponent->SetRelativeLocation({ 0.f, 200.f });

	_pNoteManager = dynamic_cast<NoteManager*>(GetWorld()->FindActor("NoteManager"));
	_pTimerManager = AddComponent<Engine::TimerComponent>("TimerComponent");

	_pTimerManager->AddTimer(3);
	_pTimerManager->GetTimer(0).onTimeOver.AddDynamic(this, &Thief::InitPosition);

	_pBitmapKnifeComponent->SetTextures(&_vecTextures);
	_pBitmapKnifeComponent->SetBitmapIndex(0);
	_pBitmapKnifeComponent->SetRelativeRotation(70.f);
	_pBitmapKnifeComponent->SetRelativeScale({ 0.5f, 0.5f });
	_pBitmapKnifeComponent->SetRelativeLocation({ 100.f, 0.f });
	_vInitKnifePosition = _pBitmapKnifeComponent->GetRelativeLocation();
	_pBitmapKnifeComponent->SetVisible(false);

	_pBasicAnimationComponent->AddFrameEvent("Attack", 13, this, &Thief::VisibleKnife);
	//케릭터 타입
	_characterType = 1;
}

void Client::Thief::Tick(_duration deltaSeconds)
{
	Character::Tick(deltaSeconds);

	if (Manager->GetCurrentStage() == Stage::Dialouge)
	{
		return;
	}
	//DebugPrintf("Skill x:%d y:%d\n", _pAnimationComponent->GetRelativeLocation().x, _pAnimationComponent->GetRelativeLocation().y);

	if (!_bIsDead) //죽지 않았을때
	{
		if (_iMp >= _iSkillDemandNote)	 //스킬사용가능 여부 업데이트
		{
			_bIsSkillOn = true;
		}
		else
		{
			_bIsSkillOn = false;
		}

		if (_pStateComponent->GetCurrentState() != _pStateComponent->GetState(2)) { //스킬 상태가 아닐때
			
			if (_pInputComponent->IsKeyPress(_skillKey) && !Sound->GetPause(CHANNEL_BGM)) //스킬키 입력시
			{
				if (_iMp >= _iSkillDemandNote) //마나가 충족 되면
				{
					_pStateComponent->SetCurrentState(2); //스킬 상태로
				}
			}

			//노트 판정 성공시 기본공격
			if (_pNoteManager->IsPerfect)  
			{
				_pStateComponent->SetCurrentState(1);
			}
			else if (_pNoteManager->IsGreat)
			{
				_pStateComponent->SetCurrentState(1);
			}
			else if (_pNoteManager->IsGood)
			{
				_pStateComponent->SetCurrentState(1);
			}
		}
		
		
		//마나 최대치 제한
		if (_iNotePoint > _iSkillDemandNote)
		{
			_iNotePoint = _iSkillDemandNote;
		}
	}

	//단검 던지기
	ThrowKnife(0.1f); //단검 활성화시 단검 날아가기
}

void Client::Thief::Fixed()
{
}

void Client::Thief::EndPlay()
{
}

void Client::Thief::ReviveInitialize()
{
	_pStateComponent->SetCurrentState(0);
	_pNoteManager = dynamic_cast<NoteManager*>(GetWorld()->FindActor("NoteManager"));
	Super::ReviveInitialize();
}

void Client::Thief::SetupInputComponent(Engine::InputComponent* inputComponent)
{
	if (_skillKey == 0)
	{
		_skillKey = DIK_E;
	}
	inputComponent->BindKeys(DIK_E, DIK_LEFT, DIP_DPAD_LEFT);
	inputComponent->AttachInputManager();
}

Client::Thief* Client::Thief::Create()
{
	return new Thief;
}

void Client::Thief::Destroy()
{
}

void Client::Thief::Idle()
{
	if (_pBasicAnimationComponent->IsFrameEnd())
	{
		_pBasicAnimationComponent->SetPlayClip("Idle");
	}
}

//도적 기본 공격
void Client::Thief::Attack()
{
 	_pBasicAnimationComponent->SetPlayClip("Attack");
}

//도적 공격 업데이트
void Client::Thief::AttackUpdate()
{
	
	if (_pBasicAnimationComponent->IsFrameEnd()) //공격 애니메이션이 끝나면
	{
		DebugPrintf("ATTACK\n");
		//_pBitmapKnifeComponent->SetVisible(true); //단검 활성화
		_pStateComponent->SetCurrentState(0); //대기 상태로 전환
	}	
}



//도적 스킬 시작
void Client::Thief::Skill()
{
	Mathf::Vector2 bossPosition = Manager->GetCurrentBoss()->GetRootComponent()->GetRelativeLocation();

	_iMp -= _iSkillDemandNote;
	//_pAnimationComponent->SetRelativeLocation(bossPosition);
	_pBasicAnimationComponent->SetRelativeLocation(bossPosition - GetRootComponent()->GetRelativeLocation());
	_pBasicAnimationComponent->AddRelativeLocation({ 100.f, 300.f });
	_pBasicAnimationComponent->SetFlip(true);
	_pBasicAnimationComponent->SetPlayClip("Skill");
	_pBasicAnimationComponent->StartReveal(0.5f);
	_pBasicAnimationComponent->SetOrder(2);
	prevPosition = Sound->GetPosition(CHANNEL_BGM);
	_pTimerManager->SetTimeSyncMusic(0, CHANNEL_BGM, 2);	
	Sound->PlaySound("Thief_Skill", (int)SoundChannel::CHANNEL_SFX_2, SoundLoop::NOT_LOOP);
}

//도적 스킬 업데이트
void Client::Thief::SkillUpdate()
{	
	float duration = 1.3f; //스킬 이동 시간

	long double currPosition = Sound->GetPosition(CHANNEL_BGM); // 현제 시간 읽기
	float frameTime = static_cast<float>((currPosition - prevPosition) * Mathf::NanoToSec); //틱 시간 계산
	prevPosition = currPosition; //이전 시간 갱신
	_fSkillTime += frameTime; //스킬 진행 시간
	Mathf::Vector2 move = { -100.f, 0.f }; //한틱 마다 의 이동량

	//DebugPrintf("_fSkillTime %f\n",_fSkillTime);
	if (duration > _fSkillTime) //이동 시간이 지나지 않으면 이동
	{
		_pBasicAnimationComponent->AddRelativeLocation(move * frameTime); //이동
	}
	

	if (_pBasicAnimationComponent->IsFrameEnd() && !_isHideAndBush) //애니메이션이 끝나면
	{
		_pBasicAnimationComponent->StartConceal(3.f);
		_isHideAndBush = true;
		_pBasicAnimationComponent->SetPlayClip("Idle"); //대기 상태로 전환
		_fSkillTime = 0; //스킬 시간 초기화
		
		Manager->DealSkillDamageToBoss(_iSkill, CharacterType::Thief); // 스킬 대미지 주기
	}
}

void Client::Thief::SoundSkill()
{
}

////도적 피격 상태
//void Client::Thief::Damage()
//{
//	_pBasicAnimationComponent->SetAlphaMaskEffect(true);
//	_pBasicAnimationComponent->SetAlphaMaskOpacity(0.5f);
//	_pBasicAnimationComponent->SetAlphaMaskDuration(1.5f);
//}



//도적 스킬 초기화
void Client::Thief::InitPosition()
{
	DebugPrintf("InitPosition\n");
	_pBasicAnimationComponent->SetRelativeLocation(_vInitPosition);
	_pBasicAnimationComponent->SetFlip(false);
	_pStateComponent->SetCurrentState(0);
	_isHideAndBush = false;
	_pBasicAnimationComponent->StartReveal(0.8f);
	_pBasicAnimationComponent->SetOrder(0);
}

void Client::Thief::VisibleKnife()
{
	_pBitmapKnifeComponent->SetVisible(true);
	Sound->PlaySound("Thief_throw", (int)SoundChannel::CHANNEL_SFX_2, SoundLoop::NOT_LOOP); //단검 던지는 사운드 체크 후 풀기
}

// 단검 던지기
void Client::Thief::ThrowKnife(float duration)
{
	if (_pBitmapKnifeComponent->IsVisible()) //단검이 활성화 되었을때
	{
		long double currPosition = Sound->GetPosition(CHANNEL_BGM); //현재 음악의 위치

		float frametime = static_cast<float>((currPosition - _fKnifePrevTime) * Mathf::NanoToSec); //한 틱 시간
		if (_fKnifePrevTime != 0) _fThrowKnifeTime += frametime;  //틱 경과 시간
		_fKnifePrevTime = currPosition; //이전 시간 갱신


		Mathf::Vector2 bossPosition = Manager->GetCurrentBoss()->GetRootComponent()->GetRelativeLocation(); //보스의 위치
		Mathf::Vector2 endPosition = bossPosition - GetRootComponent()->GetRelativeLocation();  //단검 종료 위치

		//지속 시간에 대한 틱 경과 시간에 따른 위치 계산
		Mathf::Vector2 location = (_vInitKnifePosition * (1 - (_fThrowKnifeTime / duration))) + endPosition * (_fThrowKnifeTime / duration);
		//단검 위치
		_pBitmapKnifeComponent->SetRelativeLocation(location);

		//지속 시간이 지나면 대미지를 주고 초기화
		if ((_fThrowKnifeTime / duration) > 1)
		{
			Manager->DealDamageToBoss(_iAttack); //대미지 주기
			_fThrowKnifeTime = 0;  //위치 시간 초기화
			_fKnifePrevTime = 0;  //틱 계산 시간 초기화
			_pBitmapKnifeComponent->SetRelativeLocation(_vInitKnifePosition); //위치 초기화

			_pBitmapKnifeComponent->SetVisible(false); //단검 비활성화

			return;
		}
	}
}
