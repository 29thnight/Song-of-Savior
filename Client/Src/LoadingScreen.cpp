#include "LoadingScreen.h"

#include <BasicAnimationComponent.h>
#include <TextRenderComponent.h>
#include <TimerComponent.h>

void Client::LoadingScreen::BeginPlay()
{
	Super::BeginPlay();

	_pLoadingScreen = AddComponent<Engine::BasicAnimationComponent>("LoadingScreen");
	_pLoadingScreen->SetRelativeScale({ 0.4f, 0.4f });
	int randomFactor = _random.Generate();
	if(0 == randomFactor)
	{
		_pLoadingScreen->SetRelativeLocation(Mathf::Vector2(1215.f, 925.f));
		_pLoadingScreen->AllAddClipThisActor("Assets/Bard/Clips.csv", "Bard");
	}
	else if (1 == randomFactor)
	{
		_pLoadingScreen->SetRelativeLocation(Mathf::Vector2(1180.f, 910.f));
		_pLoadingScreen->AllAddClipThisActor("Assets/Warrior/Clips.csv", "Warrior");
	}
	else if (2 == randomFactor)
	{
		_pLoadingScreen->SetRelativeLocation(Mathf::Vector2(1235.f, 915.f));
		_pLoadingScreen->AllAddClipThisActor("Assets/Thief/Clips.csv", "Thief");
	}
	else if (3 == randomFactor)
	{
		_pLoadingScreen->SetRelativeLocation(Mathf::Vector2(1220.f, 930.f));
		_pLoadingScreen->AllAddClipThisActor("Assets/Sorcerer/Clips.csv", "Sorcerer");
	}
	_pLoadingScreen->SetPlayClip("Idle");

	_pLoadingText = AddComponent<Engine::TextRenderComponent>("LoadingText");
	_pLoadingText->SetRelativeLocation(Mathf::Vector2(1600.f, 1000.f));
	_pLoadingText->SetFont(L"MapoGoldenPier");
	_pLoadingText->SetHorizontalAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
	_pLoadingText->SetVerticalAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
	_pLoadingText->SetFontSize(50.f);
	_pLoadingText->SetSize(Mathf::Vector2(500.f, 200.f));
	_pLoadingText->ChooseBrush("Default");
	_pLoadingText->Text = L"Moving On";
	_pLoadingText->SetColor(Color::White);

	_pTimerComponent = AddComponent<Engine::TimerComponent>("TimerComponent");
	_pTimerComponent->GetTimer(0).onTimeOver.AddDynamic(this, &LoadingScreen::TextChange);
	_pTimerComponent->SetTime(0, 0.5f, true);

}

void Client::LoadingScreen::Tick(_duration deltaSeconds)
{
	Super::Tick(deltaSeconds);
}

void Client::LoadingScreen::Fixed()
{
}

void Client::LoadingScreen::EndPlay()
{
}

void Client::LoadingScreen::ReviveInitialize()
{
}

void Client::LoadingScreen::TextChange()
{
	_textPointCount++;
	if (_textPointCount > 3)
	{
		_textPointCount = 0;
	}
	string text = L"Moving On";
	for (int i = 0; i < _textPointCount; i++)
	{
		text += L".";
	}
	_pLoadingText->Text = text;
}
