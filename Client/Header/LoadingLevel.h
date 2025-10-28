#pragma once
#include <Level.h>

namespace Client
{
	class LoadingScreen;
	class LoadingLevel : public Engine::Level
	{
	private:
		LoadingLevel() = default;
		virtual ~LoadingLevel() = default;

	public:
		// Level��(��) ���� ��ӵ�
		bool BeginPlay() override;
		void EndPlay() override;

	public:
		static LoadingLevel* Create() { return new LoadingLevel; }

	private:
		LoadingScreen* _pLoadingScreen{ nullptr };
	};
}