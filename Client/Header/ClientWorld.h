#pragma once
#include <World.h>

namespace Client
{
	class ClientWorld : public Engine::World
	{
	private:
		ClientWorld() = default;
		virtual ~ClientWorld() = default;

	public:
		bool BeginPlay() override;
		void Tick(_duration deltaSeconds) override;

	public:
		static ClientWorld* Create();
		void Destroy() override;
	};
}