#include <VideoManager.h>
#include <CoreManager.h>
#include <CoreManager.h>
#include <CoreBase.h>
#include <CoreDefine.h>


bool Engine::VideoManager::LoadVideo(std::wstring path)
{
	_hWnd = MCIWndCreate(Management->setting.hWnd, nullptr, WS_CHILD | WS_VISIBLE | MCIWNDF_NOPLAYBAR, path.c_str());

	return _hWnd != nullptr;
}

void Engine::VideoManager::Play()
{
	if (m_isPlaying)
		return;

#ifdef FINAL
	MoveWindow(_hWnd, 0, 0, Management->setting.width, Management->setting.height + 30, FALSE);
#endif // FINAL

#ifndef FINAL
	MoveWindow(_hWnd, 0, 0, Management->setting.width, Management->setting.height, FALSE);
#endif // !FINAL
	MCIWndPlay(_hWnd);
	m_isEnd = false;
	m_isPlaying = true;
}

void Engine::VideoManager::Pause()
{
	MCIWndPause(_hWnd);
	m_isPlaying = false;
}

void Engine::VideoManager::Stop()
{
	MCIWndStop(_hWnd);
	MCIWndClose(_hWnd);
	m_isPlaying = false;
	m_isEnd = true;
}

void Engine::VideoManager::Skip()
{
	MCIWndSeek(_hWnd, MCIWND_END);
	MCIWndClose(_hWnd);
	m_isEnd = true;
}

bool Engine::VideoManager::VideoStateUpdate()
{
	if (m_isPlaying)
	{
		if (MCIWndGetPosition(_hWnd) >= MCIWndGetLength(_hWnd))
		{
			Stop();
			m_isEnd = true;
		}
		return true;
	}

	return false;
}

void Engine::VideoManager::Initialize()
{
}

void Engine::VideoManager::Destroy()
{
	MCIWndClose(_hWnd);
}
