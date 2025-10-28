#include <CoreManager.h>

#include "GameManager.h"
#include "UIManager.h"
#include "UI.h"
#include "GameOverLevel.h"

bool Client::GameOverLevel::BeginPlay()
{
	//todo : 
	// 게임 오버 된 현제 스테이지 받아오기
	//_CurrentStage = Manager->GetCurrentStage();
	ReviveActor("BackGround");
	UIManager* uiManager = SpawnActor(LAYER::OBJECT, "UIManager", UIManager::Create());
	
	uiManager->SpawnUI(UI_SCENE::GameOver);

	_uiImageList = uiManager->GetImageList();
	_uiButtonList = uiManager->GetButtonList();

	_uiButtonList[0]->AddEnterEvent(this, &GameOverLevel::RestartStage);
	_uiButtonList[1]->AddEnterEvent(this, &GameOverLevel::GoToTitle);


	Manager->BeginPlay();

	return Level::BeginPlay();

}

void Client::GameOverLevel::EndPlay()
{
	if (Manager->IsSceneChangeTrigger && IsEndPlay())
	{
		for (auto& _uiImageList : _uiImageList)
		{
			_uiImageList->OnCompleteDestroyMark();
		}

		for (auto& _uiButtonList : _uiButtonList)
		{
			_uiButtonList->OnCompleteDestroyMark();
		}

		RemoveActor("BackGround");
		TerminateActor("UIManager");
		Management->ScheduleDestroyPoint();
		SetEndPlay(false);
	}
}

void Client::GameOverLevel::RestartStage()
{
	DebugPrintf("RestartStage");
	Manager->SetRestart(true);
	if (_PriviousStage == Stage::Stage4_2) {
		_PriviousStage = Stage::Stage4;
	}
	Manager->ChangeStage(_PriviousStage);
}

void Client::GameOverLevel::GoToTitle()
{
	DebugPrintf("GoToTitle");
	Manager->ChangeStage(Stage::Title);
}
