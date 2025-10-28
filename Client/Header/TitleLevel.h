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
		// Level을(를) 통해 상속됨
		bool BeginPlay() override;
		void Tick(_duration deltaSeconds) override;
		void EndPlay() override;

	public:
		static TitleLevel* Create() { return new TitleLevel; }
	public:
		void StartGame(); //게임 시작
		void ContinueGame(); //게임 이어하기
		void OpenSetting(); //설정창 열기
		void Credit(); //크레딧 열기
		void ExitGame(); //게임 종료


	private:
		std::vector<UI*> _uiButtonList;
		std::vector<UI*> _uiImageList;
		Setting* _pSetting{ nullptr };
		Stage _lastStage{ Stage::Title };
		bool _isContinue{ false };
	};
}