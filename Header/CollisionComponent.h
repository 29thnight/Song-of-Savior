#pragma once
#include <CoreDefine.h>
#include <RenderComponent.h>

namespace Engine
{
	class CollisionComponent abstract : public RenderComponent //inherit RenderComponent for debug draw
	{
	public:
		using ColliderType = Collider::Collider;
		using CollisionType = Collision::Collision;
		using CollisionState = std::set<CollisionComponent*>;
		using IgnoreLayerList = std::set<int>;

	protected:
		explicit CollisionComponent() = default;
		virtual ~CollisionComponent() = default;

	public:
		virtual void TickComponent(_duration deltaSeconds) override abstract;
		virtual bool IsCollision(CollisionComponent* pOther) abstract;
		virtual void Render(_RenderTarget pRenderTarget) override abstract; //for debug draw
		virtual void Destroy() override abstract;

	public:
		virtual void SerializeIn(nlohmann::ordered_json& object) {};
		virtual void SerializeOut(nlohmann::ordered_json& object) {};
	
	public:
		void ProcessCollision();
		void ProcessCollision(CollisionComponent* pOtherComponent);

		void AddCollisionState(CollisionComponent* pOtherComponent);
		void ClearCollisionState();
		void UpdateCollisionState();

	public:
		void SetColliderType(ColliderType colliderType) { _collider = colliderType; }
		ColliderType GetColliderType() const { return _collider; }

		void SetCollisionType(CollisionType collisionType) { _type = collisionType; }
		CollisionType GetCollisionType() const { return _type; }

		void SetIgnoreLayer(int layerIndex) { _ignoreLayerList.insert(layerIndex); }
		void RemoveIgnoreLayer(int layerIndex) { _ignoreLayerList.erase(layerIndex); }
		const IgnoreLayerList& GetIgnoreLayerList() const { return _ignoreLayerList; }

	public:
		bool operator==(_pstring name) const { return !strcmp(_name.c_str(), name); }
		bool operator!=(_pstring name) const { return strcmp(_name.c_str(), name); }

	public:
		void AddColliderInLayer();

	public:
		void BeginPlay() override{};
		void EndPlay() override{};

	protected:
		ColliderType	_collider{ Collider::COLLIDER_NONE };
		CollisionType	_type{ Collision::COLLISION_IGNORE };
		IgnoreLayerList _ignoreLayerList;
		CollisionState	_previousCollisionState;
		CollisionState	_currentCollisionState;
	};
}