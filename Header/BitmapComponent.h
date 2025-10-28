#pragma once
#include <RenderComponent.h>
#include <Mathf.h>

namespace Engine
{
	CORECLASS()
	class BitmapComponent : public RenderComponent
	{
	protected:
		using BitmapTextures = std::vector<Texture*>;

	protected:
		explicit BitmapComponent() = default;
		virtual ~BitmapComponent() = default;

	public:
		struct FadeSetting
		{
			float	duration{};
			float	currentTime{};
			bool	isReveal{ false };
			bool	isConceal{ false };
			float	targetOpacity{};
		};

		struct AlphaMaskSetting
		{
			bool	isAlphaMaskEffect{ false };
			float	opacity{};
			float   duration{ -1.f };
			float	currentTime{};
		};

	public:
		virtual bool InitializeComponent() override;
		virtual void TickComponent(_duration deltaSeconds) override;
		virtual void Render(_RenderTarget pRenderTarget) override;

	public:
		virtual void SerializeIn(nlohmann::ordered_json& object);
		virtual void SerializeOut(nlohmann::ordered_json& object);

	public:
		void SetTextures(BitmapTextures* vecTextures);
		void SetTextureIndex(int index) { _currentTextureIndex = index; }
		void SetTextureRect(Texture* pTexture);
		Mathf::RectF GetTextureRect();

	public:
		void SetTextureClipping(bool isTextureClipping) { _isTextureClipping = isTextureClipping; }
		bool IsTextureClipping() const { return _isTextureClipping; }
		void SetTextureClippingRect(const Mathf::RectF& textureClippingRect) { _textureClippingRect = textureClippingRect; }

	private:
		using RenderComponent::AddRenderQueueInLayer;
		using RenderComponent::RemoveRenderQueueInLayer;

	public:
		void RemoveRenderQueueInLayer();
		void AddRenderQueueInLayer();

	public:
		void SetBitmapLocalTransform();
		void SetBitmapIndex(int index) { _currentBitmapIndex = index; }
		int GetBitmapIndex() const { return _currentBitmapIndex; }
		void SetCenterAlignment(bool isCenterAlignment) { _isCenterAlignment = isCenterAlignment; }
		bool IsCenterAlignment() const { return _isCenterAlignment; }

	public:
		void SetOpacity(float opacity) { _opacity = opacity; }
		float GetOpacity() const { return _opacity; }
		void StartReveal(float duration, float targetOpacity = 1.f);
		void StartConceal(float duration, float targetOpacity = 0.f);
		void StartConcealAndReveal(float duration, int targetCount = 1);
		void StopConcealAndReveal();
		bool IsReveal() const { return _fadeSetting.isReveal; }
		bool IsConceal() const { return _fadeSetting.isConceal; }

	public:
		void SetAlphaMaskEffect(bool isAlphaMaskEffect);
		void SetMaskColor(const Color& color);
		bool IsAlphaMaskEffect() const { return _alphaMaskSetting.isAlphaMaskEffect; }
		void SetAlphaMaskDuration(float duration) { _alphaMaskSetting.duration = duration; }
		void SetAlphaMaskOpacity(float opacity) { _alphaMaskSetting.opacity = opacity; }

	public:
		void SetFlip(bool isFlip) { _isFlip = isFlip; }
		bool IsFlip() const { return _isFlip; }

	protected:
		virtual void Destroy() override;

	public:
		static BitmapComponent* Create();

	protected:
		ID2D1SolidColorBrush*	_pBrush{ nullptr };
		BitmapTextures*			_vecTextures{ nullptr };
		AlphaMaskSetting		_alphaMaskSetting{};
		FadeSetting				_fadeSetting{};
		Color					_maskColor{ Color::Red };

	protected:	
		Mathf::RectF		_textureRect{};
		Mathf::RectF		_textureClippingRect{};
		Mathf::Matx3F		_localTransform{ Matx::Identity };
		Mathf::Vector2		_localScale{ UnitVector::Zero };
		Mathf::Vector2		_localLocation{ UnitVector::Zero };

	protected:
		float				_opacity{ 1.f };
		int					_currentTextureIndex{ 0 };
		int					_currentBitmapIndex{ 0 };
		int					_revealAndConcealCount{ 0 };

	protected:
		bool 				_isFlip{ false };
		bool				_isCenterAlignment{ true };
		bool				_isTextureClipping{ false };
		bool				_isAddLayer{ false };
		bool				_isAlphaMaskEffect{ false };
	};
}