#include <BitmapComponent.h>
#include <Mathf.h>
#include <Texture.h>
#include <ACollision.h>
#include <CoreManager.h>
#include <TimeManager.h>
#include <GraphicsManager.h>
#include <Actor.h>

void Engine::BitmapComponent::Render(_RenderTarget pRenderTarget)
{
	if (!_vecTextures) { return; }
	if (_vecTextures->empty()) { return; }
	if (!_isVisible) { return; }

	Texture* pTexture = _vecTextures->at(_currentTextureIndex);
	if (!pTexture || pTexture->Empty())
	{
		return;
	}

	SetTextureRect(pTexture);

	SetBitmapLocalTransform();

	Mathf::Matx3F flipMatrix = Matx::Identity;
	switch (_isCenterAlignment)
	{
	case true:
		if (_isFlip)
		{
			flipMatrix = Mathf::Scale({-1.f, 1.f}, 
				Mathf::Point2F{_textureRect.right * 0.5f, _textureRect.bottom * 0.5f}) * _localTransform;
		}
		else
		{
			flipMatrix = _localTransform;
		}
		break;
	case false:
		if (_isFlip)
		{
			flipMatrix = Mathf::Scale(Mathf::Vector2{-1.f, 1.f}, 
				Mathf::Point2F{_textureRect.right * 0.5f, _textureRect.bottom * 0.5f});
		}
		else
		{
			flipMatrix = Matx::Identity;
		}
		break;
	}

	
	Mathf::Matx3F Transform = flipMatrix * _WorldTransform * _cameraMatrix;

	pRenderTarget->SetTransform(Transform);

	switch (_isTextureClipping)
	{
	case true:

		pRenderTarget->PushAxisAlignedClip(_textureClippingRect, D2D1_ANTIALIAS_MODE_ALIASED);
		pRenderTarget->DrawBitmap((*pTexture)[_currentBitmapIndex], NULL, _opacity);
		pRenderTarget->PopAxisAlignedClip();

		break;
	case false:

		pRenderTarget->DrawBitmap((*pTexture)[_currentBitmapIndex], NULL, _opacity);

		break;
	}

	if (_alphaMaskSetting.isAlphaMaskEffect)
	{
		_pBrush->SetColor(_maskColor);
		_pBrush->SetOpacity(_alphaMaskSetting.opacity);
		pRenderTarget->FillOpacityMask(
			(*pTexture)[_currentBitmapIndex],
			_pBrush, 
			D2D1_OPACITY_MASK_CONTENT_TEXT_NATURAL 			
		);
	}

	pRenderTarget->SetTransform(Matx::Identity);
}

void Engine::BitmapComponent::SerializeIn(nlohmann::ordered_json& object)
{
	using namespace nlohmann;

	std::vector<float> Vector2 = object["RelativeLocation"].get<std::vector<float>>();
	_RelativeLocation.x = Vector2[0];
	_RelativeLocation.y = Vector2[1];

	Vector2 = object["RelativeScale"].get<std::vector<float>>();
	_RelativeScale.x = Vector2[0];
	_RelativeScale.y = Vector2[1];

	_RelativeRotation = object["RelativeRotation"];

	_opacity = object["opacity"];
	_currentTextureIndex = object["currentTextureIndex"];
	_currentBitmapIndex = object["currentBitmapIndex"];
	_isFlip = object["isFlip"];
}

void Engine::BitmapComponent::SerializeOut(nlohmann::ordered_json& object)
{
	using namespace nlohmann;
	ordered_json componentJsonObject;
	std::string classType = typeid(*this).name();
	size_t namePos = classType.find("class") + 6;
	classType = classType.substr(namePos);
	componentJsonObject["Component Class Type"] = classType;

	componentJsonObject["RelativeLocation"] = { _RelativeLocation.x, _RelativeLocation.y };
	componentJsonObject["RelativeScale"] = { _RelativeScale.x, _RelativeScale.y };
	componentJsonObject["RelativeRotation"] = _RelativeRotation;

	componentJsonObject["opacity"] = _opacity;
	componentJsonObject["currentTextureIndex"] = _currentTextureIndex;
	componentJsonObject["currentBitmapIndex"] = _currentBitmapIndex;
	componentJsonObject["isFlip"] = _isFlip;

	object[GetName()] = componentJsonObject;
}

void Engine::BitmapComponent::SetTextures(BitmapTextures* vecTextures)
{
	if (!vecTextures->back()) { return; }

	_vecTextures = vecTextures;

	if(!_isAddLayer)
	{
		BitmapComponent::AddRenderQueueInLayer();
	}
}

void Engine::BitmapComponent::SetTextureRect(Texture* pTexture)
{
	Mathf::SizeF _size = (*pTexture)[_currentBitmapIndex]->GetSize();

	_textureRect = D2D1::RectF(0, 0, _size.width, _size.height);

	//_pCollision->SetCollisionSize({ _textureRect.right * _RelativeScale.x, _textureRect.bottom * _RelativeScale.y });
}

