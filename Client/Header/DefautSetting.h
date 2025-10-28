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
		//키 설정 창 엑터
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
		void SetVisible(bool visible);	//설정차 보이기 ,숨기기
		bool GetVisible() const;		 //설정창 열림 상태
		void SetSetting(Setting* setting) { _pSetting = setting; }

	public:
		static DefaultSetting* Create();
		void Destroy() override;

		void SetUIFrame(UI_SCENE uiFrame);	//설정창 UI 프레임 설정
		void SetUI(std::vector<UI*> uiButtonList, std::vector<UI*> uiImageList);
		void SetUiButtonList(std::vector<UI*> uiButtonList);
		void SetUiImageList(std::vector<UI*> uiImageList);

		void VolumeUp();					//볼륨 업
		void VolumeDown();					//볼륨 다운
		//void SetNoteOffset(float offset);	//노트 오프셋 설정 //폐기
		void BindKeyMap();	//조작키 맵핑
			//UIButtonReStart   //재시작 버튼
		void ReStartStage();			//스테이지 재시작
		//UIButtonGoTitle   //타이틀로 이동 버튼
		void GoTitle();					//타이틀로 이동

		std::vector<UI*> _uiButtonList;
		std::vector<UI*> _uiImageList;

		Engine::InputComponent* _pInputComponent{ nullptr };
		Engine::TimerComponent* _pTimerComponent{ nullptr };

		SoundBar* _pSoundBar{ nullptr };

		virtual void SetupInputComponent(Engine::InputComponent* inputComponent) override;
	private:
		//UIKeySettingWindow   //키 설정창 UIWindow
		bool isDefautSettingVisible{ false };	//키 설정창 열림 상태
		Stage _CurrStage{ Client::Stage::Title };
		UI_SCENE _currUIFrame{ UI_SCENE::Setting_UI };

		Setting* _pSetting{ nullptr };

		bool _gamePadLeft = false;
		bool _gamePadRight = false;
	};

}

