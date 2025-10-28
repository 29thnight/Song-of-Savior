#pragma once
#include <BitmapComponent.h>

namespace Engine
{
	CORECLASS();
}

namespace Client
{
	class HPComponent final : public Engine::BitmapComponent
	{
	private:
		explicit HPComponent() = default;
		virtual ~HPComponent() = default;

	public:
		static HPComponent* Create() { return new HPComponent; }
		void Destroy() override;

	public:
		void FindTexture();
		bool InitializeComponent() override;
		void TickComponent(_duration deltaSeconds) override;
		void Render(_RenderTarget pRenderTarget) override;

	public:
		void ReInitializeComponent();
		void SetHPIndexBlink(int willBeDecreseHP);
		void CancelHPIndexBlink();

	private:
		std::vector<Mathf::Matx3F>	_BackgroundRelativeTransforms{};
		std::vector<Mathf::Matx3F>	_HpRelativeTransforms{};
		std::vector<Mathf::Matx3F>	_MpRelativeTransforms{};

	private:
		std::vector<SmartPtr<ID2D1Bitmap>>	_HpBitmaps{};
		std::vector<SmartPtr<ID2D1Bitmap>>	_MpBitmaps{};
		ID2D1Bitmap*						_pBackgroundBitmap{};

	private:
		int _warningSignStartHP{};

	private:
		float _HPScale{};
		float _MPScale{};

	private:
		int _maxHP{};
		int _maxMP{};
		int _currentHP{};
		int _currentMP{};

	private:
		bool _isHPIndexBlink{ false };

	private:
		Mathf::SizeF _backgroundSize{};
		Mathf::SizeF _HPSize{};
		Mathf::SizeF _MPSize{};
	};
};