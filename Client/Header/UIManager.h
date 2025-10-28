#pragma once
#include <Actor.h>

namespace Engine
{
	CORECLASS()
}

namespace Client
{
	class UI;

	enum UI_SCENE
	{
		Title_Default,
		Title_Continue,
		GameOver,
		Setting_UI,
		Setting_Title_UI,
		KeySettings_UI,
	};
	
	class UIManager : public Engine::Actor
	{
		using Base = Engine::Actor;
	private:
		UIManager() = default;
		virtual ~UIManager() = default;

	public:
		virtual void BeginPlay() override;
		virtual void Tick(_duration deltaSeconds) override;
		virtual void Fixed() override;
		virtual void EndPlay() override;

	public:
		virtual void SetupInputComponent(Engine::InputComponent* inputComponent) override;

	public:
		static UIManager* Create() { return new UIManager; }
		void Destroy() override;
		const std::vector<UI*>& GetButtonList();
		const std::vector<UI*>& GetImageList();

	public:
		void SpawnUI(UI_SCENE scene);
		void SetFocusUI(int index);

	private:
		void FocusButtonUp();
		void FocusButtonDown();

	private:
		void UIAllDestroy();

	private:
		std::vector<UI*> _vecButtonUI{};
		std::vector<UI*> _vecImageUI{};
		int _focusUIindex{-1};

		bool _bIsGameOverUI = false;

		bool _gamePadDown = false;
		bool _gamePadUp = false;
	private:
		Engine::InputComponent* _pInputComponent{ nullptr };
		Engine::TimerComponent* _pTimerComponent{ nullptr };
	};
}