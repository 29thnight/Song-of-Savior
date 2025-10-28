#include <CoreManager.h>
#include <World.h>
#include <CameraActor.h>
#include <SoundManager.h>
#include <VideoManager.h>
#include <TimeManager.h>
#include <InputManager.h>
#include <GraphicsManager.h>

#include "TitleLevel.h"
#include "Title.h"
#include "GameManager.h"
#include "UIManager.h"
#include "Setting.h"
#include "UI.h"
#include "../ThirdParty/nlohmann/json.hpp"

bool Client::TitleLevel::BeginPlay()
{
	UIManager* uiManager = SpawnActor(LAYER::OBJECT, "UIManager_Title", UIManager::Create());

	Sound->StopSound(CHANNEL_BGM);
	Sound->PlaySound("Title", CHANNEL_BGM, LOOP_INFINITE);

	nlohmann::json json;

	// JSON 파일을 읽기
	std::ifstream inFile("Assets/StageClear.json");

	// 파일이 없거나 읽을 수 없으면, 기본값으로 새로 작성
	if (!inFile.is_open())
	{
		DebugPrintf("StageClear.json 파일을 찾을 수 없습니다. 기본값으로 초기화합니다.\n");
		json = {
			{"Stage1", false},
			{"Stage2", false},
			{"Stage3", false},
			{"Stage4", false}
		};

		// JSON 파일을 새로 생성하여 기본값으로 작성
		std::ofstream outFile("Assets/StageClear.json");
		if (outFile.is_open())
		{
			outFile << json.dump(4); // pretty print with indentation
			outFile.close();
		}
		else
		{
			DebugPrintf("StageClear.json 파일을 작성할 수 없습니다.\n");
			return false;
		}
	}
	else
	{
		try
		{
			inFile >> json;  // 기존 데이터를 json 객체에 로드
		}
		catch (nlohmann::json::parse_error& e)
		{
			DebugPrintf("StageClear.json 파일을 읽을 수 없습니다. 기본값으로 초기화합니다.\n");
			// 파일이 비어있거나 JSON 형식이 잘못되었으면 기본값으로 초기화
			json = {
				{"Stage1", false},
				{"Stage2", false},
				{"Stage3", false},
				{"Stage4", false}
			};

			// 잘못된 파일을 덮어쓰지 않도록 닫고 다시 엽니다.
			inFile.close();

			std::ofstream outFile("Assets/StageClear.json");
			if (outFile.is_open())
			{
				outFile << json.dump(4);  // pretty print with indentation
				outFile.close();
			}
			else
			{
				DebugPrintf("StageClear.json 파일을 작성할 수 없습니다.\n");
				return false;
			}
		}
	}

	inFile.close();

	// 각 스테이지 클리어 여부 확인
	if (json.contains("Stage1") && json["Stage1"].get<bool>() == true)
	{
		_isContinue = true;
		_lastStage = Client::Stage::Stage1;
	}
	if (json.contains("Stage2") && json["Stage2"].get<bool>() == true)
	{
		_lastStage = Client::Stage::Stage2;
	}
	if (json.contains("Stage3") && json["Stage3"].get<bool>() == true)
	{
		_lastStage = Client::Stage::Stage3;
	}
	if (json.contains("Stage4") && json["Stage4"].get<bool>() == true)
	{
		_lastStage = Client::Stage::Stage4;
	}

	if (_isContinue)
	{
		uiManager->SpawnUI(UI_SCENE::Title_Continue);

		_uiImageList = uiManager->GetImageList();
		_uiButtonList = uiManager->GetButtonList();

		_uiButtonList[0]->AddEnterEvent(this, &TitleLevel::StartGame);
		_uiButtonList[1]->AddEnterEvent(this, &TitleLevel::ContinueGame);
		_uiButtonList[2]->AddEnterEvent(this, &TitleLevel::OpenSetting);
		_uiButtonList[3]->AddEnterEvent(this, &TitleLevel::Credit);
		_uiButtonList[4]->AddEnterEvent(this, &TitleLevel::ExitGame);
	}
	else
	{
		uiManager->SpawnUI(UI_SCENE::Title_Default);

		_uiImageList = uiManager->GetImageList();
		_uiButtonList = uiManager->GetButtonList();
	
		_uiButtonList[0]->AddEnterEvent(this, &TitleLevel::StartGame);
		_uiButtonList[1]->AddEnterEvent(this, &TitleLevel::OpenSetting);
		_uiButtonList[2]->AddEnterEvent(this, &TitleLevel::Credit);
		_uiButtonList[3]->AddEnterEvent(this, &TitleLevel::ExitGame);
	}

	if(!Management->FindDestroyList("Title") && !IsBeginPlay())
	{
		float halfWidth = Management->setting.width * 0.5f;
		float halfHeight = Management->setting.height * 0.5f;
		SpawnActor(LAYER::BACKGROUND, "Title", Title::Create(), Mathf::Vector2{ halfWidth, halfHeight });

		return Level::BeginPlay();
	}
	else if (!IsBeginPlay())
	{
		ReviveActor("Title");

		return Level::BeginPlay();
	}
}

void Client::TitleLevel::Tick(_duration deltaSeconds)
{
	Super::Tick(deltaSeconds);
}

void Client::TitleLevel::EndPlay()
{
	if (Manager->IsSceneChangeTrigger && IsEndPlay())
	{
		RemoveActor("Title");
		for (auto& _uiImageList : _uiImageList)
		{
			_uiImageList->OnCompleteDestroyMark();
		}

		for (auto& _uiButtonList : _uiButtonList)
		{
			_uiButtonList->OnCompleteDestroyMark();
		}

		_uiImageList.clear();
		_uiButtonList.clear();

		TerminateActor("UIManager_Title");
		Management->ScheduleDestroyPoint();
		SetEndPlay(false);
	}
}

void Client::TitleLevel::StartGame()
{
	DebugPrintf("TitleLevel::StartGame");
	for (auto image : _uiImageList) 
	{
		image->OnCompleteDestroyMark();
	}
	for (auto ui : _uiButtonList) 
	{
		ui->OnCompleteDestroyMark();
	}

	Sound->StopSound(CHANNEL_BGM);

	VideoMgr->LoadVideo(L"Assets/Video/Intro.wmv");
	VideoMgr->Play();

	while (true)
	{
		InputMgr->InputUpdate();
		Time->UpdateTick();
		Manager->Tick(Time->GetNonScaleDeltaSeconds());
		VideoMgr->VideoStateUpdate();

		if (Manager->staticInputPress(DIK_ESCAPE) || Manager->staticInputPress(DIP_MENU))
		{
			VideoMgr->Skip();
			break;
		}

		if (VideoMgr->IsEnd())
		{
			VideoMgr->Stop();
			break;
		}
	}

	Manager->ChainLoadingScreen();
}

void Client::TitleLevel::ContinueGame()
{
	DebugPrintf("TitleLevel::ContinueGame");
	Sound->StopSound(CHANNEL_BGM);
	Manager->ChangeStage(_lastStage);
}

void Client::TitleLevel::OpenSetting()
{
	DebugPrintf("TitleLevel::Setting");
	/*_pSetting->SetVisible(true);*/
	Manager->ChangeStage(Stage::Setting);
}

void Client::TitleLevel::Credit()
{
	DebugPrintf("TitleLevel::Credit");
	Manager->ChangeStage(Stage::Credit);
}

void Client::TitleLevel::ExitGame()
{
	DebugPrintf("TitleLevel::ExitGame");
	Management->OnFinalDestroy();
}

