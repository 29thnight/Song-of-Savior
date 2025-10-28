#include <AnimationComponent.h>
#include <Actor.h>
#include <Mathf.h>
#include <Texture.h>
#include <TextureManager.h>
#include <ACollision.h>
#include <CoreDefine.h>
#include <CsvLoader.h>
#include <TimeManager.h>
#include <SoundManager.h>
#include <GraphicsManager.h>
#include <Animation.h>

#undef min
#undef max

void Engine::AnimationComponent::TickComponent(_duration deltaSeconds)
{
	SceneComponent::TickComponent(deltaSeconds);

	if (_isVisible == false) { return; }
	if (0 == _currentFrame) { _isFrameEnd = false; }
	if(_isEditMode)
	{
		if (_prevFrame != _currentFrame && _currentFrame == _frameCount - 1)
		{
			_currentFrame = 0;
			_isFrameEnd = true;
		}
		else
		{
			_isFrameEnd = false;
		}
	}
	else
	{
		if (Sound->GetPause(CHANNEL_BGM) || (_ldouble)0 == Sound->GetPosition(CHANNEL_BGM))
		{
			_currentFrameTime += Time->DurationToFloat(deltaSeconds) * _frameSpeed;
		}
		else
		{
			long double position = Sound->GetPosition(CHANNEL_BGM);
			long double cast = 1 / 1e9;
			float result = static_cast<float>(std::abs(position - _prevPosition) * cast);
			_prevPosition = position;
			_currentFrameTime += result * _frameSpeed;
		}

		if (_frameTime == 0)
			return;
	
		while (_currentFrameTime >= _frameTime)
		{
			_currentFrameTime -= _frameTime;
			_prevFrame = _currentFrame;

			if (_isLoop)
			{
				_currentFrame = (_currentFrame + 1) % _frameCount;
			}
			else
			{
				_currentFrame = std::min(_currentFrame + 1, _frameCount - 1);
			}
		}

		if (_prevFrame != _currentFrame && _currentFrame == _frameCount - 1)
		{
			_isFrameEnd = true;
		}
		else
		{
			_isFrameEnd = false;
		}

		if (_fadeSetting.isReveal)
		{
			_fadeSetting.currentTime += Time->DurationToFloat(deltaSeconds);
			_opacity = Mathf::Lerp(0.f, _fadeSetting.targetOpacity,
				(_fadeSetting.currentTime / _fadeSetting.duration));

			if (_fadeSetting.currentTime >= _fadeSetting.duration)
			{
				_fadeSetting.isReveal = false;
				_fadeSetting.currentTime = 0;
			}
		}

		if (_fadeSetting.isConceal)
		{
			_fadeSetting.currentTime += Time->DurationToFloat(deltaSeconds);
			_opacity = Mathf::Lerp(1.f, _fadeSetting.targetOpacity,
				(_fadeSetting.currentTime / _fadeSetting.duration));

			if (_fadeSetting.currentTime >= _fadeSetting.duration)
			{
				_fadeSetting.isConceal = false;
				_fadeSetting.currentTime = 0;
			}
		}

		if (_alphaMaskSetting.isAlphaMaskEffect && -1 != _alphaMaskSetting.duration)
		{
			_alphaMaskSetting.currentTime += Time->DurationToFloat(deltaSeconds);

			if (_alphaMaskSetting.currentTime >= _alphaMaskSetting.duration)
			{
				_alphaMaskSetting.isAlphaMaskEffect = false;
				_alphaMaskSetting.currentTime = 0;
			}
		}
	}
}

void Engine::AnimationComponent::Render(_RenderTarget pRenderTarget)
{
	if (_isVisible == false) { return; }
	if (_owner->IsDestroyMarked()){ return; }
	if (_vecClips.empty()) { return; }

	Animation* pAnimation{};
	if(_owner)
	{
		pAnimation = GetOwner()->GetAnimation(_currentClipIndex);
	}
	else
	{
		pAnimation = _pAnimation;
	}

	SetAnimationRect(pAnimation);

	SetBitmapLocalTransform();

	Mathf::Matx3F flipMatrix = Matx::Identity;

	if (_isFlip)
	{
		flipMatrix = Mathf::Scale(Mathf::Vector2{-1.f, 1.f}, 
			Mathf::Point2F{_textureRect.right * 0.5f, _textureRect.bottom * 0.5f}) * _localTransform;
	}
	else
	{
		flipMatrix = _localTransform;
	}

	Mathf::Matx3F Transform = flipMatrix * _WorldTransform * _cameraMatrix;

	pRenderTarget->SetTransform(Transform);
	pRenderTarget->DrawBitmap((*pAnimation)[_currentFrame], nullptr, _opacity, D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR);

	if (_alphaMaskSetting.isAlphaMaskEffect)
	{
		_pBrush->SetColor(_maskColor);
		_pBrush->SetOpacity(_alphaMaskSetting.opacity);
		pRenderTarget->FillOpacityMask(
			(*pAnimation)[_currentFrame],
			_pBrush, 
			D2D1_OPACITY_MASK_CONTENT_GRAPHICS 	
		);
	}

	pRenderTarget->SetTransform(Matx::Identity);
}

