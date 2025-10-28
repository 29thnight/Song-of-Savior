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
		//Ű ���� â ����
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
		void SetVisible(bool visible);	//������ ���̱� ,�����
		bool GetVisible() const { return isKeySettingVisible; }		 //����â ���� ����
		void SetSetting(Setting* setting) { _pSetting = setting; }

	public:
		static KeySetting* Create() { return new KeySetting; }
		void Destroy() override;

		void SetATypeKeyMap();	//A Type ����
		void SetBTypeKeyMap();	//B Type ����
		void SetPadTypeKeyMap();	//�е� Type ����

		std::vector<UI*> _uiButtonList;
		std::vector<UI*> _uiImageList;

	private:
		//UIKeySettingWindow   //Ű ����â UIWindow
		bool isKeySettingVisible{ false };	//Ű ����â ���� ����
		Setting* _pSetting{ nullptr };
	};
}