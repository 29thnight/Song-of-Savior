#pragma once
#include <Level.h>

namespace Client
{
	class UI;
	class Setting;
	class SoundBar;
	class BackGround;
	class SettingLevel : public Engine::Level
	{
	private:
		SettingLevel() = default;
		virtual ~SettingLevel() = default;

	public:
		// Level을(를) 통해 상속됨
		bool BeginPlay() override;
		void EndPlay() override;

	public:
		static SettingLevel* Create() { return new SettingLevel; }
	public:

	private:
		std::vector<UI*> _uiButtonList;
		std::vector<UI*> _uiImageList;
		Setting* _pSetting{ nullptr };	
		SoundBar* _pSoundBar{ nullptr };
		BackGround* _pBackGround{ nullptr };
	};
}