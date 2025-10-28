#include "TimelineCurrNote.h"
#include <BitmapComponent.h>
#include <TextureManager.h>

void Client::TimelineCurrNote::BeginPlay()
{
	_pBitmapComponent = AddComponent<Engine::BitmapComponent>("bitmap");
	string convertName = (string)"Assets/TimelineCurrNote";
	_vecTextures.push_back(TextureMgr->FindTexture(convertName));
	_pBitmapComponent->SetTextures(&_vecTextures);

}

void Client::TimelineCurrNote::Tick(_duration deltaSeconds)
{
	Base::Tick(deltaSeconds);

}

void Client::TimelineCurrNote::Fixed()
{
}

void Client::TimelineCurrNote::EndPlay()
{
}

void Client::TimelineCurrNote::NotifyActorBlock(Engine::CollisionData data)
{
}

void Client::TimelineCurrNote::NotifyActorBeginOverlap(Engine::CollisionData data)
{
}

void Client::TimelineCurrNote::NotifyActorEndOverlap(Engine::CollisionData data)
{
}

void Client::TimelineCurrNote::SetupInputComponent(Engine::InputComponent* inputComponent)
{
}

void Client::TimelineCurrNote::SerializeIn(nlohmann::ordered_json& object)
{
}

void Client::TimelineCurrNote::SerializeOut(nlohmann::ordered_json& object)
{
}
