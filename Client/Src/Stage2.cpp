#include <World.h>
#include <CoreManager.h>

#include "Stage2.h"
#include "GameManager.h"

bool Client::Stage2Level::BeginPlay()
{
	Manager->SpawnStageActor(this);
	//Manager->InitializeStage();
	//Manager->BeginPlay();
	return Super::BeginPlay();
}

void Client::Stage2Level::EndPlay()
{
	Manager->ReadyToTransitionStage(this);
}