Mathf::RectF Engine::BitmapComponent::GetTextureRect()
{
	if (!_vecTextures) { return Mathf::RectF(); }
	if (0 == _textureRect.right || 0 == _textureRect.bottom) 
	{ 
		Texture* pTexture = _vecTextures->at(_currentTextureIndex);
		SetTextureRect(pTexture); 
	}

	return _textureRect;
}

void Engine::BitmapComponent::RemoveRenderQueueInLayer()
{
	_isAddLayer = false;
	RenderComponent::RemoveRenderQueueInLayer();
}

void Engine::BitmapComponent::AddRenderQueueInLayer()
{
	_isAddLayer = true;
	RenderComponent::AddRenderQueueInLayer();
}


void Engine::BitmapComponent::SetBitmapLocalTransform()
{
	_localLocation.x = _textureRect.right * 0.5f;
	_localLocation.y = _textureRect.bottom * 0.5f;

	_localTransform = Mathf::Translation(Mathf::Vector2{-_localLocation.x, -_localLocation.y});
}

void Engine::BitmapComponent::StartReveal(float duration, float targetOpacity)
{
	_fadeSetting.currentTime = 0;
	_fadeSetting.duration = duration;
	_fadeSetting.targetOpacity = targetOpacity;
	_fadeSetting.isReveal = true;
	_fadeSetting.isConceal = false;
}

void Engine::BitmapComponent::StartConceal(float duration, float targetOpacity)
{
	_fadeSetting.currentTime = 0;
	_fadeSetting.duration = duration;
	_fadeSetting.targetOpacity = targetOpacity;
	_fadeSetting.isReveal = false;
	_fadeSetting.isConceal = true;
}

void Engine::BitmapComponent::StartConcealAndReveal(float duration, int targetCount)
{
	_fadeSetting.currentTime = 0;
	_fadeSetting.duration = duration;
	_fadeSetting.targetOpacity = 0.f;
	_fadeSetting.isReveal = true;
	_fadeSetting.isConceal = true;
	_revealAndConcealCount = targetCount;
}

void Engine::BitmapComponent::StopConcealAndReveal()
{
	_opacity = 1.f;
	_fadeSetting.isReveal = false;
	_fadeSetting.isConceal = false;
	_fadeSetting.currentTime = 0;
	_revealAndConcealCount = 0;
}

void Engine::BitmapComponent::SetAlphaMaskEffect(bool isAlphaMaskEffect)
{
	if (0.f < _alphaMaskSetting.currentTime)
	{
		_alphaMaskSetting.currentTime = 0;
	}

	_alphaMaskSetting.isAlphaMaskEffect = isAlphaMaskEffect;
}

void Engine::BitmapComponent::SetMaskColor(const Color& color)
{
	_maskColor = color;
}

bool Engine::BitmapComponent::InitializeComponent()
{
	Graphics->AddBrush(_owner->GetName(),Color::Red);
	_pBrush = Graphics->GetBrush(_owner->GetName());

    return true;
}

void Engine::BitmapComponent::TickComponent(_duration deltaSeconds)
{
	if (!_isVisible) { return; }

	if(_fadeSetting.isConceal && _fadeSetting.isReveal && 0 != _revealAndConcealCount)
	{
		_fadeSetting.currentTime += Time->DurationToFloat(deltaSeconds);
		if (_fadeSetting.currentTime < _fadeSetting.duration * 0.5f)
		{
			_opacity = Mathf::Lerp(1.f, 0.f,
				(_fadeSetting.currentTime / (_fadeSetting.duration * 0.5f)));
		}
		else if (_fadeSetting.currentTime >= _fadeSetting.duration * 0.5f)
		{
			_opacity = Mathf::Lerp(0.f, 1.f,
				(_fadeSetting.currentTime / _fadeSetting.duration));
		}

		if (_fadeSetting.currentTime >= _fadeSetting.duration)
		{
			_fadeSetting.currentTime = 0;

			--_revealAndConcealCount;

			if(0 == _revealAndConcealCount)
			{
				_fadeSetting.isReveal = false;
				_fadeSetting.isConceal = false;
			}
		}

	}
	else if (_fadeSetting.isReveal || _fadeSetting.isConceal)
	{
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
	}

	if (_alphaMaskSetting.isAlphaMaskEffect && -1 != _alphaMaskSetting.duration)
	{
		_alphaMaskSetting.currentTime += Time->DurationToFloat(deltaSeconds);

		if (_alphaMaskSetting.currentTime >= _alphaMaskSetting.duration)
		{
			_alphaMaskSetting.isAlphaMaskEffect = false;
			_alphaMaskSetting.currentTime -= _alphaMaskSetting.duration;
		}
	}

	Super::TickComponent(deltaSeconds);
}

void Engine::BitmapComponent::Destroy()
{
	//SafeDelete(_pCollision);
	if (_isAddLayer)
	{
		RemoveRenderQueueInLayer();
		_isAddLayer = false;
	}
}

Engine::BitmapComponent* Engine::BitmapComponent::Create()
{
	return new BitmapComponent;
}
