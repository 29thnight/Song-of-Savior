#include <CoreDefine.h>
#include <SoundManager.h>
#include <TimeManager.h>

namespace file = std::filesystem;

bool Engine::SoundManager::Initialize(int maxChannels)
{
	int numberOfAvailableChannels{};

	FMOD::System_Create(&_pSystem);
	_pSystem->getSoftwareChannels(&numberOfAvailableChannels);
	_pSystem->init(maxChannels, FMOD_INIT_NORMAL, nullptr);

	_maxChannels = maxChannels;
	_channelGroups.resize(maxChannels);
	_fadeSounds.resize(maxChannels);
	_pSystem->getMasterChannelGroup(&_pMasterChannelGroup);
	_pSystem->getSoftwareFormat(&_pSamplateInt, nullptr, nullptr);

	for (int i = 0; i < maxChannels; ++i)
	{
		_pSystem->createChannelGroup(nullptr, &_channelGroups[i]);
		_pMasterChannelGroup->addGroup(_channelGroups[i]);
	}

	_pMasterChannelGroup->setVolume(0.5f);

	return true;
}

void Engine::SoundManager::LoadSound(const char* filePath)
{
	file::path rootPath(filePath);

	for(const auto& entry : file::directory_iterator(rootPath))
	{
		if (entry.is_directory())
		{
			if (entry.path().filename() == "." || entry.path().filename() == "..")
				continue;

			LoadSound(entry.path().string().c_str());
		}
		else
		{
			file::path fullPath = entry.path();
			
			std::string tag = fullPath.filename().string();
			tag = tag.substr(0, tag.find_last_of('.'));


			FMOD::Sound* pSound = _sounds[tag];

			if (nullptr == pSound)
			{
				_pSystem->createSound(fullPath.string().c_str(), FMOD_DEFAULT, nullptr, &pSound);
				_sounds[tag] = pSound;
			}
		}
	}
}

void Engine::SoundManager::Update(_duration deltaSeconds)
{
	_pSystem->update();
	for (int i = 0; i < _maxChannels; ++i)
	{
		if (_fadeSounds[i].current >= _fadeSounds[i].target)
		{
			_fadeSounds[i].current -= Time->DurationToFloat(deltaSeconds);
		}
		_channelGroups[i]->setVolume(_fadeSounds[i].current);
	}

	for (auto it = _executeChannels.begin(); it != _executeChannels.end(); )
	{
		bool isPlaying = false;
		int loopCount = 0;
		(*it)->isPlaying(&isPlaying);
		(*it)->getLoopCount(&loopCount);

		if (!isPlaying && loopCount == 0)
		{
			it = _executeChannels.erase(it); // 항목을 제거한 후 이터레이터를 갱신
		}
		else
		{
			++it; // 다음 항목으로 이동
		}
	}
}

void Engine::SoundManager::PlaySound(const char* soundName, int channel, int loopCount)
{
	FMOD::Sound* pSound = _sounds[soundName];

	if(pSound)
	{
		FMOD::Channel* pChannel = nullptr;
		_uint startPos{ 0 };
		_uint endPos{ 0 };
		pSound->getLength(&endPos, FMOD_TIMEUNIT_MS);

		pSound->setLoopPoints(
			startPos, FMOD_TIMEUNIT_MS,
			endPos, FMOD_TIMEUNIT_MS
		);

		_pSystem->playSound(pSound, _channelGroups[channel], false, &pChannel);
		pChannel->setMode(FMOD_LOOP_NORMAL);
		pChannel->setLoopCount(loopCount);
		_executeChannels.push_back(pChannel);
	}
}

void Engine::SoundManager::StopSound(int channel)
{
	FMOD::Channel* _channel;
	_channelGroups[channel]->getChannel(0, &_channel);
	_executeChannels.remove(_channel);	
	_channelGroups[channel]->stop();
	_pSystem->createChannelGroup(nullptr, &_channelGroups[channel]);
}

void Engine::SoundManager::StopAllSound()
{
	_pMasterChannelGroup->stop();
}

bool Engine::SoundManager::FindSound(const std::string& key)
{
	auto findIter = _sounds.find(key);
	if (findIter != _sounds.end())
	{
		return true;
	}
	else
	{
		return false;
	}
}

float Engine::SoundManager::GetMasterVolume()
{
	float volume{};
	_pMasterChannelGroup->getVolume(&volume);
	return volume;
}

