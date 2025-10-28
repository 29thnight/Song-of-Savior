#include "UIManager.h"
#include <World.h>
#include <Level.h>
#include <BitmapComponent.h>
#include <InputComponent.h>
#include <TimerComponent.h>
#include <VideoManager.h>
#include <TimeManager.h>
#include <SoundManager.h>

#include "UI.h"

void Client::UIManager::BeginPlay()
{
	_pInputComponent = AddComponent<Engine::InputComponent>("InputComponent");
	_pTimerComponent = AddComponent<Engine::TimerComponent>("TimerComponent");
	_pTimerComponent->AddTimer(2);
	_pTimerComponent->GetTimer(0).onTimeOver.AddLambda([this]()
		{
			SetFocusUI(0);
		});
}

void Client::UIManager::Tick(_duration deltaSeconds)
{
	Super::Tick(Time->GetNonScaleDeltaSeconds());

	//if (VideoMgr->IsPlaying())
	//{
	//	return;
	//}

	constexpr float gamepadScrollDelay = 0.3f;
	if (-0.5f >= _pInputComponent->IsLeftAxisMove().y)
	{
		if (-0.5f >= _pInputComponent->IsLeftAxisMove().y && _pTimerComponent->GetTimer(1).isEnd)
		{
			_pTimerComponent->SetTime(1, gamepadScrollDelay);
			_gamePadUp = true;
		}
		else
		{
			_gamePadUp = false;
		}
		//DebugPrintf("x : %f, y : %f\n", _pInputComponent->IsLeftAxisMove().x, _pInputComponent->IsLeftAxisMove().y);
	}
	else if (0.5f <= _pInputComponent->IsLeftAxisMove().y)
	{
		if (0.5f <= _pInputComponent->IsLeftAxisMove().y && _pTimerComponent->GetTimer(1).isEnd)
		{
			_pTimerComponent->SetTime(1, gamepadScrollDelay);
			_gamePadDown = true;
		}
		else
		{
			_gamePadDown = false;
		}
		//DebugPrintf("x : %f, y : %f\n", _pInputComponent->IsLeftAxisMove().x, _pInputComponent->IsLeftAxisMove().y);
	}
	else
	{
		_gamePadUp = false;
		_gamePadDown = false;
	}
	
	if (!_vecButtonUI.empty())
	{
		if (_bIsGameOverUI)
		{
			if (-0.5f >= _pInputComponent->IsLeftAxisMove().x)
			{
				if (-0.5f >= _pInputComponent->IsLeftAxisMove().x && _pTimerComponent->GetTimer(1).isEnd)
				{
					_pTimerComponent->SetTime(1, gamepadScrollDelay);
					_gamePadUp = true;
				}
				else
				{
					_gamePadUp = false;
				}
				//DebugPrintf("x : %f, y : %f\n", _pInputComponent->IsLeftAxisMove().x, _pInputComponent->IsLeftAxisMove().y);
			}
			else if (0.5f <= _pInputComponent->IsLeftAxisMove().x)
			{
				if (0.5f <= _pInputComponent->IsLeftAxisMove().x && _pTimerComponent->GetTimer(1).isEnd)
				{
					_pTimerComponent->SetTime(1, gamepadScrollDelay);
					_gamePadDown = true;
				}
				else
				{
					_gamePadDown = false;
				}
				//DebugPrintf("x : %f, y : %f\n", _pInputComponent->IsLeftAxisMove().x, _pInputComponent->IsLeftAxisMove().y);
			}
			else
			{
				_gamePadUp = false;
				_gamePadDown = false;
			}

			if (!_vecButtonUI.empty()) 
			{
				if (_pInputComponent->IsKeyPress(DIK_LEFT) ||
					_gamePadUp) 
				{
					_gamePadUp = false;
					FocusButtonUp();
				}
				else if (_pInputComponent->IsKeyPress(DIK_RIGHT) ||
					_gamePadDown)
				{
					_gamePadDown = false;
					FocusButtonDown();
				}
			}

			if (_focusUIindex >= 0 && (_pInputComponent->IsKeyPress(DIK_SPACE) || _pInputComponent->IsKeyPress(DIP_B)))
			{
				Sound->PlaySound("ButtonSelect_UI", SoundChannel::CHANNEL_SFX_5, SoundLoop::NOT_LOOP);
				_vecButtonUI[_focusUIindex]->CallEnterEvent();
			}
		}
		else
		{
			if (
				_pInputComponent->IsKeyPress(DIK_UP) ||
				_pInputComponent->IsKeyPress(DIK_W) ||
				_gamePadUp
				)
			{
				_gamePadUp = false;
				FocusButtonUp();
			}
			else if (
				_pInputComponent->IsKeyPress(DIK_DOWN) ||
				_pInputComponent->IsKeyPress(DIK_S) ||
				_gamePadDown
				)
			{
				_gamePadDown = false;
				FocusButtonDown();
			}

			if (_focusUIindex >= 0 && (_pInputComponent->IsKeyPress(DIK_SPACE) || _pInputComponent->IsKeyPress(DIP_B)))
			{
				Sound->PlaySound("ButtonSelect_UI", SoundChannel::CHANNEL_SFX_5, SoundLoop::NOT_LOOP);
				_vecButtonUI[_focusUIindex]->CallEnterEvent();
			}
		}	
	}
}

