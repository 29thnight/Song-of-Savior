#pragma once
#include <CoreBase.h>
#include <Mathf.h>

namespace Engine
{
	class ACollision : public CoreBase
	{
	protected:
		explicit ACollision() = default;
		virtual ~ACollision() = default;

	public:
		virtual void SerializeIn(nlohmann::ordered_json& object) {};
		virtual void SerializeOut(nlohmann::ordered_json& object) {};

	public:
		void SetCollisionSize(const Mathf::Vector2& collisionSize) { _collisionSize = collisionSize; }
		Mathf::Vector2 GetCollisionSize() const { return _collisionSize; }

		void SetCollisionOffset(const Mathf::Vector2& collisionOffset) { _collisionOffset = collisionOffset; }
		Mathf::Vector2 GetCollisionOffset() const { return _collisionOffset; }

		void SetCollisionScale(const Mathf::Vector2& collisionScale);
		Mathf::Vector2 GetCollisionScale() const { return _collisionSize; }

		bool CheckCollision(ACollision* pCollision) const;
		bool CheckCollision(RCollision* pCollision) const;

	public:
		static ACollision* Create() { return new ACollision(); }
		virtual void Destroy() override {}

	private:
		Mathf::Vector2 _collisionSize{ 0.f, 0.f };
		Mathf::Vector2 _collisionOffset{ 0.f, 0.f };
	};
}