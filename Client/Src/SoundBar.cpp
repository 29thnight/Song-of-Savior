#include <CoreManager.h>
#include <BitmapComponent.h>
#include <SoundManager.h>
#include "SoundBar.h"

void Client::SoundBar::BeginPlay()
{
	Base::BeginPlay();

	_pCurrVolumeBar = AddComponent<Engine::BitmapComponent>("FullHPBitmap");
	_pCurrVolumeBar->SetTextures(&_vecTextures);
	_pCurrVolumeBar->SetBitmapIndex(0);
	_pCurrVolumeBar->SetOrder(1);
	_pCurrVolumeBar->SetVisible(false);

	_pBaseVolumeBar = AddComponent<Engine::BitmapComponent>("CurrHPBitmap");
	_pBaseVolumeBar->SetTextures(&_vecTextures);
	_pBaseVolumeBar->SetBitmapIndex(1);
	_pBaseVolumeBar->SetVisible(false);
	
	_pCurrVolumeBar->SetCenterAlignment(false);
	_pCurrVolumeBar->SetTextureClipping(true);
	_pBaseVolumeBar->SetCenterAlignment(false);

	_volumeBarRect = _pCurrVolumeBar->GetTextureRect();

	_pBaseVolumeBar->SetRelativeLocation({ 0.f, 0.f });
	_pCurrVolumeBar->SetRelativeLocation({ 0.f, 0.f });
}

void Client::SoundBar::Tick(_duration deltaSeconds)
{
	if(!_isVisible)
	{
		return;
	}

	Base::Tick(deltaSeconds);

	// Sound->GetMasterVolume()은 0 ~ 1의 값
	float Currvolume = Sound->GetMasterVolume();
	//DebugPrintf("Currvolume : %f\n", Currvolume);
	float ScaleX = -1.f;

	// 볼륨이 1보다 클 때
	if (Currvolume > 1.0f)
	{
		Sound->SetMasterVolume(1.0f);
		Currvolume = 1.0f;
	}
	// 볼륨이 0보다 작을 때
	else if (Currvolume < 0.0f)
	{
		Sound->SetMasterVolume(0.0f);
		Currvolume = 0.0f;
	}

	ScaleX = Currvolume;  // ScaleX는 이제 0 ~ 1의 값

	// 현재 볼륨에 따라 텍스처의 크기를 조정
	Mathf::RectF currVolumeBarRect = _volumeBarRect;
	currVolumeBarRect.right = currVolumeBarRect.left + ((_volumeBarRect.right - _volumeBarRect.left) * ScaleX);
	_pCurrVolumeBar->SetTextureClippingRect(currVolumeBarRect);

}

void Client::SoundBar::Fixed()
{
}

void Client::SoundBar::EndPlay()
{
}

void Client::SoundBar::SetupInputComponent(Engine::InputComponent* inputComponent)
{
}

void Client::SoundBar::SetVisible(bool isVisible)
{
	_pCurrVolumeBar->SetVisible(isVisible);
	_pBaseVolumeBar->SetVisible(isVisible);
	_isVisible = isVisible;
}

