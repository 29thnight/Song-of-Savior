#include <SoundManager.h>
#include <World.h>
#include <Level.h>
#include <TimeManager.h>
#include "GameManager.h"
#include "UI.h"
#include "UIManager.h"
#include "DefautSetting.h"
#include "KeySetting.h"
#include "Setting.h"


void Client::Setting::BeginPlay()
{
	GetWorld()->SpawnActor(LAYER::OBJECT, "DefaultSetting", DefaultSetting::Create());
	GetWorld()->SpawnActor(LAYER::OBJECT, "KeySetting", KeySetting::Create());
	_pDefautSetting = dynamic_cast<DefaultSetting*>(GetWorld()->FindActor("DefaultSetting"));
	_pKeySetting = dynamic_cast<KeySetting*>(GetWorld()->FindActor("KeySetting"));
	_pDefautSetting->SetSetting(this);
	_pKeySetting->SetSetting(this);
}

void Client::Setting::Tick(_duration deltaSeconds)
{
	
}

void Client::Setting::Fixed()
{
}

void Client::Setting::EndPlay()
{
	
}

void Client::Setting::SetVisible(bool visible)
{
	//todo : 설정창 보이기, 숨기기
	if (visible)
	{
		_pDefautSetting->SetVisible(true);
	}
	else 
	{	
		_pDefautSetting->SetVisible(false);
	}
	
}

bool Client::Setting::GetVisible() const
{
	//todo: setting 창 열림 상태 반환
	return isSettingVisible;
}

void Client::Setting::SetUIFrame(UI_SCENE uiFrame)
{
	_currUIFrame = uiFrame;
	_pDefautSetting->SetUIFrame(uiFrame);
}

void Client::Setting::ChangeKeySetting()
{
	_pKeySetting->SetVisible(true);
	_pDefautSetting->SetVisible(false);
}

void Client::Setting::ReturnSetting()
{
	_pKeySetting->SetVisible(false);
	_pDefautSetting->SetVisible(true);
}

void Client::Setting::CloseSetting()
{
	_pDefautSetting->SetVisible(false);
	_pKeySetting->SetVisible(false);
}

void Client::Setting::Destroy()
{
	Base::Destroy();
}

