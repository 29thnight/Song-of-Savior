#include "HPComponent.h"
#include "Character.h"

#include <TextureManager.h>
#include <CoreManager.h>
#include <Mathf.h>

void Client::HPComponent::Destroy()
{
	BitmapComponent::Destroy();
	_HpBitmaps.clear();
	_MpBitmaps.clear();
	_vecTextures->clear();
	delete _vecTextures;
}

void Client::HPComponent::FindTexture()
{
	_vecTextures = new BitmapTextures;
	_vecTextures->push_back(TextureMgr->FindTexture(L"Assets/HPBar"));
}

bool Client::HPComponent::InitializeComponent()
{
	FindTexture();

	_order = 3;

	Engine::Texture* pTexture = _vecTextures->at(0);
	_pBackgroundBitmap = (*pTexture)[0];
	_backgroundSize = _pBackgroundBitmap->GetSize();
	for(int i = 0; i < 2; ++i)
	{
		_BackgroundRelativeTransforms.push_back(Mathf::Scale({ 1.f, 1.f }) *
			Mathf::Rotation(0.f) *
			Mathf::Translation({ 0.f, (_backgroundSize.height + 5.f) * i }));
	}

	_maxHP = dynamic_cast<Character*>(_owner)->MaxHP;
	_HPSize = (*pTexture)[1]->GetSize();
	ID2D1Bitmap* pHPBitmap = (*pTexture)[1];
	D2D1_BITMAP_PROPERTIES HPProperties{};
	pHPBitmap->GetDpi(&HPProperties.dpiX, &HPProperties.dpiY);
	HPProperties.pixelFormat = pHPBitmap->GetPixelFormat();
	for (int i = 0; i < _maxHP; ++i)
	{
		SmartPtr<ID2D1Bitmap> newBitmap{ nullptr };
		HRESULT hr = Management->renderTarget->CreateBitmap(
			D2D1::SizeU(static_cast<UINT32>(_HPSize.width), static_cast<UINT32>(_HPSize.height)),
			nullptr, 0, // 초기 데이터 없이 생성
			&HPProperties,
			&newBitmap
		);
		if (SUCCEEDED(hr))
		{
			hr = newBitmap->CopyFromBitmap(nullptr, pHPBitmap, nullptr);
			if (SUCCEEDED(hr))
			{
				_HpBitmaps.push_back(newBitmap);
			}
		}
	}

	_maxMP = dynamic_cast<Character*>(_owner)->MaxMP;
	_MPSize = (*pTexture)[1]->GetSize();
	ID2D1Bitmap* pMPBitmap = (*pTexture)[2];
	D2D1_BITMAP_PROPERTIES MPProperties{};
	pMPBitmap->GetDpi(&MPProperties.dpiX, &MPProperties.dpiY);
	MPProperties.pixelFormat = pMPBitmap->GetPixelFormat();
	for (int i = 0; i < _maxMP; ++i)
	{
		SmartPtr<ID2D1Bitmap> newBitmap{ nullptr };
		HRESULT hr = Management->renderTarget->CreateBitmap(
			D2D1::SizeU(static_cast<UINT32>(_MPSize.width), static_cast<UINT32>(_MPSize.height)),
			nullptr, 0, // 초기 데이터 없이 생성
			&MPProperties,
			&newBitmap
		);
		if (SUCCEEDED(hr))
		{
			hr = newBitmap->CopyFromBitmap(nullptr, pMPBitmap, nullptr);
			if (SUCCEEDED(hr))
			{
				_MpBitmaps.push_back(newBitmap);
			}
		}
	}

	float _targetWidthPerHP = _HPSize.width / _maxHP;
	_HPScale = _targetWidthPerHP / _HPSize.width;
	_HpRelativeTransforms.resize(_maxHP);
	float calibrationHPPixel = _backgroundSize.width - _HPSize.width;

	float _targetWidthPerMP = _MPSize.width / _maxMP;
	_MPScale = _targetWidthPerMP / _MPSize.width;
	_MpRelativeTransforms.resize(_maxMP);
	float calibrationMPPixel = _backgroundSize.width - _MPSize.width;

	for (int i = 0, j = (_maxHP - 1); i < _maxHP; ++i, --j)
	{
		_HpRelativeTransforms[i] = Mathf::Scale({_HPScale * 0.95f, 0.97f}) *
			Mathf::Rotation(0.f) *
			Mathf::Translation({ _backgroundSize.width * 0.5f - _HPSize.width * 0.5f - _targetWidthPerHP * j - calibrationHPPixel, 0.f });
	}

	for (int i = 0, j = (_maxMP - 1); i < _maxMP; ++i, --j)
	{
		_MpRelativeTransforms[i] = Mathf::Scale({ _MPScale * 0.95f, 0.97f }) *
			Mathf::Rotation(0.f) *
			Mathf::Translation({ _backgroundSize.width * 0.5f - _MPSize.width * 0.5f - _targetWidthPerMP * j - calibrationMPPixel, _backgroundSize.height + 5.f });
	}

	AddRenderQueueInLayer();

	return true;
}

