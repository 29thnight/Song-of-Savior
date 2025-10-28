#pragma once
#include <Level.h>

namespace Engine
{
	CORECLASS()
	class JsonLevel : public Level
	{
	private:
		JsonLevel() = default;
		virtual ~JsonLevel() = default;

	public:
		bool BeginPlay() override { return true; }

	public:
		static JsonLevel* Create() { return new JsonLevel; }

	private:
	};
}

