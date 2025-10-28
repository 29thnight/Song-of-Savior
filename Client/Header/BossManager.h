#pragma once
#include <MainEntry.h>
#include <Actor.h>

#include "ClientEnum.h"

namespace Engine
{
	CORECLASS()
}

namespace Client
{
	class Boss; 
	class BossHP;
	class BossManager : public Engine::Actor
	{
		friend class NoteEditor;
		friend class TimelineManager;
		using Base = Engine::Actor;

	private:
		BossManager() = default;
		virtual ~BossManager() = default;

	public:
		struct NoteTable
		{
			long double _noteTime{};
			int			_noteIndex{};
			bool 	    _attack{ false };
			bool 	    _skill1{ false };
			bool 	    _skill2{ false };
		};

	public:
		virtual void BeginPlay() override;
		virtual void Tick(_duration deltaSeconds) override;
		virtual void Fixed() override;
		virtual void EndPlay() override;
		virtual void ReviveInitialize() override;

	public:
		Boss* SpawnBoss(Stage currendStage);
		void SetNoteSpawnData(const std::string& fileName);
		void SetStage(int stage) { _stage = stage; }

		int GetCurrBossHp();
		int GetCurrBossMaxHp();

		void OnBossHPBar();
		void OffBossHPBar();

		BossHP* GetBossHP() { return _pBossHP; }

	public:
		static BossManager* Create() { return new BossManager; }
		void Destroy() override { Actor::Destroy(); }

	public :
		long double GetPattenOffset(int pattenType);

	private:
		Boss* _pBoss = nullptr;
		BossHP* _pBossHP = nullptr;

		int			_noteIndexAttack{};
		int			_noteIndexSkill1{};
		int			_noteIndexSkill2{};
		Stage		_currentStage{};
		int 		_stage{};
		std::vector<NoteTable>		_noteSpawnData;
	};
}