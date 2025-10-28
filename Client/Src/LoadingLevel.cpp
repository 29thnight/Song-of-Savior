#include <World.h>
#include <CoreManager.h>

#include "LoadingLevel.h"
#include "GameManager.h"
#include "TItle.h"
#include "LoadingScreen.h"

bool Client::LoadingLevel::BeginPlay()
{
	_pLoadingScreen = SpawnActor(LAYER::UI, "LoadingScreen", LoadingScreen::Create());
	return Super::BeginPlay();
}

void Client::LoadingLevel::EndPlay()
{
	if(Manager->IsSceneChangeTrigger && IsEndPlay())
	{
		_pLoadingScreen->OnCompleteDestroyMark();
		SetEndPlay(true);
	}
}
