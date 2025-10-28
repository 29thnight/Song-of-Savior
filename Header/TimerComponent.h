#pragma once
#include <ActorComponent.h>

namespace Engine
{
	class TimerComponent : public ActorComponent
	{
	private:
		struct InnerTimer
		{
			_ldouble  innerSyncTime{};
			_ldouble  previousTime{};
			_ldouble  currentSyncTime{};
			_duration innerDuration{};
			_duration currentTime{};
			SoundChannel syncChannel{};

			bool isLoop{ false };
			bool isPause{ false };
			bool isEnd{ true };
			bool isSyncMusic{ false };
			bool isSyncTimeScale{ true };
			Delegate<> onTimeOver{};
		};

	private:
		explicit TimerComponent() = default;
		virtual ~TimerComponent() = default;

	public:
		virtual bool InitializeComponent();
		virtual void BeginPlay();
		virtual void TickComponent(_duration deltaSeconds);
		virtual void EndPlay();

	public:
		static TimerComponent* Create();
		void Destroy() override;

	public:
		Engine::TimerComponent::InnerTimer& GetTimer(int timerIndex);
		void AddTimer(int timerSize);
		void SetTime(int timerIndex, _duration nanoSeconds, bool isLoop = false);
		void SetTime(int timerIndex, float castTargetTime, bool isLoop = false);
		void SetSyncTimeScale(int timerIndex, bool isSyncTimeScale);
		void SetTimeSyncMusic(int timerIndex, int syncChannel, float castTargetTime, bool isLoop = false);
		bool IsOverTime(int timerIndex, _duration nanoSceconds);
		bool IsOverTime(int timerIndex, float castTargetTime);
		void PauseTime(int timerIndex);
		void ResumeTime(int timerIndex);
		void StopTime(int timerIndex);

	private:
		std::vector<InnerTimer> _timers;
	};
}