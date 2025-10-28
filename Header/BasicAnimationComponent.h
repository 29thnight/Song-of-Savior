#pragma once

#include <BitmapComponent.h>

namespace Engine
{
	CORECLASS()
	class BasicAnimationComponent : public BitmapComponent
	{
	protected:
		explicit BasicAnimationComponent() = default;
		virtual ~BasicAnimationComponent() = default;

	public:
		virtual bool InitializeComponent() override;
		virtual void TickComponent(_duration deltaSeconds) override;
		virtual void Render(_RenderTarget pRenderTarget) override;

	public:
		void AllAddClipThisActor();
		void AllAddClipThisActor(string csvPath, string ownerFolderName);
		void AddClip(_pstring clipName, _float frameTime, bool isLoop = false);
		void AddClip(_pstring clipName, _float frameTime, bool isLoop, _pstring ownerFolderName);
		void SetPlayClip(_pstring clipName);
		void RemoveClip(_pstring clipName);

		template <typename T>
		void AddFrameEvent(_pstring clipName, int frameIndex, T* obj, void(T::* _event)());

		template <typename T>
		void AddFrameLambda(_pstring clipName, int frameIndex, T _event);

	public:
		const bool IsClipPlay(_pstring clipName) const;
		const bool IsClipEnd(_pstring clipName) const;
		const bool IsCurrClip(_pstring clipName) const;
		const bool IsFrameEnd() const;

	public:
		//클립의 프레임 수
		int GetClipMaxframe(_pstring clipName);
		//애니메이션 클립의 총 시간
		float GetClipTime(_pstring clipName) const;
		int GetCurrentFrame() const { return _currentFrame; }

	public:
		virtual void SerializeIn(nlohmann::ordered_json& object) {};
		virtual void SerializeOut(nlohmann::ordered_json& object) {};
		
	protected:
		virtual void Destroy() override;

	public:
		static BasicAnimationComponent* Create();

	private:
		std::unordered_map<std::string, AnimationClip*> _vecClips;
		std::unordered_map<std::string, std::vector<std::pair<int, Delegate<>>>> _vecFrameEvents;

	private:
		_pstring _currentClipName{};

	private:
		long double _prevPosition{ 0 };
		float	 _frameTime{ 0 };
		float	 _currentFrameTime{ 0 };
		float    _frameSpeed{ 1 };

	private:
		int		 _indexCount{ 0 };
		int		 _prevFrame{ 0 };
		int		 _currentFrame{ -1 };
		int		 _frameCount{ 0 };
		int		 _currentClipIndex{ -1 };

	private:
		bool	 _isLoop{ false };
		bool     _isInLayer{ false };
		bool     _isFrameEnd{ false };
	};

}

template<typename T>
inline void Engine::BasicAnimationComponent::AddFrameEvent(_pstring clipName, int frameIndex, T* obj, void(T::* _event)())
{
	static_assert(std::is_base_of_v<Engine::Actor, T>, "T is not Actor");

	Delegate<> _delegate;
	_delegate.AddDynamic(obj, _event);
	_vecFrameEvents[clipName].push_back(std::make_pair(frameIndex, _delegate));
}

template<typename T>
inline void Engine::BasicAnimationComponent::AddFrameLambda(_pstring clipName, int frameIndex, T _event)
{
	Delegate<> _delegate;
	_delegate.AddLambda(_event);
	_vecFrameEvents[clipName].push_back(std::make_pair(frameIndex, _delegate));
}
