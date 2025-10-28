#pragma once
#include <MainEntry.h>
#include <Actor.h>

namespace Engine
{
	CORECLASS()
}

namespace Client
{
	class SoundBar;
	class Setting;
	class DefaultSetting : public Engine::Actor {
		//Ű ���� â ����
		using Base = Engine::Actor;
	private:
		DefaultSetting() = default;
		virtual ~DefaultSetting() = default;

	public:
		virtual void BeginPlay() override;
		virtual void Tick(_duration deltaSeconds) override;
		virtual void Fixed() override;
		virtual void EndPlay() override;

	public:
		void SetVisible(bool visible);	//������ ���̱� ,�����
		bool GetVisible() const;		 //����â ���� ����
		void SetSetting(Setting* setting) { _pSetting = setting; }

	public:
		static DefaultSetting* Create();
		void Destroy() override;

		void SetUIFrame(UI_SCENE uiFrame);	//����â UI ������ ����
		void SetUI(std::vector<UI*> uiButtonList, std::vector<UI*> uiImageList);
		void SetUiButtonList(std::vector<UI*> uiButtonList);
		void SetUiImageList(std::vector<UI*> uiImageList);

		void VolumeUp();					//���� ��
		void VolumeDown();					//���� �ٿ�
		//void SetNoteOffset(float offset);	//��Ʈ ������ ���� //���
		void BindKeyMap();	//����Ű ����
			//UIButtonReStart   //����� ��ư
		void ReStartStage();			//�������� �����
		//UIButtonGoTitle   //Ÿ��Ʋ�� �̵� ��ư
		void GoTitle();					//Ÿ��Ʋ�� �̵�

		std::vector<UI*> _uiButtonList;
		std::vector<UI*> _uiImageList;

		Engine::InputComponent* _pInputComponent{ nullptr };
		Engine::TimerComponent* _pTimerComponent{ nullptr };

		SoundBar* _pSoundBar{ nullptr };

		virtual void SetupInputComponent(Engine::InputComponent* inputComponent) override;
	private:
		//UIKeySettingWindow   //Ű ����â UIWindow
		bool isDefautSettingVisible{ false };	//Ű ����â ���� ����
		Stage _CurrStage{ Client::Stage::Title };
		UI_SCENE _currUIFrame{ UI_SCENE::Setting_UI };

		Setting* _pSetting{ nullptr };

		bool _gamePadLeft = false;
		bool _gamePadRight = false;
	};

}

