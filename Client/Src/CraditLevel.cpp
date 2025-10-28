#include <CoreManager.h>
#include <World.h>
#include <CameraActor.h>
#include <SoundManager.h>
#include <VideoManager.h>
#include <TimeManager.h>
#include <InputManager.h>
#include <GraphicsManager.h>

#include "CraditLevel.h"
#include "GameManager.h"
#include "ClientEnum.h"
#include "Cradit.h"

bool Client::CraditLevel::BeginPlay()
{
	_pWorld->GetCameraActor()->FadeEnd();
	_pCradit = SpawnActor(LAYER::BACKGROUND, "Credit", Credit::Create(), Mathf::Vector2{ 960.f, 540.f });

	if (isPlayEnding)
	{
		_pCradit->SetBgmPlay(false);
	}
	else
	{
		_pCradit->SetBgmPlay(true);
	}

	return Level::BeginPlay();
}

void Client::CraditLevel::Tick(_duration deltaSeconds)
{
	Super::Tick(deltaSeconds);

	if (isPlayEnding)
	{
		VideoMgr->LoadVideo(L"Assets/Video/ending.wmv");
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

				isPlayEnding = false;
				break;
			}

			if (VideoMgr->IsEnd())
			{
				VideoMgr->Stop();
				isPlayEnding = false;
				break;
			}
		}

		_pWorld->GetCameraActor()->FadeOutIn(1.5f, Color::White);
	}
	else if (Manager->staticInputPress(DIK_ESCAPE) || Manager->staticInputPress(DIP_MENU))
	{
		Manager->ChangeStage(Stage::Title);
		TerminateActor("Credit");
		Management->ScheduleDestroyPoint();
	}
}

void Client::CraditLevel::EndPlay()
{
	if (Manager->IsSceneChangeTrigger && IsEndPlay())
	{
		RemoveActor("Credit");
		Management->ScheduleDestroyPoint();
		SetEndPlay(false);
	}
}
