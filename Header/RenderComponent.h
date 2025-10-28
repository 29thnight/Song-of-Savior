#pragma once
#include <CoreDefine.h>
#include <SceneComponent.h>

namespace Engine
{
	class RenderComponent abstract : public SceneComponent
	{
	protected:
		explicit RenderComponent() = default;
		virtual ~RenderComponent() = default;

	public:
		virtual void Render(_RenderTarget pRenderTarget) abstract;

	public:
		void AddRenderQueueInLayer(int layer);
		void AddRenderQueueInLayer();
		void RemoveRenderQueueInLayer();
		void SetOrder(int order) { _order = order; }
		int GetOrder() const { return _order; }
		void SetVisible(bool isVisible) { _isVisible = isVisible; }
		bool IsVisible() const { return _isVisible; }
		void SetCameraMatrix(const Mathf::Matx3F& cameraMatrix);

	public:
		_Property(bool, Visible)
		_Get(Visible)
		{
			return _isVisible;
		}
		_Set(Visible)
		{
			_isVisible = value;
		}

	public:
		virtual void SerializeIn(nlohmann::ordered_json& object) {};
		virtual void SerializeOut(nlohmann::ordered_json& object) {};

	public:
		bool operator>(const RenderComponent& rhs) { return _order > rhs._order; };
		bool operator<(const RenderComponent& rhs) { return _order < rhs._order; };

	protected:
		virtual bool InitializeComponent() { return true; }
		virtual void Destroy() override abstract;

	protected:
		Mathf::Matx3F _renderMatrix{};
		Mathf::Matx3F _cameraMatrix{ Matx::Identity };
		bool _isVisible{ true };
		int  _order{ 0 };
	};
}