#pragma once
#include <Singleton.h>
#include <CoreDefine.h>
#include <../ThirdParty/FMOD/inc/fmod.hpp>

#undef PlaySound
namespace Engine
{
	class SoundManager : public CoreBase, public Singleton<SoundManager>
	{
	private:
		friend class Singleton;

	private:
		SoundManager() = default;
		~SoundManager() = default;

	public:
		constexpr static int _inf = -1;

	public:
		bool Initialize(int maxChannels);
		void LoadSound(const char* filePath);
		void Update(_duration deltaSeconds);

	public:
		void PlaySound(const char* soundName, int channel, int loopCount = -1);
		void StopSound(int channel);
		void StopAllSound();

	public:
		/* 리소스에 해당 파일의 존재 여부를 반환합니다.*/
		bool FindSound(const std::string& key);

	public:
		float GetMasterVolume();
		void SetMasterVolume(float volume);
		void SetTargetVolume(int channel, float volume);
		void SetVolume(int channel, float volume);
		void SetMute(int channel, bool mute);
		void SetPause(int channel, bool pause);
		void SetPitch(int channel, float pitch);
		void SetPosition(int channel, long double nanosec);

	public:
		void SetPan(int channel, float pan);
		void LoopEnter(int channel, _uint startPosition, _uint endPosition);
		void LoopExit(int channel, int loopCount);
		long double GetPosition(int channel);
		long double GetLength(int channel);
		bool GetPause(int channel);

	public:
		virtual void Destroy() override;

	public:
		virtual void SerializeIn(nlohmann::ordered_json& object) {};
		virtual void SerializeOut(nlohmann::ordered_json& object) {};

	private:
		FMOD::System*									_pSystem{ nullptr };
		FMOD::ChannelGroup*								_pMasterChannelGroup{ nullptr };
		std::vector<FMOD::ChannelGroup*>				_channelGroups;
		std::list<FMOD::Channel*>						_executeChannels;
		std::unordered_map<std::string, FMOD::Sound*>	_sounds;
		std::vector<FadeSound>							_fadeSounds;
		int												_maxChannels{ 0 };
		int												_pSamplateInt{ 0 };
	};
}

inline Engine::SoundManager* Sound = Engine::SoundManager::GetInstance();
inline std::function<void()> SoundTerminate = []() { Sound->Terminate(); };