void Client::HPComponent::TickComponent(_duration deltaSeconds)
{
	BitmapComponent::TickComponent(deltaSeconds);

	Character* pCharacter = dynamic_cast<Character*>(_owner);
	if (!pCharacter) { return; }

	_currentHP = pCharacter->HP;
	_currentMP = pCharacter->MP;
}

void Client::HPComponent::Render(_RenderTarget pRenderTarget)
{
	if (!_vecTextures) { return; }
	if (_vecTextures->empty()) { return; }
	if (!_isVisible) { return; }

	_textureRect = D2D1::RectF(0, 0, _backgroundSize.width, _backgroundSize.height);

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

	for (int i = 0; i < 2; ++i)
	{
		Mathf::Matx3F Transform = flipMatrix * _WorldTransform * _cameraMatrix * _BackgroundRelativeTransforms[i];
		pRenderTarget->SetTransform(Transform);
		pRenderTarget->DrawBitmap(_pBackgroundBitmap);
	}

	Mathf::Matx3F Transform = flipMatrix * _WorldTransform * _cameraMatrix;

	pRenderTarget->SetTransform(Transform);

	Mathf::Matx3F invertTextureTransform = Mathf::Translation({_textureRect.right * 0.5f, 0.f});
	Mathf::Matx3F midCalculation = _WorldTransform * _cameraMatrix * invertTextureTransform;
	
	Mathf::RectF indexHPRect = { 0.f, 0.f, _HPSize.width, _HPSize.height };
	Mathf::RectF indexMPRect = { 0.f, 0.f, _MPSize.width, _MPSize.height };
	
	Mathf::Matx3F indexAxisHPXTransform = Mathf::Translation({ -indexHPRect.right, -indexHPRect.bottom * 0.5f });
	Mathf::Matx3F indexAxisMPXTransform = Mathf::Translation({ -indexMPRect.right, -indexMPRect.bottom * 0.5f });

	for (int i = 0; i < _currentHP; ++i)
	{
		Mathf::Matx3F indexTransform = indexAxisHPXTransform * _HpRelativeTransforms[i] * midCalculation;
		pRenderTarget->SetTransform(indexTransform);
		if(i >= _warningSignStartHP)
		{
			pRenderTarget->DrawBitmap(_HpBitmaps[i].get(), nullptr, _opacity);
		}
		else
		{
			pRenderTarget->DrawBitmap(_HpBitmaps[i].get());
		}
	}

	for (int i = 0; i < _currentMP; ++i)
	{
		Mathf::Matx3F indexTransform = indexAxisMPXTransform * _MpRelativeTransforms[i] * midCalculation;
		pRenderTarget->SetTransform(indexTransform);
		pRenderTarget->DrawBitmap(_MpBitmaps[i].get());
	}

	pRenderTarget->SetTransform(Matx::Identity);
}

