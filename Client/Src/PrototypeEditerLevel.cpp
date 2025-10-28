#include <World.h>
#include <CoreManager.h>

#include "PrototypeEditerLevel.h"
#include "NoteManager.h"
#include "NoteEditer.h"
#include "BGMManager.h"	
#include "Warrior.h"
#include "Thief.h"
#include "Sorcerer.h"
#include "Bard.h"

#include "BossManager.h"
#include "GameManager.h"
#include "UI.h"
#include "UiIconSkill.h"
#include "BackGround.h"
#include "UIManager.h"

using Vector2 = Mathf::Vector2;

bool Client::PrototypeEditorLevel::BeginPlay()
{
	if (!Management->FindDestroyList("NoteManager") && !IsBeginPlay())
	{
		SpawnActor(LAYER::OBJECT, "NoteManager", NoteManager::Create(), Vector2(300.f, 300.f));
		SpawnActor(LAYER::OBJECT, "BGMManager", BGMManager::Create(), Vector2(500.f, 500.f));
		SpawnActor(LAYER::OBJECT, "BossManager", BossManager::Create(), Vector2(500.f, 500.f));
		SpawnActor(LAYER::OBJECT, "NoteEditor", NoteEditor::Create()); //에디터는 Manager들 생성 이후 가장 마지막에 생성

		SpawnActor(LAYER::OBJECT, "Warrior", Warrior::Create(), Mathf::Vector2{ 960.f, 710.f });
		SpawnActor(LAYER::OBJECT, "Thief", Thief::Create(), Mathf::Vector2{ 750.f, 600.f });
		SpawnActor(LAYER::OBJECT, "Sorcerer", Sorcerer::Create(), Mathf::Vector2{ 500.f, 770.f });
		SpawnActor(LAYER::OBJECT, "Bard", Bard::Create(), Mathf::Vector2{ 280.f, 700.f });

		SpawnActor(LAYER::BACKGROUND, "BackGround", BackGround::Create(), Mathf::Vector2{ 960.f, 540.f });
		
		//UI Skill Icon
		SpawnActor(LAYER::UI, "BardSkillIcon", UiIconSkill::Create(), Mathf::Vector2{ 1319.f, 910.f });
		SpawnActor(LAYER::UI, "ThiefSkillIcon", UiIconSkill::Create(), Mathf::Vector2{ 1453.f, 910.f });
		SpawnActor(LAYER::UI, "SorcererSkillIcon", UiIconSkill::Create(), Mathf::Vector2{ 1588.f, 910.f });
		SpawnActor(LAYER::UI, "WarriorSkillIcon", UiIconSkill::Create(), Mathf::Vector2{ 1727.f, 910.f });

		Manager->InitializeStage();
		Manager->BeginPlay();

		
	}
	else if(!IsBeginPlay())
	{
		ReviveActor("NoteManager");
		ReviveActor("BGMManager");
		ReviveActor("BossManager");
		SpawnActor(LAYER::OBJECT, "NoteEditor", NoteEditor::Create()); //에디터는 Manager들 생성 이후 가장 마지막에 생성

		Manager->ReviveAllCharacter();
		Manager->BeginPlay();

		ReviveActor("BackGround");
		//ReviveActor("UI");

	}

	return Super::BeginPlay();
}

void Client::PrototypeEditorLevel::EndPlay()
{
	Manager->ReadyToTransitionStage(this);
}
