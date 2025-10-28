#pragma once
#include <CoreDefine.h>
#include <ActorComponent.h>
#include <Mathf.h>

namespace Engine
{
	class RigidBodyComponent final : public ActorComponent
	{
	protected:
		explicit RigidBodyComponent() = default;
		virtual ~RigidBodyComponent() = default;

	public:
		virtual bool InitializeComponent() override { return true; }
		virtual void BeginPlay() override {};
		virtual void TickComponent(_duration deltaSeconds) override;
		virtual void EndPlay() override {};
		virtual void Destroy() override {};

	public:
		virtual void SerializeIn(nlohmann::ordered_json& object) {};
		virtual void SerializeOut(nlohmann::ordered_json& object) {};

	public:
		void SetMass(float mass) { _mass = mass; }
		void SetFriction(float friction) { _friction = friction; }
		void AddForce(Mathf::Vector2 force) { _force += force; }
		void SetGrounded(bool isGrounded) { _isGrounded = isGrounded; }
		void SetVelocity(Mathf::Vector2 velocity) { _velocity = velocity; }
		void SetGravityFactor(float gravityFactor) { _gravityFactor = gravityFactor; }
		Mathf::Vector2 GetVelocity() const { return _velocity; }

	public:
		static RigidBodyComponent* Create();

	private:
		Mathf::Vector2 _limitedVelocity{ 800.f, 1000.f };

	private:
		Mathf::Vector2 _acceleration{ UnitVector::Zero };
		Mathf::Vector2 _velocity{ UnitVector::Zero };
		Mathf::Vector2 _force{ UnitVector::Zero };
		Mathf::Vector2 _gravity{ 0.f, 9.8f };

	private:
		float _mass{ 1.f };
		float _friction{ 100.f };
		float _gravityFactor{ 800.f };

	private:
		bool _isGrounded{ false };
	};
}