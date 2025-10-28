#include <GraphicsManager.h>
#include <FontManager.h>
#include <TextRenderComponent.h>
#include <FontManager.h>

void Engine::TextRenderComponent::TickComponent(_duration deltaSeconds)
{
	SceneComponent::TickComponent(deltaSeconds);
}

void Engine::TextRenderComponent::Render(_RenderTarget pRenderTarget)
{
	if (!_font)
		return;

	if (!_isVisible)
		return;

	_pBrush->SetColor(_color);
	Mathf::Matx3F Transform = _WorldTransform * _cameraMatrix;
	pRenderTarget->SetTransform(Transform);
	if(0 != _text.length())
	{	
		_textLayout->SetFontSize(_fontSize, _textRange);

		pRenderTarget->DrawTextLayout({ _textRect.left, _textRect.top }, _textLayout.get(), _pBrush);
	}

//#ifdef _DEBUG
//	pRenderTarget->DrawRectangle({_textRect.left, _textRect.top, _textRect.right, _textRect.bottom}, _pBrush, 3.0f);
//#endif // _DEBUG

}

void Engine::TextRenderComponent::SetText(string text)
{
	//if (_text.length() != text.length())
	//{
		_text = text;
		if (_textLayout.get() != NULL)
		{
			_textLayout.Reset();
		}

		FontMgr->GetDWriteFactory()->CreateTextLayout(
			_text, _text.length(),
			_font, _textRect.right - _textRect.left,
			_textRect.bottom - _textRect.top,
			_textLayout.GetAddressOf());
		_textRange.length = _text.length();
	//}
}

void Engine::TextRenderComponent::SetFont(_pwstring fontName)
{
	_font = FontMgr->GetFont(fontName);
}

void Engine::TextRenderComponent::SetSize(const Mathf::Vector2& size)
{
	Mathf::Vector2 rectHalfSize = size * 0.5f;
	Mathf::Vector2 position = GetWorldLocation();

	_textRect.left	 = position.x - rectHalfSize.x;
	_textRect.top	 = position.y - rectHalfSize.y;
	_textRect.right  = position.x + rectHalfSize.x;
	_textRect.bottom = position.x + rectHalfSize.y;
}

void Engine::TextRenderComponent::ChooseBrush(_pstring brushName)
{
	_pBrush = Graphics->GetBrush(brushName);
}

void Engine::TextRenderComponent::SetColor(const Color& color)
{
	if(!_pBrush)
		return;

	_color = color;
}

void Engine::TextRenderComponent::SetHorizontalAlignment(DWRITE_TEXT_ALIGNMENT alignment)
{
	_font->SetTextAlignment(alignment);
}

void Engine::TextRenderComponent::SetVerticalAlignment(DWRITE_PARAGRAPH_ALIGNMENT alignment)
{
	_font->SetParagraphAlignment(alignment);
}

Engine::TextRenderComponent* Engine::TextRenderComponent::Create()
{
	return new TextRenderComponent();
}

bool Engine::TextRenderComponent::InitializeComponent()
{
	AddRenderQueueInLayer();

	return true;
}

void Engine::TextRenderComponent::Destroy()
{
}
