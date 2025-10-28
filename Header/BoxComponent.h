#pragma once
#include <CoreDefine.h>
#include <CollisionComponent.h>

namespace Engine
{
	class BoxComponent : public CollisionComponent
	{
	protected:
		explicit BoxComponent() = default;
		virtual ~BoxComponent() = default;

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
		ACollision* GetCollision() const { return _pCollision; }

	public:
		void Destroy() override;
	
	public:
		static BoxComponent* Create();

	protected:
		ACollision*	   _pCollision{ nullptr };
		Mathf::Vector2 _addOffset{ 0.f, 0.f };
	};
}