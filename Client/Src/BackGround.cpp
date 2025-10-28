#include <BitmapComponent.h>
#include <AnimationComponent.h>
#include <BitmapComponent.h>
#include <BoxComponent.h>
#include <TimerComponent.h>

#include "BackGround.h"
#include "GameManager.h"


void Client::BackGround::BeginPlay()
{
	Super::BeginPlay();
	
	_pPrimaryBitmapComponent = AddComponent<Engine::BitmapComponent>("frontBitmap");
	_pPrimaryBitmapComponent->SetTextures(&_vecTextures);
	currentStage = Manager->GetCurrentStage();
	_pTimerComponent = AddComponent<Engine::TimerComponent>("TimerComponent");
	_pTimerComponent->GetTimer(0).onTimeOver.AddDynamic(this, &BackGround::ChangeDarkBackground);

	switch (currentStage)
	{
	case Stage::Editor:
	case Stage::Stage1:
	case Stage::Stage2:
	case Stage::Setting:
		_pPrimaryBitmapComponent->SetBitmapIndex(0);
		_pTimerComponent->SetTime(0, 1.f);
		break;
	case Stage::Stage3:
		_pPrimaryBitmapComponent->SetBitmapIndex(1);
		break;
	case Stage::Stage4:
		_pPrimaryBitmapComponent->SetBitmapIndex(2);
		break;
	case Stage::Dialouge:
	case Stage::Stage4_2:
		_pPrimaryBitmapComponent->SetBitmapIndex(3);
		break;
	case Stage::GameOver:
		_pPrimaryBitmapComponent->SetBitmapIndex(8);
		break;
	};

	_pSecondryBitmapComponent = AddComponent<Engine::BitmapComponent>("backBitmap");
	_pSecondryBitmapComponent->SetTextures(&_vecTextures);
	_pSecondryBitmapComponent->SetBitmapIndex(4);
	_pSecondryBitmapComponent->SetOpacity(0.f);

}

void Client::BackGround::Tick(_duration deltaSeconds)
{
	Super::Tick(deltaSeconds);
}

void Client::BackGround::Fixed()
{
}

void Client::BackGround::EndPlay()
{

}

void Client::BackGround::ReviveInitialize()
{
	currentStage = Manager->GetCurrentStage();
	_pSecondryBitmapComponent->SetVisible(false);

	switch (currentStage)
	{
	case Stage::Editor:
	case Stage::Stage1:
	case Stage::Stage2:
		_pPrimaryBitmapComponent->SetBitmapIndex(0);
		_pTimerComponent->SetTime(0, 1.f);
		break;
	case Stage::Stage3:
		_pPrimaryBitmapComponent->SetBitmapIndex(1);
		_pTimerComponent->SetTime(0, 1.f);
		break;
	case Stage::Stage4:
		_pPrimaryBitmapComponent->SetBitmapIndex(2);
		_pTimerComponent->SetTime(0, 1.f);
		break;
	case Stage::Dialouge:
	case Stage::Stage4_2:
		_pPrimaryBitmapComponent->SetBitmapIndex(3);
		break;
	case Stage::GameOver:
		_pPrimaryBitmapComponent->SetBitmapIndex(8);
		break;
	default:
		break;
	};
}

void Client::BackGround::SetStageBackground(Stage stage)
{
	switch (currentStage)
	{
	case Stage::Editor:
	case Stage::Stage1:
	case Stage::Stage2:
		_pPrimaryBitmapComponent->SetBitmapIndex(0);
		_pTimerComponent->SetTime(0, 1.f);
		break;
	case Stage::Stage3:
		_pPrimaryBitmapComponent->SetBitmapIndex(1);
		_pTimerComponent->SetTime(0, 1.f);
		break;
	case Stage::Stage4:
		_pPrimaryBitmapComponent->SetBitmapIndex(2);
		_pTimerComponent->SetTime(0, 1.f);
		break;
	case Stage::Dialouge:
	case Stage::Stage4_2:
		_pPrimaryBitmapComponent->SetBitmapIndex(3);
		break;
	default:
		break;
	};
}

void Client::BackGround::ChangeDarkBackground()
{
	currentStage = Manager->GetCurrentStage();
	_pSecondryBitmapComponent->SetVisible(true);

	switch (currentStage)
	{
	case Stage::Editor:
	case Stage::Stage1:
	case Stage::Stage2:
		_pSecondryBitmapComponent->SetBitmapIndex(4);
		_pSecondryBitmapComponent->StartReveal(1.f);
		break;
	case Stage::Stage3:
		_pSecondryBitmapComponent->SetBitmapIndex(5);
		_pSecondryBitmapComponent->StartReveal(1.f);
		break;
	case Stage::Stage4:
		_pSecondryBitmapComponent->SetBitmapIndex(6);
		_pSecondryBitmapComponent->StartReveal(1.f);
		break;
	case Stage::Dialouge:
		break;
	case Stage::Stage4_2:
		_pPrimaryBitmapComponent->SetBitmapIndex(7);
		break;
	default:
		break;
	};
}
