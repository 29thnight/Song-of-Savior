#pragma once
#include <CoreDefine.h>
#include <Actor.h>
#include <SoundManager.h>

namespace Engine
{
	CORECLASS()
}

namespace Client
{
	class BGMManager : public Engine::Actor
	{
		friend class NoteEditor;
		using Base = Engine::Actor;
	private:
		BGMManager() = default;
		virtual ~BGMManager() = default;

	public:
		virtual void BeginPlay() override;
		virtual void Tick(_duration deltaSeconds) override;
		virtual void Fixed() override;
		virtual void EndPlay() override;
		virtual void ReviveInitialize() override;

	public:		
		void CreateBGMList(int stageCount);
		void CreatePhaseSize(int stageIndex, int phaseSize);
		void ReadBGMList(std::string filePath);
		void SetBGMList(int stageNumber, int phaseNumber, std::string filePath);
		void WriteBGMList(std::string filePath) const;
		bool IsDelayedPlayBGM() const { return _isDelayedPlayBGM; }

	public:
		void DelayedPlayBGM(float delayTime);
		void PlayBGM();
		bool PauseBGM();
		void StopBGM();

		bool IsBGMEnd();

	public:
		ReadOnly_Property(long double, GetPosition);
		_Get(GetPosition)
		{
			return Sound->GetPosition(CHANNEL_BGM);
		}

		_Property(int, CurrentStage);
		_Get(CurrentStage)
		{
			return _currentStage;
		}
		_Set(CurrentStage)
		{
			_currentStage = value;
		}

		_Property(int, CurrentPhase);
		_Get(CurrentPhase)
		{
			return _currentPhase;
		}
		_Set(CurrentPhase)
		{
			_currentPhase = value;
		}

	public:
		static BGMManager* Create() { return new BGMManager; }
		void Destroy() override { Actor::Destroy(); }

	private:
		std::vector<std::vector<std::string>> _bgmList;

	private:
		float _delayTime{};
		int	 _stageNumber{};
		int	 _currentStage{};
		int	 _currentPhase{};
		bool _isDelayedPlayBGM{ false };

		bool _isPlayBGM{ false };
	};
}