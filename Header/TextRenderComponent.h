#pragma once
#include <RenderComponent.h>
#include <Mathf.h>

namespace Engine
{
	class TextRenderComponent final : public RenderComponent
	{
	protected:
		explicit TextRenderComponent() = default;
		virtual ~TextRenderComponent() = default;

	public:
		virtual void TickComponent(_duration deltaSeconds) override;
		virtual void Render(_RenderTarget pRenderTarget) override;

		virtual bool InitializeComponent() override;
		virtual void Destroy() override;
	public:
		void SetText(string text);
		string GetText() { return _text; }
		__declspec(property (put = SetText, get = GetText )) string Text;

		void SetFont(_pwstring fontName);
		void SetSize(const Mathf::Vector2& size);
		void SetFontSize(_float fontSize) { _fontSize = fontSize; }
		void ChooseBrush(_pstring brushName);
		void SetColor(const Color& color);
		_pFontFormat GetFont() const { return _font; }

		void SetHorizontalAlignment(DWRITE_TEXT_ALIGNMENT alignment);
		void SetVerticalAlignment(DWRITE_PARAGRAPH_ALIGNMENT alignment);

	public:
		virtual void SerializeIn(nlohmann::ordered_json& object) {};
		virtual void SerializeOut(nlohmann::ordered_json& object) {};

	public:
		static TextRenderComponent* Create();

	private:
		Brush					_pBrush{ nullptr };
		DWRITE_TEXT_RANGE		_textRange{};
		SmartPtr<_TextLayout>	_textLayout{ nullptr };
		Mathf::RectF			_textRect{};
		Color					_color{ D2D1::ColorF::White };
		float					_fontSize{ 16.0f };

	private:
		_pFontFormat _font{ nullptr };
		string		 _text{};
	};
}