#include <World.h>
#include <Level.h>
#include <CoreManager.h>

#include "UI.h"
#include "UIManager.h"
#include "Setting.h"
#include "KeySetting.h"
#include "GameManager.h"

void Client::KeySetting::BeginPlay()
{
	DebugPrintf("KeySetting::BeginPlay\n");
}

void Client::KeySetting::Tick(_duration deltaSeconds)
{
}

void Client::KeySetting::Fixed()
{
}

void Client::KeySetting::EndPlay()
{
}

void Client::KeySetting::SetVisible(bool visible)
{
	//todo : 설정창 보이기, 숨기기
	if (visible)
	{
		//todo : 설정창 열기
		int CurrLevelIndex = GetWorld()->GetCurrLevelIndex();
		Engine::Level* pCurrLevel = GetWorld()->GetLevel(CurrLevelIndex);

		UIManager* uiManager = pCurrLevel->SpawnActor(LAYER::OBJECT, "UIManager_Key", UIManager::Create());
		uiManager->SpawnUI(UI_SCENE::KeySettings_UI);

		_uiImageList = uiManager->GetImageList();
		_uiButtonList = uiManager->GetButtonList();

		//button event binding
		_uiButtonList[0]->AddEnterEvent(this, &KeySetting::SetATypeKeyMap);
		_uiButtonList[1]->AddEnterEvent(this, &KeySetting::SetBTypeKeyMap);
		_uiButtonList[2]->AddEnterEvent(this, &KeySetting::SetPadTypeKeyMap);

		isKeySettingVisible = true;

	}
	else {
		//todo : 설정창 닫기
		isKeySettingVisible = false;

		DebugPrintf("isSettingVisible\n");
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
		GetWorld()->TerminateActor("UIManager_Key");
		Management->ScheduleDestroyPoint();
	}
}

void Client::KeySetting::Destroy()
{
	Base::Destroy();
}

void Client::KeySetting::SetATypeKeyMap()
{
	DebugPrintf("Set A Type Key Map\n");
	Manager->SetBindKeySetting(GameKeySettingType::A);
	_pSetting->ReturnSetting();
}

void Client::KeySetting::SetBTypeKeyMap()
{
	DebugPrintf("Set B Type Key Map\n");
	Manager->SetBindKeySetting(GameKeySettingType::B);
	_pSetting->ReturnSetting();
}

void Client::KeySetting::SetPadTypeKeyMap()
{
	DebugPrintf("Set Pad Type Key Map\n");
	Manager->SetBindKeySetting(GameKeySettingType::GamePad);
	_pSetting->ReturnSetting();
}
