#include "TimelineBlock.h"
#include <CoreManager.h>
#include <BitmapComponent.h>
#include <TextureManager.h>

void Client::TimelineBlock::BeginPlay()
{
	_pBitmapComponent = AddComponent<Engine::BitmapComponent>("bitmap");
	string convertName = (string)"Assets/TimelineBlock";
	_vecTextures.push_back(TextureMgr->FindTexture(convertName));
	_pBitmapComponent->SetTextures(&_vecTextures);
}

void Client::TimelineBlock::Tick(_duration deltaSeconds)
{
	Base::Tick(deltaSeconds);

}


void Client::TimelineBlock::Fixed()
{

}

void Client::TimelineBlock::EndPlay()
{

}

void Client::TimelineBlock::NotifyActorBlock(Engine::CollisionData data)
{

}

void Client::TimelineBlock::NotifyActorBeginOverlap(Engine::CollisionData data)
{

}

void Client::TimelineBlock::NotifyActorEndOverlap(Engine::CollisionData data)
{

}

void Client::TimelineBlock::SetupInputComponent(Engine::InputComponent* inputComponent)
{

}

void Client::TimelineBlock::SerializeIn(nlohmann::ordered_json& object)
{

}

void Client::TimelineBlock::SerializeOut(nlohmann::ordered_json& object)
{

}
