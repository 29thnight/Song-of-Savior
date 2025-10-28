#include <PrototypeEditerWorld.h>
#include <Level.h>
#include <PrototypeEditerLevel.h>

#include "GameManager.h"
#include "TitleLevel.h"

bool Client::PrototypeEditorWorld::BeginPlay()
{
	AddLevel<TitleLevel>()->BeginPlay();
	//AddLevel<PrototypeEditorLevel>()/*->BeginPlay()*/;
	ContainLevel<PrototypeEditorLevel>();
	SettingCamera();

	Manager->SetWorld(this);

	return isBeginPlayEnd;
}

Client::PrototypeEditorWorld* Client::PrototypeEditorWorld::Create()
{
	return new PrototypeEditorWorld();
}

void Client::PrototypeEditorWorld::Destroy()
{
	Super::Destroy();
}
