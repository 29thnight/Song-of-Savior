#pragma once
#include <MainEntry.h>
#include <Actor.h>
#include "UIManager.h"

namespace Engine
{
	CORECLASS()
}

namespace Client
{
	
	class DefaultSetting;
	class KeySetting;
	class Setting : public Engine::Actor {
		//���� â ����
		using Base = Engine::Actor;
	private:
		Setting() = default;
		virtual ~Setting() = default;

	public:
		virtual void BeginPlay() override;
		virtual void Tick(_duration deltaSeconds) override;
		virtual void Fixed() override;
		virtual void EndPlay() override;

	public:
		void SetVisible(bool visible);	//������ ���̱� ,�����
		bool GetVisible() const;		 //����â ���� ����
		
		//todo :
		//void SetUI(std::vector<UI*> uiButtonList, std::vector<UI*> uiImageList);
		void SetUIFrame(Client::UI_SCENE uiFrame);	//����â UI ������ ����
		void ChangeKeySetting();		//Ű ����â���� �̵�
		void ReturnSetting();			//����â���� �̵�
		void CloseSetting();			//����â �ݱ�
	public:
		static Setting* Create() { return new Setting; }
		void Destroy() override;


	public:
		virtual void SetupInputComponent(Engine::InputComponent* inputComponent) override {};

	private:

		//todo :
		//UISettingWindow   //����â UIWindow
		bool isSettingVisible{ false };	//����â ���� ����
		DefaultSetting* _pDefautSetting{ nullptr };
		KeySetting* _pKeySetting{ nullptr };
		Client::UI_SCENE _currUIFrame{ UI_SCENE::Setting_UI };
	};
}