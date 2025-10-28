#include <TimeManager.h>
#include <TimerComponent.h>
#include <SoundManager.h>

bool Engine::TimerComponent::InitializeComponent()
{
    _timers.resize(1);

    return true;
}

void Engine::TimerComponent::BeginPlay()
{
}

void Engine::TimerComponent::TickComponent(_duration deltaSeconds)
{
	for (auto& timer : _timers)
	{
		if (timer.isEnd || timer.isPause)
			continue;

		if (!timer.isSyncMusic)
		{
			if (timer.isSyncTimeScale)
			{
				timer.currentTime += deltaSeconds;
			}
			else
			{
				timer.currentTime += Time->GetNonScaleDeltaSeconds();
			}

			if (timer.currentTime >= timer.innerDuration)
			{
				if (timer.isLoop)
				{
					timer.onTimeOver.Broadcast();
					timer.currentTime = timer.currentTime.zero();
				}
				else
				{
					timer.onTimeOver.Broadcast();
					timer.isEnd = true;
				}
			}
		}
		else
		{
			_ldouble syncTime = Sound->GetPosition(timer.syncChannel);
			timer.currentSyncTime += std::abs(syncTime - timer.previousTime);
			timer.previousTime = syncTime;
			if (timer.currentSyncTime >= timer.innerSyncTime)
			{
				if (timer.isLoop)
				{
					timer.onTimeOver.Broadcast();
					timer.currentSyncTime = 0;
				}
				else
				{
					timer.onTimeOver.Broadcast();
					timer.isEnd = true;
				}
			}
		}
	}
}

void Engine::TimerComponent::EndPlay()
{
	for (auto& timer : _timers)
	{
		timer.onTimeOver.Unbind();
	}
}

Engine::TimerComponent* Engine::TimerComponent::Create()
{
	return new TimerComponent;
}

void Engine::TimerComponent::Destroy()
{
    _timers.clear();
}

Engine::TimerComponent::InnerTimer& Engine::TimerComponent::GetTimer(int timerIndex)
{
	return _timers[timerIndex];
}

void Engine::TimerComponent::AddTimer(int timerSize)
{
	_timers.resize(timerSize);
}

void Engine::TimerComponent::SetTime(int timerIndex, _duration nanoSeconds, bool isLoop)
{
	_timers[timerIndex].innerDuration = nanoSeconds;
	_timers[timerIndex].currentTime = _duration{ 0 };
	_timers[timerIndex].isLoop = isLoop;
	_timers[timerIndex].isEnd = false;
}

void Engine::TimerComponent::SetTime(int timerIndex, float castTargetTime, bool isLoop)
{
	_timers[timerIndex].innerDuration = Time->FloatToDuration(castTargetTime);
	_timers[timerIndex].currentTime = _duration{ 0 };
	_timers[timerIndex].isLoop = isLoop;
	_timers[timerIndex].isEnd = false;
}

void Engine::TimerComponent::SetSyncTimeScale(int timerIndex, bool isSyncTimeScale)
{
	_timers[timerIndex].isSyncTimeScale = isSyncTimeScale;
}

void Engine::TimerComponent::SetTimeSyncMusic(int timerIndex, int syncChannel, float castTargetTime, bool isLoop)
{
	_timers[timerIndex].innerSyncTime = static_cast<_ldouble>(castTargetTime) * (long double)1e9;
	_timers[timerIndex].currentSyncTime = 0;
	_timers[timerIndex].isLoop = isLoop;
	_timers[timerIndex].syncChannel = static_cast<SoundChannel>(syncChannel);
	_timers[timerIndex].isSyncMusic = true;
	_timers[timerIndex].isEnd = false;
	_timers[timerIndex].previousTime = Sound->GetPosition(syncChannel);
}

bool Engine::TimerComponent::IsOverTime(int timerIndex, _duration nanoSeconds)
{
	return _timers[timerIndex].currentTime >= nanoSeconds;
}

bool Engine::TimerComponent::IsOverTime(int timerIndex, float castTargetTime)
{
	return _timers[timerIndex].currentTime >= Time->FloatToDuration(castTargetTime);
}

void Engine::TimerComponent::PauseTime(int timerIndex)
{
	_timers[timerIndex].isPause = true;
}

void Engine::TimerComponent::ResumeTime(int timerIndex)
{
	_timers[timerIndex].isPause = false;
}

void Engine::TimerComponent::StopTime(int timerIndex)
{
	_timers[timerIndex].innerDuration = _duration::zero();
	_timers[timerIndex].currentTime = _duration::zero();
	_timers[timerIndex].isLoop = false;
	_timers[timerIndex].isEnd = true;
}
