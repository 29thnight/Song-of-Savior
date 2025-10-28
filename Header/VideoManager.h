#pragma once
#include <CoreBase.h>
#include <CoreDefine.h>
#include <Singleton.h>

namespace Engine
{
	CORECLASS()
	class VideoManager : public CoreBase, public Singleton<VideoManager>
	{
	private:
		friend class Singleton;

	private:
		VideoManager() = default;
		~VideoManager() = default;

	public:
		bool LoadVideo(std::wstring path);
		void Play();
		void Pause();
		void Stop();
		void Skip();
		bool VideoStateUpdate();
		bool IsPlaying() const { return m_isPlaying; }
		bool IsEnd() const { return m_isEnd; }

	public:
		virtual void SerializeIn(nlohmann::ordered_json& object) {};
		virtual void SerializeOut(nlohmann::ordered_json & object) {};

	public:
		void Initialize();
		void Destroy() override;

	private:
		HWND _hWnd = nullptr;

		bool m_isPlaying = false;
		bool m_isEnd = false;
	};
}

inline Engine::VideoManager* VideoMgr = Engine::VideoManager::GetInstance();
inline std::function<void()> VideoTerminate = []() { VideoMgr->Terminate(); };