void Client::HPComponent::ReInitializeComponent()
{
	_HpRelativeTransforms.clear();
	_MpRelativeTransforms.clear();
	_HpBitmaps.clear();
	_MpBitmaps.clear();

	Engine::Texture* pTexture = _vecTextures->at(0);
	_maxHP = static_cast<Character*>(_owner)->MaxHP;
	_HPSize = (*pTexture)[1]->GetSize();
	ID2D1Bitmap* pHPBitmap = (*pTexture)[1];
	D2D1_BITMAP_PROPERTIES HPProperties{};
	pHPBitmap->GetDpi(&HPProperties.dpiX, &HPProperties.dpiY);
	HPProperties.pixelFormat = pHPBitmap->GetPixelFormat();
	for (int i = 0; i < _maxHP; ++i)
	{
		SmartPtr<ID2D1Bitmap> newBitmap{ nullptr };
		HRESULT hr = Management->renderTarget->CreateBitmap(
			D2D1::SizeU(static_cast<UINT32>(_HPSize.width), static_cast<UINT32>(_HPSize.height)),
			nullptr, 0, // 초기 데이터 없이 생성
			&HPProperties,
			&newBitmap
		);
		if (SUCCEEDED(hr))
		{
			hr = newBitmap->CopyFromBitmap(nullptr, pHPBitmap, nullptr);
			if (SUCCEEDED(hr))
			{
				_HpBitmaps.push_back(newBitmap);
			}
		}
	}

	_maxMP = dynamic_cast<Character*>(_owner)->MaxMP;
	_MPSize = (*pTexture)[1]->GetSize();
	ID2D1Bitmap* pMPBitmap = (*pTexture)[2];
	D2D1_BITMAP_PROPERTIES MPProperties{};
	pMPBitmap->GetDpi(&MPProperties.dpiX, &MPProperties.dpiY);
	MPProperties.pixelFormat = pMPBitmap->GetPixelFormat();
	for (int i = 0; i < _maxMP; ++i)
	{
		SmartPtr<ID2D1Bitmap> newBitmap{ nullptr };
		HRESULT hr = Management->renderTarget->CreateBitmap(
			D2D1::SizeU(static_cast<UINT32>(_MPSize.width), static_cast<UINT32>(_MPSize.height)),
			nullptr, 0, // 초기 데이터 없이 생성
			&MPProperties,
			&newBitmap
		);
		if (SUCCEEDED(hr))
		{
			hr = newBitmap->CopyFromBitmap(nullptr, pMPBitmap, nullptr);
			if (SUCCEEDED(hr))
			{
				_MpBitmaps.push_back(newBitmap);
			}
		}
	}

	float _targetWidthPerHP = _HPSize.width / _maxHP;
	_HPScale = _targetWidthPerHP / _HPSize.width;
	_HpRelativeTransforms.resize(_maxHP);
	float calibrationHPPixel = _backgroundSize.width - _HPSize.width;

	float _targetWidthPerMP = _MPSize.width / _maxMP;
	_MPScale = _targetWidthPerMP / _MPSize.width;
	_MpRelativeTransforms.resize(_maxMP);
	float calibrationMPPixel = _backgroundSize.width - _MPSize.width;

	for (int i = 0, j = (_maxHP - 1); i < _maxHP; ++i, --j)
	{
		_HpRelativeTransforms[i] = Mathf::Scale({_HPScale * 0.97f, 0.97f}) *
			Mathf::Rotation(0.f) *
			Mathf::Translation({ _backgroundSize.width * 0.5f - _HPSize.width * 0.5f - _targetWidthPerHP * j - calibrationHPPixel, 0.f });
	}

	for (int i = 0, j = (_maxMP - 1); i < _maxMP; ++i, --j)
	{
		_MpRelativeTransforms[i] = Mathf::Scale({ _MPScale * 0.97f, 0.97f }) *
			Mathf::Rotation(0.f) *
			Mathf::Translation({ _backgroundSize.width * 0.5f - _MPSize.width * 0.5f - _targetWidthPerMP * j - calibrationMPPixel, _backgroundSize.height + 5.f });
	}
}

void Client::HPComponent::SetHPIndexBlink(int willBeDecreaseHP)
{
	if (_isHPIndexBlink) { return; }

	if (_currentHP <= willBeDecreaseHP)
	{
		_warningSignStartHP = 0;
	}
	else
	{
		_warningSignStartHP = _currentHP - willBeDecreaseHP;
	}

	StartConcealAndReveal(0.6f, 5);
	_isHPIndexBlink = true;
}

void Client::HPComponent::CancelHPIndexBlink()
{
	StopConcealAndReveal();
	_isHPIndexBlink = false;
}

