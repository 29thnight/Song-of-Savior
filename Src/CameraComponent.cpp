#include <CameraComponent.h>
#include <RenderComponent.h>
#include <GraphicsManager.h>
#include <TimeManager.h>
#include <CoreManager.h>
#include <ACollision.h>
#include <Random.h>

void Engine::CameraComponent::TickComponent(_duration deltaSeconds)
{
	SceneComponent::TickComponent(deltaSeconds);

	float Width = (float)Management->setting.width;
	float Height = (float)Management->setting.height;

	float halfWidth = Width * 0.5f;
	float halfHeight = Height * 0.5f;

	Mathf::Vector2 CollisionPosition = Mathf::Vector2{ _RelativeLocation.x, _RelativeLocation.y };

	//_pCollision->SetCollisionSize({ Width, Height });
	//_pCollision->SetCollisionOffset(CollisionPosition);

	if (_fadeSetting.isFadeOut)
	{
		_fadeSetting.currentTime += Time->DurationToFloat(deltaSeconds);
		_fadeSetting.opacity = Mathf::Lerp(0.f, 1.f, 
			(_fadeSetting.currentTime / _fadeSetting.fadeOutDuration));

		if (_fadeSetting.currentTime >= _fadeSetting.fadeOutDuration)
		{
			_fadeSetting.isFadeOut = false;
			_fadeSetting.currentTime = 0;
		}
	}
	else if (_fadeSetting.isFadeIn)
	{
		_fadeSetting.currentTime += Time->DurationToFloat(deltaSeconds);
		_fadeSetting.opacity = Mathf::Lerp(1.f, 0.f,
			(_fadeSetting.currentTime / _fadeSetting.fadeInDuration));

		if (_fadeSetting.currentTime >= _fadeSetting.fadeInDuration)
		{
			_fadeSetting.isFadeIn = false;
			_fadeSetting.currentTime = 0;
		}
	}

	if (_cameraShakeSetting.isShake)
	{
		_cameraShakeSetting.currentTime += Time->DurationToFloat(deltaSeconds);
		_cameraShakeSetting.cycleTime += Time->DurationToFloat(deltaSeconds);
		if(_cameraShakeSetting.cycleTime >= _cameraShakeSetting.cycleTick)
		{
			float angle = _cameraShakeSetting.shakeOffset[_cameraShakeSetting.shakeIndex++];
			SetRelativeLocation(Mathf::Vector2{ 
				Mathf::Sin(angle) * _cameraShakeSetting.magnitude, 
				Mathf::Cos(angle) * _cameraShakeSetting.magnitude 
				});
			_cameraShakeSetting.cycleTime -= _cameraShakeSetting.cycleTick;
		}

		if (_cameraShakeSetting.shakeIndex >= _cameraShakeSetting.shakeOffset.size())
		{
			_cameraShakeSetting.shakeIndex = 0;
		}

		if (_cameraShakeSetting.currentTime >= _cameraShakeSetting.duration)
		{
			_cameraShakeSetting.isShake = false;
			_cameraShakeSetting.currentTime = 0;
			SetRelativeLocation(UnitVector::Zero);
		}
	}
}

void Engine::CameraComponent::Render(_RenderTarget pRenderTarget)
{
	pRenderTarget->SetTransform(Matx::Identity);
	_pBrush->SetOpacity(_fadeSetting.opacity);
	pRenderTarget->FillRectangle(
		D2D1::RectF(
			0, 
			0, 
			(float)Management->setting.width, 
			(float)Management->setting.height), 
		_pBrush);
}

bool Engine::CameraComponent::CheckCollision(ACollision* pCollision)
{
	return _pCollision->CheckCollision(pCollision);
}

void Engine::CameraComponent::CameraShake(float duration, float magnitude, float cycleTick, int randomAngle)
{
	_cameraShakeSetting.duration = duration;
	_cameraShakeSetting.magnitude = magnitude;
	_cameraShakeSetting.cycleTick = cycleTick;
	_cameraShakeSetting.cycleTime = 0;
	_cameraShakeSetting.currentTime = 0;
	_cameraShakeSetting.shakeOffset = _random.Generate(randomAngle);
	_cameraShakeSetting.isShake = true;
}

void Engine::CameraComponent::StartFadeIn(float duration)
{
	_fadeSetting.isFadeIn = true;
	_fadeSetting.isFadeOut = false;
	_fadeSetting.fadeInDuration = duration;
	_fadeSetting.currentTime = 0;
}

void Engine::CameraComponent::StartFadeOut(float duration)
{
	_fadeSetting.isFadeOut = true;
	_fadeSetting.isFadeIn = false;
	_fadeSetting.fadeOutDuration = duration;
	_fadeSetting.currentTime = 0;
}

void Engine::CameraComponent::StartFadeOutIn(float duration, Color color)
{
	float halfDuration = duration * 0.5f;
	_fadeSetting.isFadeOut = true;
	_fadeSetting.isFadeIn = true;
	_fadeSetting.fadeOutDuration = halfDuration;
	_fadeSetting.fadeInDuration = halfDuration;
	_fadeSetting.currentTime = 0;
	_pBrush->SetColor(color);
}

void Engine::CameraComponent::SetColor(Color color)
{
	_pBrush->SetColor(color);
}

void Engine::CameraComponent::EndFade()
{
	_fadeSetting.isFadeOut = false;
	_fadeSetting.isFadeIn = false;
	_fadeSetting.currentTime = 0;
	_fadeSetting.opacity = 0;
}

bool Engine::CameraComponent::InitializeComponent()
{
	/*_pCollision = ACollision::Create();*/
	AddRenderQueueInLayer(LAYER::UI);
	Graphics->AddBrush("CameraFade",Color::Black);
	_pBrush = Graphics->GetBrush("CameraFade");

	return true;
}

void Engine::CameraComponent::Destroy()
{
	/*SafeDelete(_pCollision);*/
}

Engine::CameraComponent* Engine::CameraComponent::Create()
{
	return new CameraComponent();
}
