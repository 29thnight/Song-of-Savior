#include "LineRenderComponent.h"
#include "GraphicsManager.h"

bool Engine::LineRenderComponent::InitializeComponent()
{
	Graphics->AddBrush("LineRender", Color::White);
	_pBrush = Graphics->GetBrush("LineRender");

	return true;
}


void Engine::LineRenderComponent::Render(_RenderTarget pRenderTarget)
{
	if (_isVisible)
	{
		Mathf::Matx3F Transform = _WorldTransform * _cameraMatrix;

		D2D1_POINT_2F p1{};
		p1.x = -Length * 0.5f;

		D2D1_POINT_2F p2{};
		p2.x = +Length * 0.5f;

		pRenderTarget->SetTransform(Transform);
		_pBrush->SetColor(Color);
		pRenderTarget->DrawLine(p1, p2, _pBrush, Width);
	}
}

void Engine::LineRenderComponent::Destroy()
{
	RemoveRenderQueueInLayer();
}
