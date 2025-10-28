#pragma once
#include <Actor.h>

namespace Engine
{
	CORECLASS()
}

namespace Client
{
	class TimelineManager : public Engine::Actor
	{
		using Base = Engine::Actor;
		friend class NoteManager;
	private:
		TimelineManager() = default;
		virtual ~TimelineManager() = default;

	public:
		virtual void BeginPlay() override;
		virtual void Tick(_duration deltaSeconds) override;
		virtual void Fixed() override;
		virtual void EndPlay() override;

	public:
		static TimelineManager* Create() { return new TimelineManager; }
		void Destroy() override;

	public:
		void UpdateCurrNote() { _updateNote = true; }

	public:
		_Property(int, TimelineBlockCount)
		_Get(TimelineBlockCount)
		{
			return _timelineBlockCount;
		}
		_Set(TimelineBlockCount)
		{
			DestroyTimeline();
			_timelineBlockCount = std::clamp(value, 50, 200);
			_isDestroyTimelineBlock = true;
			_updateNote = true;
		}

		_Property(int , SelectMode)
		_Get(SelectMode)
		{
			return _selectMode;
		}
		_Set(SelectMode)
		{
			_selectMode = value;
			UpdateCurrNote();
		}

	private:
		class CurrTimeBlock* _pCurrTimeBlock = nullptr;
		class BGMManager* _pBGMManager = nullptr;
		class NoteManager* _pNoteManager = nullptr;
		class BossManager* _pBossManager = nullptr;

	private:
		std::vector<Engine::Actor*> timelineCurrNoteArray;

	private:
		float _startPosition{};
		float _endPosition{};

		int _timelineBlockCount = 100;
		int _selectMode{};

		bool _isDestroyTimelineBlock = false;
		bool _isSpawnCurrNote = true;
		bool _updateNote = false;

	private:
		void DestroyTimeline();
		void SpawnTimeline();

		void UpdateCurrTimeBlock();

		void SpawnCurrNote();
	};
}