#pragma once
#include <CoreDefine.h>
#include <CollisionComponent.h>

namespace Engine
{
	class LineColliderComponent : public CollisionComponent
	{
	protected:
		explicit LineColliderComponent() = default;
		virtual ~LineColliderComponent() = default;

	public:
		virtual bool InitializeComponent() override;
		virtual void TickComponent(_duration deltaSeconds) override;
		virtual bool IsCollision(CollisionComponent* pOther) override;
		virtual void Render(_RenderTarget pRenderTarget) override; //for debug draw

	public:
		virtual void SerializeIn(nlohmann::ordered_json& object) {};
		virtual void SerializeOut(nlohmann::ordered_json& object) {};

	public:
		void SetAddOffset(const Mathf::Vector2& offsetVector);
		void SetSize(const Mathf::Vector2& sizeVector);
		RCollision* GetCollision() const { return _pCollision; }

	public:
		void Destroy() override;

	public:
		static LineColliderComponent* Create();

	protected:
		RCollision* _pCollision{ nullptr };
	};
}