#include <World.h>
#include <CoreManager.h>
#include <SoundManager.h>

#include "DialogueLevel.h"
#include "GameManager.h"
#include "Dialogue.h"
#include "BGMManager.h"


bool Client::DialogueLevel::BeginPlay()
{
	//배치 변경 있을 경우 따로 진행해야함.
	Sound->SetPause(CHANNEL_BGM, false);
	//ReviveActor("BGMManager");
	ReviveActor("Warrior");
	ReviveActor("Thief");
	ReviveActor("Sorcerer");
	ReviveActor("Bard");
	ReviveActor("BackGround");
	_pDialogue = SpawnActor(LAYER::UI, "Dialogue", Dialogue::Create(), Mathf::Vector2{ 960.f, 600.f });

	return Super::BeginPlay();
}

void Client::DialogueLevel::EndPlay()
{
	if (Manager->IsSceneChangeTrigger && IsEndPlay())
	{
		//RemoveActor("BGMManager");
		RemoveActor("Warrior");
		RemoveActor("Thief");
		RemoveActor("Sorcerer");
		RemoveActor("Bard");
		RemoveActor("BackGround");
		TerminateActor("Dialogue");

		//Sound->SetPause(CHANNEL_BGM, false);
		SetEndPlay(false);
	}
}
