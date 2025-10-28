#pragma once
#include <RenderComponent.h>

namespace Engine
{
	CORECLASS()
	class LineRenderComponent : public RenderComponent
	{
	protected:
		explicit LineRenderComponent() = default;
		virtual ~LineRenderComponent() override = default;

	public:
		static LineRenderComponent* Create() { return new LineRenderComponent; }

	public:
		virtual void Render(_RenderTarget pRenderTarget) override;

	public:
		virtual void SerializeIn(nlohmann::ordered_json& object) override {};
		virtual void SerializeOut(nlohmann::ordered_json& object) override {};

	public:
		virtual bool InitializeComponent() override;
		virtual void Destroy() override;   

	public:
		float Length{30.f}; //�� ����
		float Width{1.0f}; //�� �β�
		D2D1::ColorF Color = D2D1::ColorF::White; //��
		
	private:
		ID2D1SolidColorBrush* _pBrush = nullptr;
	};
}