void Engine::AnimationComponent::SetAnimationRect(Animation* pAnimation)
{
	Mathf::SizeF size = pAnimation->GetCanvasSize();

	_textureRect = Mathf::RectF{ 0, 0, size.width, size.height };
}

void Engine::AnimationComponent::AllAddClipThisActor()
{
	string ownerName = GetOwner()->GetName();
	_pwstring convertOwnerName = ownerName;

	for (const auto& [tag, animation] : TextureMgr->GetAnimations())
	{
		if (tag.find(convertOwnerName) != std::string::npos)
		{
			std::wstring clipName = tag.substr(tag.find_last_of(L"/") + 1);
			string convertClipName = clipName.c_str();
			AddClip(convertClipName);
		}
	}
}

void Engine::AnimationComponent::AddClip(_pstring clipName)
{
	if(!_isInLayer)
	{
		AddRenderQueueInLayer();
		_isInLayer = true;
	}

	string convertOwnerName = (string)"Assets/" + GetOwner()->GetName();
	string convertName = clipName;
	string textureName = convertOwnerName + "/" + convertName;

	Animation* pAnimation = TextureMgr->FindAnimation(textureName);
	AnimationClip* pClip = new AnimationClip(clipName);
	pClip->duration = (float)pAnimation->GetDuration();
	pClip->frameTime = (float)pAnimation->GetFrameRate();
	pClip->clipIndex = _indexCount++;
	pClip->maxFrame = (int)pAnimation->size();
	pClip->isLoop = pAnimation->IsLoop();

	_vecClips[clipName] = std::move(pClip);
	GetOwner()->AddAnimation(pAnimation);
}

void Engine::AnimationComponent::EditorAddClip(Animation* pAnimation, bool isLoop)
{
	_pAnimation = pAnimation;

	AnimationClip* pClip = new AnimationClip("default");
	pClip->frameTime = (float)_pAnimation->GetFrameRate();
	pClip->clipIndex = _indexCount++;
	pClip->maxFrame = (int)_pAnimation->size();

	_vecClips["default"] = std::move(pClip);
	_frameCount = pClip->maxFrame;
	_frameTime = pClip->frameTime;
	_isLoop = isLoop;

	_currentFrame = 0;
	_prevFrame = 0;
}

void Engine::AnimationComponent::InEditMode()
{
	_isEditMode = true;
	_currentFrame = 0;
	_prevFrame = 0;
	_currentFrameTime = 0;
	_isFrameEnd = false;
}

void Engine::AnimationComponent::OutEditMode()
{
	_isEditMode = false;
	_currentFrame = 0;
	_prevFrame = 0;
	_currentFrameTime = 0;
	_isFrameEnd = false;
}

bool Engine::AnimationComponent::IsEditMode() const
{
	return _isEditMode;
}

int Engine::AnimationComponent::GetCurrentFrame() const
{
	return _currentFrame;
}

double Engine::AnimationComponent::GetClipDuration(_pstring clipName)
{
	return _vecClips[clipName]->duration;
}

void Engine::AnimationComponent::EditorSetFrame(_int frame)
{
	_currentFrame = frame;
}

void Engine::AnimationComponent::EditorNextFrame()
{
	_currentFrame = (_currentFrame + 1) % _frameCount;
}

void Engine::AnimationComponent::EditorPrevFrame()
{
	_currentFrame = (_currentFrame - 1) % _frameCount;
	if (_currentFrame < 0)
	{
		_currentFrame = _frameCount - 1;
	}
}

const bool Engine::AnimationComponent::IsClipPlay(_pstring clipName) const
{
	if (_currentClipName && !strcmp(_currentClipName, clipName))
	{
		return !_isFrameEnd;
	}

	return false;
}

const bool Engine::AnimationComponent::IsClipEnd(_pstring clipName) const
{
	if (_currentClipName && !strcmp(_currentClipName, clipName))
	{
		return _isFrameEnd;
	}

	return true;
}

const bool Engine::AnimationComponent::IsFrameEnd() const
{
	return _isFrameEnd;
}

void Engine::AnimationComponent::SetPlayClip(_pstring clipName)
{
	auto findIter = _vecClips.find(clipName);
	if (findIter == _vecClips.end())
		return;

	_currentClipName = clipName;
	_currentClipIndex = _vecClips[clipName]->clipIndex;
	_frameCount = _vecClips[clipName]->maxFrame;
	_frameTime = _vecClips[clipName]->frameTime;
	_isLoop = _vecClips[clipName]->isLoop;

	Animation* pAnimation = GetOwner()->GetAnimation(_currentClipIndex);

	_currentFrame = 0;
	_prevFrame = 0;
}

void Engine::AnimationComponent::RemoveClip(_pstring clipName)
{
	_vecClips.erase(clipName);
}

bool Engine::AnimationComponent::InitializeComponent()
{
	AllAddClipThisActor();

	Graphics->AddBrush(_owner->GetName(),Color::Red);
	_pBrush = Graphics->GetBrush(_owner->GetName());

    return true;
}

void Engine::AnimationComponent::Destroy()
{
	for (auto& clip : _vecClips)
	{
		delete clip.second;
	}
	_vecClips.clear();
}

Engine::AnimationComponent* Engine::AnimationComponent::Create()
{
	return new AnimationComponent;
}
