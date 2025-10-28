#include <World.h>
#include <CoreManager.h>

#include "Stage1.h"
#include "GameManager.h"

bool Client::Stage1Level::BeginPlay()
{
	Manager->SpawnStageActor(this);
	//Manager->InitializeStage();
	//Manager->BeginPlay();
	return Super::BeginPlay();
}

void Client::Stage1Level::EndPlay()
{
	Manager->ReadyToTransitionStage(this);
}
