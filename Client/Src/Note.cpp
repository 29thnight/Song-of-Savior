#include <World.h>
#include <BitmapComponent.h>
#include <InputComponent.h>
#include <SoundManager.h>
#include <TextureManager.h>
#include <Rlottie/inc/rlottie.h>
#include "Note.h"

#undef min
#undef max


void Client::Note::BeginPlay()
{
	string convertName = (string)"Assets/Note";
	_vecTextures.push_back(TextureMgr->FindTexture(convertName));

	_pBitmapComponent = AddComponent<Engine::BitmapComponent>("BitmapComponent");
	_pBitmapComponent->SetTextures(&_vecTextures);
	_pBitmapComponent->SetVisible(false);
	_pInputComponent = AddComponent<Engine::InputComponent>("InputComponent");	
}

void Client::Note::Tick(_duration deltaSeconds)
{
	if (_pBitmapComponent->IsVisible())
	{
		long double currPos = Sound->GetPosition(CHANNEL_BGM);
		if (_ldTargetPosition >= currPos)
		{
			float scale = static_cast<float>((_ldTargetPosition - currPos) / (_ldTargetPosition - _ldStartPosition));//-tick
			float castScale = scale * (_fMaxScale - _fMinScale);
			castScale += _fMinScale;
			castScale = std::max(_fMinScale, std::min(castScale, _fMaxScale)); //clamp

			Mathf::Vector2 vScale = { castScale,castScale };
			if (_ldTargetPosition > currPos)
			{
				_pBitmapComponent->SetRelativeScale(vScale);
			}
		}
		else
		{
			float scale = static_cast<float>((_ldTargetPosition + _ldMissOffset - currPos) / (_ldMissOffset));//-tick
			_pBitmapComponent->SetRelativeScale({ scale, scale });

			if (0.f >= scale)
			{
				_pBitmapComponent->SetRelativeScale({ 0.f, 0.f });
				_pBitmapComponent->SetVisible(false);
			}
		}
	}

	Super::Tick(deltaSeconds);
}

void Client::Note::Fixed()
{
}

void Client::Note::EndPlay()
{
}

void Client::Note::SetVisible(bool visible)
{
	_pBitmapComponent->SetVisible(visible);
}

bool Client::Note::GetVisible() const
{
	return _pBitmapComponent->IsVisible();
}

Client::Note* Client::Note::Create()
{
	return new Note;
}

void Client::Note::Destroy()
{
	Super::Destroy();
}
