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
	//�ɸ��� Ÿ��
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

	if (!_bIsDead) //���� �ʾ�����
	{
		if (_iMp >= _iSkillDemandNote)	 //��ų��밡�� ���� ������Ʈ
		{
			_bIsSkillOn = true;
		}
		else
		{
			_bIsSkillOn = false;
		}

		if (_pStateComponent->GetCurrentState() != _pStateComponent->GetState(2)) { //��ų ���°� �ƴҶ�
			
			if (_pInputComponent->IsKeyPress(_skillKey) && !Sound->GetPause(CHANNEL_BGM)) //��ųŰ �Է½�
			{
				if (_iMp >= _iSkillDemandNote) //������ ���� �Ǹ�
				{
					_pStateComponent->SetCurrentState(2); //��ų ���·�
				}
			}

			//��Ʈ ���� ������ �⺻����
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
		
		
		//���� �ִ�ġ ����
		if (_iNotePoint > _iSkillDemandNote)
		{
			_iNotePoint = _iSkillDemandNote;
		}
	}

	//�ܰ� ������
	ThrowKnife(0.1f); //�ܰ� Ȱ��ȭ�� �ܰ� ���ư���
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

//���� �⺻ ����
void Client::Thief::Attack()
{
 	_pBasicAnimationComponent->SetPlayClip("Attack");
}

//���� ���� ������Ʈ
void Client::Thief::AttackUpdate()
{
	
	if (_pBasicAnimationComponent->IsFrameEnd()) //���� �ִϸ��̼��� ������
	{
		DebugPrintf("ATTACK\n");
		//_pBitmapKnifeComponent->SetVisible(true); //�ܰ� Ȱ��ȭ
		_pStateComponent->SetCurrentState(0); //��� ���·� ��ȯ
	}	
}



//���� ��ų ����
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

//���� ��ų ������Ʈ
void Client::Thief::SkillUpdate()
{	
	float duration = 1.3f; //��ų �̵� �ð�

	long double currPosition = Sound->GetPosition(CHANNEL_BGM); // ���� �ð� �б�
	float frameTime = static_cast<float>((currPosition - prevPosition) * Mathf::NanoToSec); //ƽ �ð� ���
	prevPosition = currPosition; //���� �ð� ����
	_fSkillTime += frameTime; //��ų ���� �ð�
	Mathf::Vector2 move = { -100.f, 0.f }; //��ƽ ���� �� �̵���

	//DebugPrintf("_fSkillTime %f\n",_fSkillTime);
	if (duration > _fSkillTime) //�̵� �ð��� ������ ������ �̵�
	{
		_pBasicAnimationComponent->AddRelativeLocation(move * frameTime); //�̵�
	}
	

	if (_pBasicAnimationComponent->IsFrameEnd() && !_isHideAndBush) //�ִϸ��̼��� ������
	{
		_pBasicAnimationComponent->StartConceal(3.f);
		_isHideAndBush = true;
		_pBasicAnimationComponent->SetPlayClip("Idle"); //��� ���·� ��ȯ
		_fSkillTime = 0; //��ų �ð� �ʱ�ȭ
		
		Manager->DealSkillDamageToBoss(_iSkill, CharacterType::Thief); // ��ų ����� �ֱ�
	}
}

void Client::Thief::SoundSkill()
{
}

////���� �ǰ� ����
//void Client::Thief::Damage()
//{
//	_pBasicAnimationComponent->SetAlphaMaskEffect(true);
//	_pBasicAnimationComponent->SetAlphaMaskOpacity(0.5f);
//	_pBasicAnimationComponent->SetAlphaMaskDuration(1.5f);
//}



//���� ��ų �ʱ�ȭ
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
	Sound->PlaySound("Thief_throw", (int)SoundChannel::CHANNEL_SFX_2, SoundLoop::NOT_LOOP); //�ܰ� ������ ���� üũ �� Ǯ��
}

// �ܰ� ������
void Client::Thief::ThrowKnife(float duration)
{
	if (_pBitmapKnifeComponent->IsVisible()) //�ܰ��� Ȱ��ȭ �Ǿ�����
	{
		long double currPosition = Sound->GetPosition(CHANNEL_BGM); //���� ������ ��ġ

		float frametime = static_cast<float>((currPosition - _fKnifePrevTime) * Mathf::NanoToSec); //�� ƽ �ð�
		if (_fKnifePrevTime != 0) _fThrowKnifeTime += frametime;  //ƽ ��� �ð�
		_fKnifePrevTime = currPosition; //���� �ð� ����


		Mathf::Vector2 bossPosition = Manager->GetCurrentBoss()->GetRootComponent()->GetRelativeLocation(); //������ ��ġ
		Mathf::Vector2 endPosition = bossPosition - GetRootComponent()->GetRelativeLocation();  //�ܰ� ���� ��ġ

		//���� �ð��� ���� ƽ ��� �ð��� ���� ��ġ ���
		Mathf::Vector2 location = (_vInitKnifePosition * (1 - (_fThrowKnifeTime / duration))) + endPosition * (_fThrowKnifeTime / duration);
		//�ܰ� ��ġ
		_pBitmapKnifeComponent->SetRelativeLocation(location);

		//���� �ð��� ������ ������� �ְ� �ʱ�ȭ
		if ((_fThrowKnifeTime / duration) > 1)
		{
			Manager->DealDamageToBoss(_iAttack); //����� �ֱ�
			_fThrowKnifeTime = 0;  //��ġ �ð� �ʱ�ȭ
			_fKnifePrevTime = 0;  //ƽ ��� �ð� �ʱ�ȭ
			_pBitmapKnifeComponent->SetRelativeLocation(_vInitKnifePosition); //��ġ �ʱ�ȭ

			_pBitmapKnifeComponent->SetVisible(false); //�ܰ� ��Ȱ��ȭ

			return;
		}
	}
}
