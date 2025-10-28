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

	// Sound->GetMasterVolume()�� 0 ~ 1�� ��
	float Currvolume = Sound->GetMasterVolume();
	//DebugPrintf("Currvolume : %f\n", Currvolume);
	float ScaleX = -1.f;

	// ������ 1���� Ŭ ��
	if (Currvolume > 1.0f)
	{
		Sound->SetMasterVolume(1.0f);
		Currvolume = 1.0f;
	}
	// ������ 0���� ���� ��
	else if (Currvolume < 0.0f)
	{
		Sound->SetMasterVolume(0.0f);
		Currvolume = 0.0f;
	}

	ScaleX = Currvolume;  // ScaleX�� ���� 0 ~ 1�� ��

	// ���� ������ ���� �ؽ�ó�� ũ�⸦ ����
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

