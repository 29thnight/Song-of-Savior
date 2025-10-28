#include <BitmapComponent.h>
#include <InputComponent.h>
#include <World.h>
#include <CameraActor.h>
#include <BasicAnimationComponent.h>
#include <VideoManager.h>

#include "GameManager.h"
#include "ClientEnum.h"
#include "Title.h"

void Client::Title::BeginPlay()
{
	Super::BeginPlay();

	_pBackgroundComponent = AddComponent<Engine::BitmapComponent>("TitleBackGround");
	_pBackgroundComponent->SetTextures(&_vecTextures);
}

void Client::Title::Tick(_duration deltaSeconds)
{
	Super::Tick(deltaSeconds);

}

void Client::Title::Fixed()
{
}

void Client::Title::EndPlay()
{
}

void Client::Title::SetupInputComponent(Engine::InputComponent* inputComponent)
{

}

void Client::Title::Destroy()
{
	Super::Destroy();
}
