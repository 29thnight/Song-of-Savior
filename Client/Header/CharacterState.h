#pragma once
#include <MainEntry.h>
#include <State.h>

namespace Engine
{
	CORECLASS()
}

namespace Client
{
	class CharacterState : public Engine::State
	{
	public:
		CharacterState() = default;
		virtual ~CharacterState() = default;
	public:
		virtual void Destroy() override {}

	public:
		virtual void SerializeIn(nlohmann::ordered_json& object) {};
		virtual void SerializeOut(nlohmann::ordered_json& object) {};
	};
}

