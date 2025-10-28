#include "TestActor.h"
#include <AnimationComponent.h>
#include <InputComponent.h>
#include <SoundManager.h>

void Client::TestActor::BeginPlay()
{
	_pAnimationComponent = AddComponent<Engine::AnimationComponent>("AnimationComponent");
	_pAnimationComponent->SetPlayClip("Idle");
	Super::BeginPlay();

	//_pBitmapComponent = AddComponent<Engine::BitmapComponent>("BitmapComponent");
	//_pBitmapComponent->SetTextures(&_vecTextures);
	//_pBitmapComponent->SetAlphaMaskEffect(true);
	//_pBitmapComponent->SetAlphaMaskOpacity(0.5f);

	_pInputComponent = AddComponent<Engine::InputComponent>("InputComponent");
}

void Client::TestActor::Tick(_duration deltaSeconds)
{
	Super::Tick(deltaSeconds);
}

void Client::TestActor::Fixed()
{
}

void Client::TestActor::EndPlay()
{
}

void Client::TestActor::SetupInputComponent(Engine::InputComponent* inputComponent)
{
	inputComponent->BindKeys(DIK_SPACE, DIK_LEFT, DIK_RIGHT);
	inputComponent->AttachInputManager();
}

void Client::TestActor::SerializeIn(nlohmann::ordered_json& object)
{
}

void Client::TestActor::SerializeOut(nlohmann::ordered_json& object)
{
}

Client::TestActor* Client::TestActor::Create()
{
    return new TestActor;
}

void Client::TestActor::Destroy()
{
	Super::Destroy();
}

