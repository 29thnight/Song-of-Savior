#include "CurrTimeBlock.h"
#include <BitmapComponent.h>
#include <World.h>

#include "BGMManager.h"

void Client::CurrTimeBlock::BeginPlay()
{
	Base::BeginPlay();
	_pBitmapComponent = AddComponent<Engine::BitmapComponent>("BitmapComponent");
	_pBitmapComponent->SetTextures(&_vecTextures);
}

void Client::CurrTimeBlock::Tick(_duration deltaSeconds)
{
	Base::Tick(deltaSeconds);


}

void Client::CurrTimeBlock::Fixed()
{

}

void Client::CurrTimeBlock::EndPlay()
{

}

void Client::CurrTimeBlock::NotifyActorBlock(Engine::CollisionData data)
{

}

void Client::CurrTimeBlock::NotifyActorBeginOverlap(Engine::CollisionData data)
{

}

void Client::CurrTimeBlock::NotifyActorEndOverlap(Engine::CollisionData data)
{

}

void Client::CurrTimeBlock::SetupInputComponent(Engine::InputComponent* inputComponent)
{

}

void Client::CurrTimeBlock::SerializeIn(nlohmann::ordered_json& object)
{

}

void Client::CurrTimeBlock::SerializeOut(nlohmann::ordered_json& object)
{

}
