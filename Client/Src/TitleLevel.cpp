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

	// JSON ������ �б�
	std::ifstream inFile("Assets/StageClear.json");

	// ������ ���ų� ���� �� ������, �⺻������ ���� �ۼ�
	if (!inFile.is_open())
	{
		DebugPrintf("StageClear.json ������ ã�� �� �����ϴ�. �⺻������ �ʱ�ȭ�մϴ�.\n");
		json = {
			{"Stage1", false},
			{"Stage2", false},
			{"Stage3", false},
			{"Stage4", false}
		};

		// JSON ������ ���� �����Ͽ� �⺻������ �ۼ�
		std::ofstream outFile("Assets/StageClear.json");
		if (outFile.is_open())
		{
			outFile << json.dump(4); // pretty print with indentation
			outFile.close();
		}
		else
		{
			DebugPrintf("StageClear.json ������ �ۼ��� �� �����ϴ�.\n");
			return false;
		}
	}
	else
	{
		try
		{
			inFile >> json;  // ���� �����͸� json ��ü�� �ε�
		}
		catch (nlohmann::json::parse_error& e)
		{
			DebugPrintf("StageClear.json ������ ���� �� �����ϴ�. �⺻������ �ʱ�ȭ�մϴ�.\n");
			// ������ ����ְų� JSON ������ �߸��Ǿ����� �⺻������ �ʱ�ȭ
			json = {
				{"Stage1", false},
				{"Stage2", false},
				{"Stage3", false},
				{"Stage4", false}
			};

			// �߸��� ������ ����� �ʵ��� �ݰ� �ٽ� ���ϴ�.
			inFile.close();

			std::ofstream outFile("Assets/StageClear.json");
			if (outFile.is_open())
			{
				outFile << json.dump(4);  // pretty print with indentation
				outFile.close();
			}
			else
			{
				DebugPrintf("StageClear.json ������ �ۼ��� �� �����ϴ�.\n");
				return false;
			}
		}
	}

	inFile.close();

	// �� �������� Ŭ���� ���� Ȯ��
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

