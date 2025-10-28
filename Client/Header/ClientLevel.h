#pragma once
#include <Level.h>

namespace Client
{
	class ClientLevel : public Engine::Level
	{
	private:
		ClientLevel() = default;
		virtual ~ClientLevel() = default;

	public:
		// Level을(를) 통해 상속됨
		bool BeginPlay() override;

	public:
		static ClientLevel* Create() { return new ClientLevel; }

	private:

	};
};