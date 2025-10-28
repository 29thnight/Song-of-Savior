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
		// Level��(��) ���� ��ӵ�
		bool BeginPlay() override;

	public:
		static ClientLevel* Create() { return new ClientLevel; }

	private:

	};
};