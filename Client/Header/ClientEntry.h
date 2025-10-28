#pragma once
#include <MainEntry.h>

namespace Client
{
	class ClientEntry : public Engine::MainEntry
	{
	public:
		ClientEntry() = default;
		virtual ~ClientEntry() = default;

	public:
		// MainEntry을(를) 통해 상속됨
		void CreateWorld() override;
	};
}