#pragma once
#include <Level.h>

namespace Client
{
	class UI;
	class GameOverLevel : public Engine::Level
	{
	private:
		GameOverLevel() = default;
		virtual ~GameOverLevel() = default;

	public:
		// Level을(를) 통해 상속됨
		bool BeginPlay() override;
		void EndPlay() override;

	public:
		static GameOverLevel* Create() { return new GameOverLevel; }
	public:
		void RestartStage(); //스테이지 재시작
		void GoToTitle(); //타이틀로 이동

		void SetStage(Client::Stage stage) { _PriviousStage = stage; }

	private:
		std::vector<UI*> _uiButtonList;
		std::vector<UI*> _uiImageList;
		Client::Stage _PriviousStage{ Client::Stage::Title };
	};
}