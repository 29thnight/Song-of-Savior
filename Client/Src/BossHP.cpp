#include "BossHP.h"
#include <BitmapComponent.h>
#include <CoreManager.h>
#include <SoundManager.h>
#include <TextRenderComponent.h>

#include "BossManager.h"
#include "GameManager.h"

constexpr float yDamp = 1025.f;

void Client::BossHP::BeginPlay()
{
	Base::BeginPlay();

	_pFullHPBitmap = AddComponent<Engine::BitmapComponent>("FullHPBitmap");
	_pFullHPBitmap->SetTextures(&_vecTextures);
	_pFullHPBitmap->SetBitmapIndex(0);

	_pCurrHPBitmap = AddComponent<Engine::BitmapComponent>("CurrHPBitmap");
	_pCurrHPBitmap->SetTextures(&_vecTextures);
	_pCurrHPBitmap->SetBitmapIndex(1);

	_pBossHeadBitmap = AddComponent<Engine::BitmapComponent>("BossHeadBitmap");
	_pBossHeadBitmap->SetTextures(&_vecTextures);
	_pBossHeadBitmap->SetBitmapIndex(2);

	_pCurrHPBitmap->SetRelativeLocation({Management->setting.width * 0.5f, GetBitmapSize(0).y * 0.5f + yDamp});
	_pFullHPBitmap->SetRelativeLocation({ Management->setting.width * 0.5f, GetBitmapSize(0).y * 0.5f + yDamp});
	_pBossHeadBitmap->SetRelativeLocation({ Management->setting.width * 0.5f - GetBitmapSize(0).x * 0.5f, GetBitmapSize(0).y * 0.5f + yDamp});

	_pIndicatorBitmap = AddComponent<Engine::BitmapComponent>("IndicatorBitmap");
	_pIndicatorBitmap->SetTextures(&_vecTextures);
	_pIndicatorBitmap->SetBitmapIndex(6);
	_pIndicatorBitmap->SetRelativeLocation({ 960.f , 215.f });

	_pIndicatorText = AddComponent<Engine::TextRenderComponent>("IndicatorText");
	_pIndicatorText->SetRelativeLocation({ 1050.f , 215.f });
	_pIndicatorText->ChooseBrush("Default");
	_pIndicatorText->SetColor(Color::Red);
	_pIndicatorText->SetFont(L"MapoGoldenPier");
	_pIndicatorText->SetFontSize(65.f);
	_pIndicatorText->SetSize({ 50.f, 50.f });
	_pIndicatorText->SetHorizontalAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
	_pIndicatorText->SetVerticalAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
}

void Client::BossHP::Tick(_duration deltaSeconds)
{
	Base::Tick(deltaSeconds);

	_pIndicatorBitmap->SetRelativeRotation(_randomAngle.Generate());
}

void Client::BossHP::Fixed()
{
}

void Client::BossHP::EndPlay()
{
}

void Client::BossHP::SetHeadBitmap()
{
	Stage currentStage = Manager->GetCurrentStage();
	switch (currentStage)
	{
	case Client::Stage::Stage1:
	case Client::Stage::Stage2:
		_pBossHeadBitmap->SetBitmapIndex(2);
		break;
	case Client::Stage::Stage3:
		_pBossHeadBitmap->SetBitmapIndex(3);
		break;
	case Client::Stage::Stage4:
		_pBossHeadBitmap->SetBitmapIndex(4);
		break;
	case Client::Stage::Stage4_2:
		_pBossHeadBitmap->SetBitmapIndex(5);
		break;
	default:
		break;
	}
}

void Client::BossHP::SetIndicatorText(int ChargingCount)
{
	std::string str = std::to_string(ChargingCount);
	_pIndicatorText->Text = str.c_str();
}

void Client::BossHP::SetVisibleIndicator(bool bVisible)
{
	_pIndicatorBitmap->SetVisible(bVisible);
	_pIndicatorText->SetVisible(bVisible);
}

void Client::BossHP::UpdateHpScale()
{
	int bossHP = _pBossManager->GetCurrBossHp();
	int bossMaxHP = _pBossManager->GetCurrBossMaxHp();
	if (bossHP != -1 && bossMaxHP != -1)
	{
		float realScaleX = (float)bossHP / (float)bossMaxHP;
		float maxScaleX = static_cast<float>(1 - Sound->GetPosition(CHANNEL_BGM) / Sound->GetLength(CHANNEL_BGM));

		if (maxScaleX > realScaleX)
			realScaleX = maxScaleX;

		_pCurrHPBitmap->SetRelativeScale({ realScaleX, 1.0f });
		_pCurrHPBitmap->SetRelativeLocation({ (Management->setting.width * 0.5f - GetBitmapSize(0).x * 0.5f) + GetBitmapSize(0).x * realScaleX * 0.5f , _pCurrHPBitmap->GetRelativeLocation().y });
	}
}

