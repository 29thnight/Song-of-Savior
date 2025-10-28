#pragma once
#include <BitmapComponent.h>

namespace Engine
{
	CORECLASS()
	class AnimationComponent : public BitmapComponent
	{
	protected:
		explicit AnimationComponent() = default;
		virtual ~AnimationComponent() = default;

	public:
		virtual bool InitializeComponent() override;
		virtual void TickComponent(_duration deltaSeconds) override;
		virtual void Render(_RenderTarget pRenderTarget) override;
		void SetAnimationRect(Animation* pAnimation);
		void AllAddClipThisActor();
		void AddClip(_pstring clipName);
		const bool IsClipPlay(_pstring clipName) const;
		const bool IsClipEnd(_pstring clipName) const;
		const bool IsFrameEnd() const;
		void SetPlayClip(_pstring clipName);
		void RemoveClip(_pstring clipName);
		void SetFrameSpeed(float speed) { _frameSpeed = speed; }

	public:
		void EditorAddClip(Animation* pAnimation, bool isLoop = false);
		void InEditMode();
		void OutEditMode();
		void EditorSetFrame(_int frame);
		void EditorNextFrame();
		void EditorPrevFrame();
		bool IsEditMode() const;
		int GetCurrentFrame() const;
		double GetClipDuration(_pstring clipName);

	public:
		virtual void SerializeIn(nlohmann::ordered_json& object) {};
		virtual void SerializeOut(nlohmann::ordered_json& object) {};
		
	protected:
		virtual void Destroy() override;

	public:
		static AnimationComponent* Create();

	private:
		std::unordered_map<std::string, AnimationClip*> _vecClips;

	private:
		Animation* _pAnimation{};
		_pstring _currentClipName{};

	private:
		long double _prevPosition{ 0 };
		float	 _frameTime{ 0 };
		float	 _currentFrameTime{ -1 };
		float    _frameSpeed{ 1 };

	private:
		int		 _indexCount{ 0 };
		int		 _prevFrame{ -1 };
		int		 _currentFrame{ -1 };
		int		 _frameCount{ -1 };
		int		 _currentClipIndex{ -1 };

	private:
		bool	 _isLoop{ false };
		bool	 _isInLayer{ false };
		bool	 _isFrameEnd{ false };
		bool     _isEditMode{ false };
	};
}