#pragma once
#include <Actor.h>
#include "ClientEnum.h"

namespace Engine
{
	CORECLASS()
}

namespace Client
{
	class NoteEditor : public Engine::Actor
	{
	public:
		enum class EMODE
		{
			NOTE,
			BOSS,
			END
		};

	private:
		NoteEditor() = default;
		virtual ~NoteEditor() = default;

	public:
		virtual void BeginPlay() override;
		virtual void Tick(_duration deltaSeconds) override;
		virtual void Fixed() override;
		virtual void EndPlay() override;

	public:
		virtual void SetupInputComponent(Engine::InputComponent* inputComponent) override;

	public:
		static NoteEditor* Create() { return new NoteEditor; }
		void Destroy() override { Actor::Destroy(); }

	private:
		void SelectStage();
		void SelectPhase();
		void SelectBgm();

		void SelectMode(EMODE mode);
		/// <summary>
		/// \TODO: we will add difficulty system
		/// </summary>
		void SelectDifficulty(Difficulty difficulty);
		//TODO END : we will add difficulty system

		void ResizeStage();
		void ResizePhase();

		void MoveBgmPosition();

		void UpdateNoteManager();

		void InsertNote();
		void DeleteNote();

		void UpdateDebugText();
		void UpdateNoteListText();
		
		void UpdateBGMManager();

		void SaveNoteToCSV();
		void LoadNoteCSV();
		void NewFileNoteCSV(const std::wstring& path);

	private:
		_Property(int, selectStage)
		_Get(selectStage)
		{
			return _selectStage;
		}
		_Set(selectStage)
		{
			_selectStage = value;
			UpdateBGMManager();
			LoadNoteCSV();
			LoadBossPatternCSV();
			UpdateNoteManager();
			UpdateBossManager();
		}

		_Property(int, selectPhase)
			_Get(selectPhase)
		{
			return _selectPhase;
		}
		_Set(selectPhase)
		{
			_selectPhase = value;
			UpdateBGMManager();
			LoadNoteCSV();
			LoadBossPatternCSV();
			UpdateNoteManager();
			UpdateBossManager();
		}

	private:
		//Boos Patten edit
		//void InsertBossPattern(int pattenNum);
		//void DeleteBossPattern();

		void UpdateBossManager();

		void SaveBossPatternToCSV();
		void LoadBossPatternCSV();
		void NewFileBossPatternCSV(const std::wstring& path);

	private:
		class NoteManager* _pNoteManager = nullptr;
		class BGMManager* _pBGMManager = nullptr;
		class TimelineManager* _pTimelineManager = nullptr;
		class BossManager* _pBossManager = nullptr;
		Engine::InputComponent* _pInputComponent = nullptr;
		Engine::TextRenderComponent* _pEditorDebugText = nullptr;
		Engine::TextRenderComponent* _pNoteListText = nullptr;

		int _selectBossAttackType = 1;
		int _selectStage = -1;
		int _selectPhase = -1;
		int _debugIndex = 0;

		bool _respawnBoss{false};

		EMODE _selectMode = EMODE::NOTE;
		/// <summary>
		/// \TODO: we will add difficulty system
		/// </summary>
		Difficulty _selectDifficulty = Difficulty::Normal;
		//TODO END : we will add difficulty system

		bool isPause = false;
	};
}