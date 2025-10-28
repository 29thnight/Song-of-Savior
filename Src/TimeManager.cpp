#include <TimeManager.h>

constexpr auto immediate = std::chrono::nanoseconds{1000000};

bool Engine::TimeManager::Initialize()
{
	_prevTick = _clock::now();

	return true;
}

void Engine::TimeManager::UpdateTick()
{
	auto currentTick = _clock::now();
	_duration duration = currentTick - _prevTick;
	_deltaSeconds = std::chrono::duration_cast<_duration>(duration * _timeScale);
	_nonScaleDeltaSeconds = std::chrono::duration_cast<_duration>(duration);
	_prevTick = currentTick;
}

void Engine::TimeManager::SetTimeScale(_float timeScale)
{
	_timeScale = timeScale;
}

int Engine::TimeManager::GetFPS() const
{
	return static_cast<int>(round(1e9 / _deltaSeconds.count()));
}

_duration Engine::TimeManager::GetDeltaSeconds() const
{
    if (_deltaSeconds > std::chrono::nanoseconds{100000000}) // 100 milliseconds in nanoseconds
        return immediate;

    return _deltaSeconds;
}

_duration Engine::TimeManager::GetNonScaleDeltaSeconds() const
{
	if (_deltaSeconds > std::chrono::nanoseconds{100000000}) // 100 milliseconds in nanoseconds
        return immediate;

	return _nonScaleDeltaSeconds;
}

_float Engine::TimeManager::DurationToFloat(_duration duration) const
{
	return std::chrono::duration_cast<std::chrono::duration<float>>(duration).count();
}

_float Engine::TimeManager::NonScaleDurationToFloat() const
{
	return std::chrono::duration_cast<std::chrono::duration<float>>(_nonScaleDeltaSeconds).count();
}

_duration Engine::TimeManager::FloatToDuration(_float duration) const
{
	return std::chrono::duration_cast<_duration>(std::chrono::duration<float>(duration));
}

_float Engine::TimeManager::GetTimeScale() const
{
	return _timeScale;
}

void Engine::TimeManager::Destroy()
{
}
