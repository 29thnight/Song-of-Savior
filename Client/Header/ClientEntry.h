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
		// MainEntry��(��) ���� ��ӵ�
		void CreateWorld() override;
	};
}