void Client::UIManager::Fixed()
{
}

void Client::UIManager::EndPlay()
{
}

void Client::UIManager::SetupInputComponent(Engine::InputComponent* inputComponent)
{
	inputComponent->BindKeys(DIK_W, DIK_S, DIK_UP, DIK_DOWN, DIK_SPACE, DIP_LY, DIP_B, DIK_LEFT, DIK_RIGHT,DIP_LX);

	inputComponent->AttachInputManager();
}

void Client::UIManager::Destroy()
{
	UIAllDestroy();
	Actor::Destroy(); 
}

const std::vector<Client::UI*>& Client::UIManager::GetButtonList()
{
	return _vecButtonUI;
}

const std::vector<Client::UI*>& Client::UIManager::GetImageList()
{
	return _vecImageUI;
}

void Client::UIManager::SpawnUI(UI_SCENE scene)
{
	std::vector<Actor*> spawnActorList;
	_bIsGameOverUI = false;
	switch (scene)
	{
	case Client::Title_Default:
		GetWorld()->LoadLevelToJson(L"Title_기본_UI\\Title_기본.json", &spawnActorList);
		
		break;
	case Client::Title_Continue:
		GetWorld()->LoadLevelToJson(L"Title_이어하기_UI\\Title_이어하기.json", &spawnActorList);
		
		break;
	case Client::GameOver:
		GetWorld()->LoadLevelToJson(L"GameOver_UI\\GameOver.json", &spawnActorList);
		_bIsGameOverUI = true;
		break;
	case Client::Setting_UI:
		GetWorld()->LoadLevelToJson(L"Setting_UI\\Setting.json", &spawnActorList);
		break;
	case Client::Setting_Title_UI:
		GetWorld()->LoadLevelToJson(L"Setting_Title_UI\\Setting_Title.json", &spawnActorList);
		break;
	case Client::KeySettings_UI:
		GetWorld()->LoadLevelToJson(L"KeySettings_UI\\KeySettings.json", &spawnActorList);
		break;
	default:
		break;
	}
	std::vector<UI*> vecUI;
	vecUI.reserve(spawnActorList.size());
	for (auto& actor : spawnActorList)
	{
		vecUI.emplace_back(dynamic_cast<UI*>(actor));
	}
	_vecButtonUI.clear();
	for (auto& ui : vecUI)
	{
		if (ui->_pSecondaryBitmap->GetOpacity() == 1.0f)
		{
			_vecButtonUI.push_back(ui);
		}
		else {
			_vecImageUI.push_back(ui);
		}
	}
	//DebugPrintf("_vecImageUI.size : %d\n", _vecImageUI.size());
	std::sort(_vecButtonUI.begin(), _vecButtonUI.end(), [](const UI* fir, const UI* sec)->bool
		{
			Mathf::Vector2 firPosition = fir->GetRootComponent()->GetRelativeLocation();
			Mathf::Vector2 secPosition = sec->GetRootComponent()->GetRelativeLocation();

			if (firPosition.x != secPosition.x)
				return firPosition.x < secPosition.x;
			else
				return firPosition.y < secPosition.y;
		}
	);
	_pTimerComponent->SetTime(0, 0.3f);
}

void Client::UIManager::SetFocusUI(int index)
{
	_focusUIindex = index;
	_vecButtonUI[_focusUIindex]->_isFocus = true;
	
}

void Client::UIManager::FocusButtonUp()
{
	if (_focusUIindex != -1)
		_vecButtonUI[_focusUIindex]->_isFocus = false;

	_focusUIindex--;
	if (_focusUIindex < 0)
	{
		_focusUIindex = static_cast<int>(_vecButtonUI.size() - 1);
	}
	Sound->PlaySound("ButtonSwoosh_UI", SoundChannel::CHANNEL_SFX_5, SoundLoop::NOT_LOOP);
	_vecButtonUI[_focusUIindex]->_isFocus = true;
}

void Client::UIManager::FocusButtonDown()
{
	if (_focusUIindex != -1)
		_vecButtonUI[_focusUIindex]->_isFocus = false;

	_focusUIindex++;
	if (_focusUIindex >= _vecButtonUI.size())
	{
		_focusUIindex = 0;
	}
	Sound->PlaySound("ButtonSwoosh_UI", SoundChannel::CHANNEL_SFX_5, SoundLoop::NOT_LOOP);
	_vecButtonUI[_focusUIindex]->_isFocus = true;
}

void Client::UIManager::UIAllDestroy()
{
	if (!_vecButtonUI.empty())
		for (auto& actor : _vecButtonUI)
		{
			GetWorld()->TerminateActor(actor->GetName());
		}
	if (!_vecImageUI.empty())
		for (auto& actor : _vecImageUI)
		{
			GetWorld()->TerminateActor(actor->GetName());
		}
}
