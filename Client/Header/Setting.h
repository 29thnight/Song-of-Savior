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
		//설정 창 엑터
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
		void SetVisible(bool visible);	//설정차 보이기 ,숨기기
		bool GetVisible() const;		 //설정창 열림 상태
		
		//todo :
		//void SetUI(std::vector<UI*> uiButtonList, std::vector<UI*> uiImageList);
		void SetUIFrame(Client::UI_SCENE uiFrame);	//설정창 UI 프레임 설정
		void ChangeKeySetting();		//키 설정창으로 이동
		void ReturnSetting();			//설정창으로 이동
		void CloseSetting();			//설정창 닫기
	public:
		static Setting* Create() { return new Setting; }
		void Destroy() override;


	public:
		virtual void SetupInputComponent(Engine::InputComponent* inputComponent) override {};

	private:

		//todo :
		//UISettingWindow   //설정창 UIWindow
		bool isSettingVisible{ false };	//설정창 열림 상태
		DefaultSetting* _pDefautSetting{ nullptr };
		KeySetting* _pKeySetting{ nullptr };
		Client::UI_SCENE _currUIFrame{ UI_SCENE::Setting_UI };
	};
}