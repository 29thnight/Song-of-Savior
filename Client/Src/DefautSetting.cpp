#include <SoundManager.h>
#include <World.h>
#include <Level.h>
#include <InputComponent.h>
#include <TimeManager.h>
#include <CoreManager.h>
#include <TimerComponent.h>

#include "UI.h"
#include "UIManager.h"

#include "GameManager.h"
#include "SoundBar.h"
#include "Setting.h"
#include "DefautSetting.h"

void Client::DefaultSetting::BeginPlay()
{
	GetWorld()->SpawnActor(LAYER::UI, "SoundBar", SoundBar::Create(), { 850.0f, 408.0f });
	
	_pSoundBar = dynamic_cast<SoundBar*>(GetWorld()->FindActor("SoundBar"));
	_pInputComponent = AddComponent<Engine::InputComponent>("InputComponent");
	_pTimerComponent = AddComponent<Engine::TimerComponent>("TimerComponent");
	_pTimerComponent->AddTimer(2);
}

void Client::DefaultSetting::Tick(_duration deltaSeconds)
{
	Super::Tick(Time->GetNonScaleDeltaSeconds());

	if (isDefautSettingVisible)
	{
		constexpr float gamepadScrollDelay = 0.3f;
		if (-0.5f >= _pInputComponent->IsLeftAxisMove().x)
		{
			if (-0.5f >= _pInputComponent->IsLeftAxisMove().x && _pTimerComponent->GetTimer(1).isEnd)
			{
				_pTimerComponent->SetTime(1, gamepadScrollDelay);
				_gamePadLeft = true;
			}
			else
			{
				_gamePadLeft = false;
			}
			//DebugPrintf("x : %f, y : %f\n", _pInputComponent->IsLeftAxisMove().x, _pInputComponent->IsLeftAxisMove().y);
		}
		else if (0.5f <= _pInputComponent->IsLeftAxisMove().x)
		{
			if (0.5f <= _pInputComponent->IsLeftAxisMove().x && _pTimerComponent->GetTimer(1).isEnd)
			{
				_pTimerComponent->SetTime(1, gamepadScrollDelay);
				_gamePadRight = true;
			}
			else
			{
				_gamePadRight = false;
			}
			//DebugPrintf("x : %f, y : %f\n", _pInputComponent->IsLeftAxisMove().x, _pInputComponent->IsLeftAxisMove().y);
		}
		else
		{
			_gamePadLeft = false;
			_gamePadRight = false;
		}


		if (!_uiButtonList.empty()) {
			if (_uiButtonList[0]->_isFocus) {
				if (_pInputComponent->IsKeyPress(DIK_LEFT) || _gamePadLeft)
				{
					VolumeDown();
					_uiButtonList[0]->_isFocus = true;
					_gamePadLeft = false;
				}
				if (_pInputComponent->IsKeyPress(DIK_RIGHT) || _gamePadRight)
				{
					VolumeUp();
					_uiButtonList[0]->_isFocus = true;
					_gamePadRight = false;
				}
			}
		}
	}
}

void Client::DefaultSetting::Fixed()
{
}

void Client::DefaultSetting::EndPlay()
{
}

