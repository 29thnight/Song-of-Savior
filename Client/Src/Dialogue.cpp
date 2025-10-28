#include <BitmapComponent.h>
#include <InputComponent.h>
#include <World.h>
#include <CameraActor.h>
#include <TimerComponent.h>
#include <BasicAnimationComponent.h>

#include "Dialogue.h"
#include "GameManager.h"
#include "ClientEnum.h"

void Client::Dialogue::BeginPlay()
{
	Super::BeginPlay();

	_pDialogueComponent = AddComponent<Engine::BitmapComponent>("Dialogue");
	//_pTextComponent = AddComponent<Engine::TextRenderComponent>("Text");
	_pBossAnimationComponent = AddComponent<Engine::BasicAnimationComponent>("BossAnimation");
	_pChangeBossAnimationComponent = AddComponent<Engine::BasicAnimationComponent>("ChangeBossAnimation");

	_pBossAnimationComponent->AllAddClipThisActor("Assets/Lich/Clips.csv", "Lich");
	_pChangeBossAnimationComponent->AllAddClipThisActor("Assets/Lich/Clips.csv", "Lich");

	_pBossAnimationComponent->SetPlayClip("Idle");
	_pChangeBossAnimationComponent->SetPlayClip("Phase2/Idle");
	_pChangeBossAnimationComponent->SetVisible(false);

	_pBossAnimationComponent->SetRelativeScale({0.5f, 0.5f});
	_pBossAnimationComponent->SetRelativeLocation(Mathf::Vector2(540.f, 100.f));
	_pChangeBossAnimationComponent->SetRelativeScale({ 0.5f, 0.5f });
	_pChangeBossAnimationComponent->SetRelativeLocation(Mathf::Vector2(540.f, 100.f));

	_pDialogueComponent->SetTextures(&_vecTextures);
	_pDialogueComponent->SetVisible(false);

	_pTimerComponent = AddComponent<Engine::TimerComponent>("Timer");
	_pTimerComponent->AddTimer(6);
	_pTimerComponent->GetTimer(0).onTimeOver.AddDynamic(this, &Dialogue::Present);
	_pTimerComponent->GetTimer(1).onTimeOver.AddDynamic(this, &Dialogue::ChainDialogue);
	_pTimerComponent->GetTimer(2).onTimeOver.AddDynamic(this, &Dialogue::ChainCameraShake);
	_pTimerComponent->GetTimer(3).onTimeOver.AddDynamic(this, &Dialogue::ChainWhiteFade);
	_pTimerComponent->GetTimer(4).onTimeOver.AddDynamic(this, &Dialogue::ChangeBossAnimation);
	_pTimerComponent->GetTimer(5).onTimeOver.AddDynamic(this, &Dialogue::EnterPhaseTwo);
	_pTimerComponent->SetTime(0, 1.f);
	_pTimerComponent->SetTime(5, 8.f);
}

void Client::Dialogue::Tick(_duration deltaSeconds)
{
	Super::Tick(deltaSeconds);
}

void Client::Dialogue::Fixed()
{
}

void Client::Dialogue::EndPlay()
{
}

void Client::Dialogue::SetupInputComponent(Engine::InputComponent* inputComponent)
{
	//혹시 모를 기획 변경을 대비한 코드
}

void Client::Dialogue::Korean()
{
}

void Client::Dialogue::English()
{
}

void Client::Dialogue::Present()
{
	constexpr float targetTime = 2.f;
	_pTimerComponent->SetTime(1, targetTime, true);
	_pTimerComponent->SetTime(2, 0.1f);
	_pTimerComponent->SetTime(3, 3.1f);
}

void Client::Dialogue::ChainDialogue()
{
	static int index{};
	if (index < _vecTextures[0]->size())
	{
		if (0 == index)
		{
			_pDialogueComponent->SetVisible(true);
		}
		_pDialogueComponent->SetBitmapIndex(index++);
	}
	else
	{
		_pTimerComponent->StopTime(1);
		_isDialogueEnd = true;
	}
}

void Client::Dialogue::ChainCameraShake()
{
	GetWorld()->GetCameraActor()->CameraShake(3.f, 13.2f);
}

void Client::Dialogue::ChainWhiteFade()
{
	GetWorld()->GetCameraActor()->FadeOutIn(1.f, Color::White);
	_pTimerComponent->SetTime(4, 0.1f);
}

void Client::Dialogue::ChangeBossAnimation()
{
	_pBossAnimationComponent->SetVisible(false);
	_pChangeBossAnimationComponent->SetVisible(true);
}

void Client::Dialogue::EnterPhaseTwo()
{
	Manager->ChangeStage(Stage::Stage4_2);
}

void Client::Dialogue::Destroy()
{
	Super::Destroy();
}
