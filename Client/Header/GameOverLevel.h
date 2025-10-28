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
		// Level��(��) ���� ��ӵ�
		bool BeginPlay() override;
		void EndPlay() override;

	public:
		static GameOverLevel* Create() { return new GameOverLevel; }
	public:
		void RestartStage(); //�������� �����
		void GoToTitle(); //Ÿ��Ʋ�� �̵�

		void SetStage(Client::Stage stage) { _PriviousStage = stage; }

	private:
		std::vector<UI*> _uiButtonList;
		std::vector<UI*> _uiImageList;
		Client::Stage _PriviousStage{ Client::Stage::Title };
	};
}