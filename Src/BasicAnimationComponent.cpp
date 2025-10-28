#include <BasicAnimationComponent.h>
#include <Actor.h>
#include <Mathf.h>
#include <Texture.h>
#include <TextureManager.h>
#include <TimeManager.h>
#include <ACollision.h>
#include <CoreDefine.h>
#include <CsvLoader.h>
#include <SoundManager.h>
#include <GraphicsManager.h>

#undef min
#undef max

void Engine::BasicAnimationComponent::TickComponent(_duration deltaSeconds)
{
	SceneComponent::TickComponent(deltaSeconds);

	//if (_isVisible == false) { return; }
	if (0 == _currentFrame) { _isFrameEnd = false; }
	
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
		if (-1 != _currentFrame)
		{
			_currentFrameTime += result * _frameSpeed;
		}
	}
	if (_frameTime == 0)  return;

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

		auto findIter = _vecFrameEvents.find(_currentClipName);
		if (findIter != _vecFrameEvents.end())
		{
			//if (_vecFrameEvents[_currentClipName].first == _currentFrame)
			//{
			//	_vecFrameEvents[_currentClipName].second.Broadcast();
			//}

			for (auto& frameEvent : _vecFrameEvents[_currentClipName])
			{
				if (frameEvent.first == _currentFrame)
				{
					frameEvent.second.Broadcast();
					break;
				}
			}
		}
	}

	if (_currentFrame == _frameCount - 1)
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

void Engine::BasicAnimationComponent::Render(_RenderTarget pRenderTarget)
{
	if (_isVisible == false) { return; }
	if (-1 == _currentClipIndex) { return; }

	Texture* pTexture = GetOwner()->GetFrame(_currentClipIndex);

	SetTextureRect(pTexture);

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

	pRenderTarget->DrawBitmap((*pTexture)[_currentFrame], nullptr, _opacity, D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR);

	if (_alphaMaskSetting.isAlphaMaskEffect)
	{
		_pBrush->SetOpacity(_alphaMaskSetting.opacity);
		pRenderTarget->FillOpacityMask(
			(*pTexture)[_currentFrame],
			_pBrush, 
			D2D1_OPACITY_MASK_CONTENT_GRAPHICS 	
		);
	}

	pRenderTarget->SetTransform(Matx::Identity);
}

void Engine::BasicAnimationComponent::AddClip(_pstring clipName, _float frameTime, bool isLoop)
{
	if(!_isInLayer)
	{
		AddRenderQueueInLayer();
		_isInLayer = true;
	}

	AnimationClip* pClip = new AnimationClip(clipName);
	pClip->frameTime = frameTime;
	pClip->clipIndex = (int)GetOwner()->GetTextureSize();
	pClip->isLoop = isLoop;

	string convertOwnerName = (string)"Assets/" + GetOwner()->GetName();
	string convertName = clipName;
	string textureName = convertOwnerName + "/" + convertName;
	
	GetOwner()->AddTexture(TextureMgr->FindTexture(textureName));

	pClip->maxFrame = GetOwner()->GetFrame(pClip->clipIndex)[0].size();

	_vecClips[clipName] = std::move(pClip);
}

void Engine::BasicAnimationComponent::AddClip(_pstring clipName, _float frameTime, bool isLoop, _pstring ownerFolderName)
{
	if (!_isInLayer)
	{
		AddRenderQueueInLayer();
		_isInLayer = true;
	}

	AnimationClip* pClip = new AnimationClip(clipName);
	pClip->frameTime = frameTime;
	pClip->clipIndex = (int)GetOwner()->GetTextureSize();
	pClip->isLoop = isLoop;

	string convertOwnerName = (string)"Assets/" + ownerFolderName;
	string convertName = clipName;
	string textureName = convertOwnerName + "/" + convertName;

	GetOwner()->AddTexture(TextureMgr->FindTexture(textureName));

	pClip->maxFrame = GetOwner()->GetFrame(pClip->clipIndex)[0].size();

	_vecClips[clipName] = std::move(pClip);
}

const bool Engine::BasicAnimationComponent::IsClipPlay(_pstring clipName) const
{
	if (_currentClipName && !strcmp(_currentClipName, clipName))
	{
		return !_isFrameEnd;
	}

	return false;
}

const bool Engine::BasicAnimationComponent::IsClipEnd(_pstring clipName) const
{
	if (_currentClipName && !strcmp(_currentClipName, clipName))
	{
		return _isFrameEnd;
	}

	return true;
}

const bool Engine::BasicAnimationComponent::IsCurrClip(_pstring clipName) const
{
	return _currentClipName && !strcmp(_currentClipName, clipName);
}

const bool Engine::BasicAnimationComponent::IsFrameEnd() const
{
	return _isFrameEnd;
}

int Engine::BasicAnimationComponent::GetClipMaxframe(_pstring clipName)
{
	auto findIter = _vecClips.find(std::string(clipName));
	if (_vecClips.end() != findIter)
	{
		int maxFrame = findIter->second->maxFrame;
		return maxFrame;
	}
	return 0;
}

float Engine::BasicAnimationComponent::GetClipTime(_pstring clipName) const
{
	auto findIter = _vecClips.find(std::string(clipName));
	if(_vecClips.end() != findIter)
	{
		float time = findIter->second->frameTime * findIter->second->maxFrame;
		return time;
	}
	return 0;
}

void Engine::BasicAnimationComponent::SetPlayClip(_pstring clipName)
{
	_currentClipName = clipName;
	_currentClipIndex = _vecClips[clipName]->clipIndex;
	_frameTime = _vecClips[clipName]->frameTime;
	_isLoop = _vecClips[clipName]->isLoop;

	Texture* pTexture = GetOwner()->GetFrame(_currentClipIndex);

	_frameCount = (int)pTexture->size();

	_currentFrameTime = 0;
	_currentFrame = 0;
	_prevFrame = 0;
	_isFrameEnd = false;	
}

void Engine::BasicAnimationComponent::RemoveClip(_pstring clipName)
{
	_vecClips.erase(clipName);
}

void Engine::BasicAnimationComponent::AllAddClipThisActor()
{
	string BasePath = (string)"Assets/" + GetOwner()->GetName();
	string convertDataName = BasePath + "/Clips.csv";
	CSVReader<std::string, float, bool> csvReader((_pstring)convertDataName);

	csvReader.forEach([&](std::string clipName, float frameTime, bool isLoop)
		{
			AddClip(clipName.c_str(), frameTime, isLoop);
		});
}

void Engine::BasicAnimationComponent::AllAddClipThisActor(string csvPath, string ownerFolderName)
{
	string BasePath = csvPath;
	CSVReader<std::string, float, bool> csvReader((_pstring)csvPath);
	csvReader.forEach([&](std::string clipName, float frameTime, bool isLoop)
		{
			AddClip(clipName.c_str(), frameTime, isLoop, ownerFolderName);
		});
}

bool Engine::BasicAnimationComponent::InitializeComponent()
{
	Graphics->AddBrush(_owner->GetName(),Color::Red);
	_pBrush = Graphics->GetBrush(_owner->GetName());

	return true;
}

void Engine::BasicAnimationComponent::Destroy()
{
	for (auto& clip : _vecClips)
	{
		delete clip.second;
	}
	_vecClips.clear();
}

Engine::BasicAnimationComponent* Engine::BasicAnimationComponent::Create()
{
	return new BasicAnimationComponent;
}