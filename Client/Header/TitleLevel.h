#pragma once
#include <Level.h>

#include "ClientEnum.h"

namespace Client
{
	class UI;
	class Setting;
	class TitleLevel : public Engine::Level
	{
	private:
		TitleLevel() = default;
		virtual ~TitleLevel() = default;

	public:
		// Level��(��) ���� ��ӵ�
		bool BeginPlay() override;
		void Tick(_duration deltaSeconds) override;
		void EndPlay() override;

	public:
		static TitleLevel* Create() { return new TitleLevel; }
	public:
		void StartGame(); //���� ����
		void ContinueGame(); //���� �̾��ϱ�
		void OpenSetting(); //����â ����
		void Credit(); //ũ���� ����
		void ExitGame(); //���� ����


	private:
		std::vector<UI*> _uiButtonList;
		std::vector<UI*> _uiImageList;
		Setting* _pSetting{ nullptr };
		Stage _lastStage{ Stage::Title };
		bool _isContinue{ false };
	};
}