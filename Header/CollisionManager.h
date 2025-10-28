#pragma once
#include <CoreDefine.h>

namespace Engine
{
	CORECLASS()
	class CollisionManager : public CoreBase
	{
	public:
		using CollsionData = std::pair<int, CollisionComponent*>;
		using CollisionQueue = std::vector<CollsionData>;

	protected:
		explicit CollisionManager() = default;
		virtual ~CollisionManager() = default;

	public:
		void SimulateCollision();
		void AddCollisionQueue(int layerIndex, CollisionComponent* pCollsionComponent);
		void RemoveCollisionQueue(int layerIndex, CollisionComponent* pCollsionComponent);

	public:
		virtual void SerializeIn(nlohmann::ordered_json& object) {};
		virtual void SerializeOut(nlohmann::ordered_json& object) {};

	public:
		static CollisionManager* Create();
		void Destroy() override;

	private:
		CollisionQueue _collisionQueue;
	};
}