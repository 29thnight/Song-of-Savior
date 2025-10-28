#pragma once
#include <CoreDefine.h>
#include <Actor.h>
#include <Singleton.h>
#include <SimpleIniLoader.h>

#include "ClientEnum.h"

namespace Engine
{
	CORECLASS()
};

namespace Client
{
	class UiIconSkill;
	class Boss;
	class Character;
	class NoteManager;
	class BossManager;
	class BGMManager;
	class BackGround;
	class Setting;
	class GameManager : public Engine::Actor, public Singleton<GameManager>
	{
	private:
		friend class Singleton;

	private:
		GameManager() = default;
		virtual ~GameManager() = default;

	public:
		void SpawnStageActor(Engine::Level* targetLevel);
		void InitializeStage();
		void InitializeGameManagerComponent();
		void ReadyToTransitionStage(Engine::Level* targetLevel);

	public:
		virtual void BeginPlay() override;
		virtual void Tick(_duration deltaSeconds) override;
		virtual void Fixed() override;
		virtual void EndPlay() override;
		virtual void SetupInputComponent(Engine::InputComponent* inputComponent) override;
		virtual void Destroy() override;

	public:
		void AddHPBarBlinkTarget(int target) { _hpBarBlinkTargets.insert(target); }
		void ClearHPBarBlinkTargets() { _hpBarBlinkTargets.clear(); }
		void UpdateCharacterHPBarBlink(int willBeDecreaseHP);
		void CancelCharacterHPBarBlink();
		void SetBindKeySetting(GameKeySettingType setting);
		void UpdateStageClearConditions();
		void UpdateHPDefeatConditions();
		void HPDefeat();
		void UpdateDefeatConditions();
		void SpawnBoss(Stage stage);
		void SetFullHP();
		void ChainLoadingScreen();
		void LoadingScreenEndAndFadeIn();
		void LoadingScreenView();
		void ChangeNextStage();
		void ChangeStage(Stage stage);
		void RemoveAllCharacters();
		void ChangeEditorLevel();
		void DebugChangeStage();
		bool IsDefeat() const { return _isDefeat; }
		bool staticInputPress(_uint key);
		void AddAllCharacterMaxHP(int value);
		Boss* GetCurrentBoss() { return _pBoss; };
		std::pair<float, int> GetCurrSmashingData();

	public:
		NoteManager* GetNoteManager() { return _pNoteManager; }

	public:
		bool IsPause() const { return _isPaused; }

	public:
		_Property(bool, IsSceneChangeTrigger)
		_Get(IsSceneChangeTrigger) { return _isSceneChangeTriggered; }
		_Set(IsSceneChangeTrigger) { _isSceneChangeTriggered = value; }

	public:
		void PauseGame();
		void ResumeGame();
		void DelayedResumeGame();
		void PlayCountDown();
		void SetRestart(bool value);
		void ReviveAllCharacter();
		void DealDamageToCharacter(CharacterType target, int damage);
		void DealSkillDamageToCharacter(CharacterType target, int damage);
		void DealDamageToBoss(int damage, bool ableToAttackCancel = false);
		void DealSkillDamageToBoss(int damage, CharacterType target);
		void BardHeal();
		void ChargeMana(CharacterType target, int mana);
		void CancelBossAttack();
		Stage GetCurrentStage() const { return currentStage; }

	private:
		Boss*	   _pBoss{ nullptr };
		Character* _pWarrior{ nullptr };
		Character* _pThief{ nullptr };
		Character* _pSorcerer{ nullptr };
		Character* _pBard{ nullptr };
		std::vector<Character*> _characterContainer;

		UiIconSkill* _pUiWarriorSkill{ nullptr };
		UiIconSkill* _pUiThiefSkill{ nullptr };
		UiIconSkill* _pUiSorcererSkill{ nullptr };
		UiIconSkill* _pUiBardSkill{ nullptr };

	private:
		Setting*	_pSetting{ nullptr };
		BackGround* _pBackGround{ nullptr };

	private:
		NoteManager* _pNoteManager{ nullptr };
		BossManager* _pBossManager{ nullptr };
		BGMManager* _pBGMManager{ nullptr };

	private:
		Engine::BasicAnimationComponent* _pLoadingScreen{ nullptr };
		Engine::BitmapComponent* _pTimerBitmap{ nullptr };
		Engine::TimerComponent* _pTimerComponent{ nullptr };
		Engine::InputComponent* _pInputComponent{ nullptr };

	private:
		GameKeySettingType _currentKeySetting{ GameKeySettingType::A };
		Stage currentStage{ Stage::Title };
		Difficulty _currentDifficulty{ Difficulty::Normal };
		GameStatus _gameStatus{ GameStatus::Playing };

	private:
		bool _isSceneChangeTriggered{ false };
		bool _isDefeat{ false };
		bool _isStageClear{ false };
		bool _isFirstSpawn{ false };
		bool _isRestart{ false };
		bool _isPaused{ false };

	private:
		std::set<int> _hpBarBlinkTargets;

	private:
		SimpleIniFile _smashingInI;
	};
}

inline Client::GameManager* Manager = Client::GameManager::GetInstance();
inline std::function<void()> GameTerminate = []() { Manager->Terminate(); };