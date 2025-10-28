#pragma once
#include <CoreBase.h>
#include <Mathf.h>

//class for raycast and box collision
namespace Engine
{
	class ACollision;
	class SCollision;
	class RCollision : public CoreBase
	{
	protected:
		explicit RCollision() = default;
		virtual ~RCollision() = default;

	public:
		virtual void SerializeIn(nlohmann::ordered_json& object) {};
		virtual void SerializeOut(nlohmann::ordered_json& object) {};

	public:
		void SetOrigin(const Mathf::Vector2& origin) { _origin = origin; }
		Mathf::Vector2 GetOrigin() const { return _origin; }

		void SetRayDirection(const Mathf::Vector2& rayDirection) { _rayDirection = rayDirection; }
		Mathf::Vector2 GetRayDirection() const { return _rayDirection; }

		bool CheckCollision(ACollision* pCollision) const;
		bool CheckCollision(SCollision* pCollision) const;

	public:
		static RCollision* Create() { return new RCollision; }
		virtual void Destroy() override {}

	private:
		Mathf::Vector2 _origin{ UnitVector::Zero };
		Mathf::Vector2 _rayDirection{ UnitVector::Zero };
	};
}
