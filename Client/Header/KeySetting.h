#pragma once
#include <MainEntry.h>
#include <Actor.h>

namespace Engine
{
	CORECLASS()
}

namespace Client
{
	class Setting;
	class KeySetting : public Engine::Actor {
		//키 설정 창 엑터
		using Base = Engine::Actor;
	private:
		KeySetting() = default;
		virtual ~KeySetting() = default;

	public:
		virtual void BeginPlay() override;
		virtual void Tick(_duration deltaSeconds) override;
		virtual void Fixed() override;
		virtual void EndPlay() override;

	public:
		void SetVisible(bool visible);	//설정차 보이기 ,숨기기
		bool GetVisible() const { return isKeySettingVisible; }		 //설정창 열림 상태
		void SetSetting(Setting* setting) { _pSetting = setting; }

	public:
		static KeySetting* Create() { return new KeySetting; }
		void Destroy() override;

		void SetATypeKeyMap();	//A Type 맵핑
		void SetBTypeKeyMap();	//B Type 맵핑
		void SetPadTypeKeyMap();	//패드 Type 맵핑

		std::vector<UI*> _uiButtonList;
		std::vector<UI*> _uiImageList;

	private:
		//UIKeySettingWindow   //키 설정창 UIWindow
		bool isKeySettingVisible{ false };	//키 설정창 열림 상태
		Setting* _pSetting{ nullptr };
	};
}