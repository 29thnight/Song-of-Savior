#include <BitmapComponent.h>
#include <AnimationComponent.h>
#include <BitmapComponent.h>
#include <BoxComponent.h>
#include <TimerComponent.h>
#include <SoundManager.h>
#include <VideoManager.h>
#include <CameraActor.h>
#include <World.h>

#include "Cradit.h"
#include "GameManager.h"

void Client::Credit::BeginPlay()
{
	Super::BeginPlay();

	Sound->StopSound(CHANNEL_BGM);

	_pBackgroundComponent = AddComponent<Engine::BitmapComponent>("CraditBackGround");
	_pBackgroundComponent->SetTextures(&_vecTextures);

	_pTimerComponent = AddComponent<Engine::TimerComponent>("TimerComponent");
	_pTimerComponent->GetTimer(0).onTimeOver.AddDynamic(this, &Credit::ChainCraditIndex);
	_pTimerComponent->SetTime(0, 3.5f, true);
}

void Client::Credit::Tick(_duration deltaSeconds)
{
	if(VideoMgr->IsEnd() || isBgmPlay)
	{
		Super::Tick(deltaSeconds);
		Sound->PlaySound("Credit", CHANNEL_BGM, NOT_LOOP);
		isBgmPlay = false;
	}
}

void Client::Credit::Fixed()
{
}

void Client::Credit::EndPlay()
{
}

void Client::Credit::ChainCraditIndex()
{
	static int index{};
	if (index < _vecTextures[0]->size())
	{
		if(0 != index)
		{
			GetWorld()->GetCameraActor()->GetCameraComponent()->SetColor(Color::Black);
			GetWorld()->GetCameraActor()->FadeIn(0.5f);
		}
		_pBackgroundComponent->SetBitmapIndex(index++);
	}
	else
	{
		index = 0;
		Sound->StopSound(CHANNEL_BGM);
		Manager->ChangeStage(Stage::Title);
	}
}

void Client::Credit::Destroy()
{
	Super::Destroy();
}