void Engine::SoundManager::SetMasterVolume(float volume)
{
	_pMasterChannelGroup->setVolume(volume);
}

void Engine::SoundManager::SetTargetVolume(int channel, float volume)
{
	_fadeSounds[channel].target = volume;
}

void Engine::SoundManager::SetVolume(int channel, float volume)
{
	_fadeSounds[channel].current = volume;
	_fadeSounds[channel].target = volume;
}

void Engine::SoundManager::SetMute(int channel, bool mute)
{
	_channelGroups[channel]->setMute(mute);
}

void Engine::SoundManager::SetPause(int channel, bool pause)
{
	_channelGroups[channel]->setPaused(pause);
}

void Engine::SoundManager::SetPitch(int channel, float pitch)
{
	_channelGroups[channel]->setPitch(pitch);
}

void Engine::SoundManager::SetPosition(int channel, long double nanosec)
{
	if (channel < 0 || channel >= _maxChannels)
	{
		return;  // 잘못된 채널 번호 처리
	}

	_uint position = static_cast<_uint>((nanosec / 1e9) * _pSamplateInt);
	FMOD::Channel* pChannel = nullptr;
	_channelGroups[channel]->getChannel(0, &pChannel);

	if (pChannel)
	{
		pChannel->setPosition(position, FMOD_TIMEUNIT_PCM);
	}
}

void Engine::SoundManager::SetPan(int channel, float pan)
{
	_channelGroups[channel]->setPan(pan);
}

void Engine::SoundManager::LoopEnter(int channel, _uint startPosition, _uint endPosition)
{
	FMOD::Channel* pChannel = nullptr;

	_channelGroups[channel]->getChannel(0, &pChannel);
	pChannel->setLoopPoints(
		startPosition, FMOD_TIMEUNIT_MS,
		endPosition, FMOD_TIMEUNIT_MS
	);
	pChannel->setMode(FMOD_LOOP_NORMAL);
	pChannel->setLoopCount(LOOP_INFINITE);
}

void Engine::SoundManager::LoopExit(int channel, int loopCount)
{
	FMOD::Channel* pChannel = nullptr;
	FMOD::Sound* pSound = nullptr;

	_channelGroups[channel]->getChannel(0, &pChannel);
	_uint startPos{ 0 };
	_uint endPos{ 0 };
	pChannel->getCurrentSound(&pSound);
	pSound->getLength(&endPos, FMOD_TIMEUNIT_MS);
	pChannel->setLoopPoints(
		startPos, FMOD_TIMEUNIT_MS,
		endPos, FMOD_TIMEUNIT_MS
	);
	pChannel->setMode(FMOD_LOOP_NORMAL);
	pChannel->setLoopCount(loopCount);
}

long double Engine::SoundManager::GetPosition(int channel)
{
	if (channel < 0 || channel >= _maxChannels) 
	{
        return 0;  // 잘못된 채널 번호 처리
    }

	long double result = 0;
    _uint position = 0;
    FMOD::Channel* pChannel = nullptr;
    _channelGroups[channel]->getChannel(0, &pChannel);

    if (pChannel)
	{
        pChannel->getPosition(&position, FMOD_TIMEUNIT_PCM);
		result = (static_cast<long double>(position) / _pSamplateInt) * 1e9;
	}

    return result;
}

long double Engine::SoundManager::GetLength(int channel)
{
	if (channel < 0 || channel >= _maxChannels)
	{
		return 0;  // 잘못된 채널 번호 처리
	}

	long double result = 0;
	_uint length = 0;
	FMOD::Channel* pChannel = nullptr;
	FMOD::Sound* pSound = nullptr;
	_channelGroups[channel]->getChannel(0, &pChannel);
	pChannel->getCurrentSound(&pSound);

	if (pSound)
	{
		pSound->getLength(&length, FMOD_TIMEUNIT_PCM);
		result = static_cast<long double>(length / _pSamplateInt * 1e9);
	}

	return result;
}

bool Engine::SoundManager::GetPause(int channel)
{
	bool result;
	_channelGroups[channel]->getPaused(&result);

	return result;
}

void Engine::SoundManager::Destroy()
{
	for (auto& sound : _sounds)
	{
		sound.second->release();
	}

	for (auto& channelGroup : _channelGroups)
	{
		channelGroup->release();
	}

	_pMasterChannelGroup->release();

	_pSystem->close();
	_pSystem->release();

	_sounds.clear();
}
