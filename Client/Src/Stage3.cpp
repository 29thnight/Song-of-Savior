#include <World.h>
#include <CoreManager.h>

#include "Stage3.h"
#include "GameManager.h"

bool Client::Stage3Level::BeginPlay()
{
	Manager->SpawnStageActor(this);

	return Super::BeginPlay();
}

void Client::Stage3Level::EndPlay()
{
	Manager->ReadyToTransitionStage(this);
}
