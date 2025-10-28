#pragma once
#include <Level.h>

namespace Client
{
	class Credit;
	class CraditLevel : public Engine::Level
	{
	private:
		CraditLevel() = default;
		virtual ~CraditLevel() = default;

	public:
		// Level을(를) 통해 상속됨
		bool BeginPlay() override;
		void Tick(_duration deltaSeconds) override;
		void EndPlay() override;
		void SetPlayEnding(bool isPlayEnding) { this->isPlayEnding = isPlayEnding; }

	public:
		static CraditLevel* Create() { return new CraditLevel; }

	private:
		Credit* _pCradit{ nullptr };

	private:
		bool isPlayEnding{ false };
	};
}