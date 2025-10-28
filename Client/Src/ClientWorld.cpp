#include <ClientWorld.h>
#include <TimeManager.h>
#include <SoundManager.h>
#include <Level.h>
#include <FontManager.h>

#include "TitleLevel.h"
#include "GameManager.h"
#include "PrototypeEditerLevel.h"
#include "Stage1.h"
#include "Stage2.h"
#include "Stage3.h"
#include "DialogueLevel.h"
#include "LoadingLevel.h"
#include "GameOverLevel.h"
#include "SettingLevel.h"
#include "CraditLevel.h"

bool Client::ClientWorld::BeginPlay()
{
	AddLevel<TitleLevel>()->BeginPlay();
	ContainLevel<PrototypeEditorLevel>();
	ContainLevel<Stage1Level>();// ºó·¹º§ ³Ö¾î¾ß ÇÏ´Âµ¥ ±ÍÂú¾Æ...
	ContainLevel<Stage1Level>();
	ContainLevel<Stage2Level>();
	ContainLevel<Stage3Level>();
	ContainLevel<DialogueLevel>();
	ContainLevel<Stage3Level>();
	ContainLevel<CraditLevel>();
	ContainLevel<LoadingLevel>();
	ContainLevel<SettingLevel>();
	ContainLevel<GameOverLevel>();

	SettingCamera();

	Manager->SetWorld(this);
	Manager->InitializeGameManagerComponent();

	FontMgr->LoadFonts();

	return isBeginPlayEnd;
}

void Client::ClientWorld::Tick(_duration deltaSeconds)
{
	Super::Tick(deltaSeconds);
	Manager->Tick(deltaSeconds);
}

Client::ClientWorld* Client::ClientWorld::Create()
{
	return new ClientWorld();
}

void Client::ClientWorld::Destroy()
{
	Super::Destroy();
	GameTerminate();
}