void Client::DefaultSetting::SetVisible(bool visible)
{
	if (visible)
	{
		int CurrLevelIndex = GetWorld()->GetCurrLevelIndex();
		Engine::Level* pCurrLevel = GetWorld()->GetLevel(CurrLevelIndex);

		_CurrStage = Manager->GetCurrentStage();

		UIManager* uiManager = pCurrLevel->SpawnActor(LAYER::OBJECT, "UIManager_Setting", UIManager::Create());
		uiManager->SpawnUI(_currUIFrame);

		_uiImageList = uiManager->GetImageList();
		_uiButtonList = uiManager->GetButtonList();

		//_pSoundBar = pCurrLevel->SpawnActor(LAYER::UI, "SoundBar", SoundBar::Create(),{ 850.0f, 408.0f });
		if ((int)Stage::Setting == CurrLevelIndex)
		{
			_pSoundBar->SetVisible(true);
			_uiButtonList[1]->AddEnterEvent(this, &DefaultSetting::BindKeyMap);
			_uiButtonList[2]->AddEnterEvent(this, &DefaultSetting::GoTitle);
		}
		else
		{
			_pSoundBar->SetVisible(true);
			_uiButtonList[1]->AddEnterEvent(this, &DefaultSetting::BindKeyMap);
			_uiButtonList[2]->AddEnterEvent(this, &DefaultSetting::ReStartStage);
			_uiButtonList[3]->AddEnterEvent(this, &DefaultSetting::GoTitle);
		}

		/*for (auto button : _uiButtonList )
		{
			DebugPrintf("%s", button->GetName());
		}*/
		isDefautSettingVisible = true;
	}
	else {
		//todo : 설정창 닫기
		isDefautSettingVisible = false;

		_pSoundBar->SetVisible(false);

		int i = std::_Max_int_dig;
		for (auto image : _uiImageList)
		{
			image->SetName(std::to_string(i).c_str());
			GetWorld()->TerminateActor(image->GetName());
			i--;
		}
		for (auto ui : _uiButtonList)
		{
			ui->SetName(std::to_string(i).c_str());
			GetWorld()->TerminateActor(ui->GetName());
			i--;
		}
		GetWorld()->TerminateActor("UIManager_Setting");
		DebugPrintf("isSettingVisible");
	}
}

bool Client::DefaultSetting::GetVisible() const
{
	return isDefautSettingVisible;
}



Client::DefaultSetting* Client::DefaultSetting::Create()
{
	return new DefaultSetting();
}

void Client::DefaultSetting::Destroy()
{
	_pSoundBar->Destroy();
	Base::Destroy();
}

void Client::DefaultSetting::SetUIFrame(UI_SCENE uiFrame)
{
	_currUIFrame = uiFrame;
}

void Client::DefaultSetting::SetUI(std::vector<UI*> uiButtonList, std::vector<UI*> uiImageList)
{
	_uiButtonList = uiButtonList;
	_uiImageList = uiImageList;
}

void Client::DefaultSetting::SetUiButtonList(std::vector<UI*> uiButtonList)
{
	_uiButtonList = uiButtonList;
}

void Client::DefaultSetting::SetUiImageList(std::vector<UI*> uiImageList)
{
	_uiImageList = uiImageList;
}



void Client::DefaultSetting::SetupInputComponent(Engine::InputComponent* inputComponent)
{
	inputComponent->BindKeys(DIK_LEFT, DIK_RIGHT,DIP_LX);

	inputComponent->AttachInputManager();
}


void Client::DefaultSetting::VolumeUp()
{
	DebugPrintf("VolumeUp  ");

	float volume = Sound->GetMasterVolume();
	volume += 0.1f;  // 1.0f 대신 0.1f를 더하여 0 ~ 1 사이에서 증가하도록
	if (volume > 1.0f)
	{
		volume = 1.0f;  // 최대 볼륨은 1.0f
	}

	Sound->SetMasterVolume(volume);
	DebugPrintf("Volume : %f \n", volume);
}

void Client::DefaultSetting::VolumeDown()
{
	DebugPrintf("VolumeDown  ");

	float volume = Sound->GetMasterVolume();
	volume -= 0.1f;  // 1.0f 대신 0.1f를 빼서 0 ~ 1 사이에서 감소하도록
	if (volume < 0.0f)
	{
		volume = 0.0f;  // 최소 볼륨은 0.0f
	}

	Sound->SetMasterVolume(volume);
	DebugPrintf("Volume : %f \n", volume);
}


void Client::DefaultSetting::BindKeyMap()
{
	//todo : 세팅 keySetting open
	_pSetting->ChangeKeySetting();
}

void Client::DefaultSetting::ReStartStage()
{
	//todo : 스테이지 재시작
	DebugPrintf("RestartStage");
	Manager->SetRestart(true);
	Manager->ChangeStage(_CurrStage);
	_pSetting->CloseSetting();
}

void Client::DefaultSetting::GoTitle()
{
	//todo : 타이틀로 이동
	DebugPrintf("GoToTitle");
	Manager->ChangeStage(Stage::Title);
	_pSetting->CloseSetting();
}
