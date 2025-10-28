#include <CoreManager.h>
#include "GameManager.h"
#include "UIManager.h"
#include "UI.h"
#include "Setting.h"
#include "SoundBar.h"
#include "SettingLevel.h"
#include "BackGround.h"


bool Client::SettingLevel::BeginPlay()
{
	_pBackGround = SpawnActor(LAYER::BACKGROUND, "BackGround", BackGround::Create(), Mathf::Vector2{ 960.f, 540.f });
	_pSetting = SpawnActor(LAYER::OBJECT, "Setting", Setting::Create());
	_pSetting->SetUIFrame(UI_SCENE::Setting_Title_UI);
	_pSetting->SetVisible(true);

	return Level::BeginPlay();

}

void Client::SettingLevel::EndPlay()
{
	if (Manager->IsSceneChangeTrigger && IsEndPlay())
	{
		_pBackGround->OnCompleteDestroyMark();
		_pSetting->OnCompleteDestroyMark();
		Management->ScheduleDestroyPoint();
		SetEndPlay(false);
	}
}


