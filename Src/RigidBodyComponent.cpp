#include <RigidBodyComponent.h>
#include <SceneComponent.h>
#include <Actor.h>
#include <Mathf.h>
#include <TimeManager.h>

void Engine::RigidBodyComponent::TickComponent(_duration deltaSeconds)
{
	_acceleration = _force / _mass;

	_velocity += _acceleration * Time->DurationToFloat(deltaSeconds);

	if(_isGrounded)
	{
		Mathf::Vector2 gravity = _gravity;
		
		_velocity -= gravity * Mathf::Dot(_velocity, gravity.Normalize());
	}
	else
	{
		_velocity += _gravity * _gravityFactor * Time->DurationToFloat(deltaSeconds);
	}

	Mathf::Vector2 gravity = _gravity;
	gravity = gravity * Mathf::Dot(_velocity, gravity.Normalize());

	Mathf::Vector2 sideVelocity = _velocity - gravity;
	if (abs(_limitedVelocity.y) < abs(gravity.Length))
	{
		_velocity = gravity.Normalize() * _limitedVelocity.y;
	}

	if (abs(_limitedVelocity.x) < abs(sideVelocity.Length))
	{
		_velocity = sideVelocity.Normalize() * _limitedVelocity.x;
	}
	_velocity = gravity + sideVelocity;

	if(_velocity != UnitVector::Zero)
	{
		Mathf::Vector2 friction = _velocity.Normalize() * -1.f * _friction * _mass * Time->DurationToFloat(deltaSeconds);

		if(_velocity.Length < friction.Length)
		{
			_velocity = UnitVector::Zero;
		}
		else
		{
			_velocity += friction;
		}
	}

	_owner->GetRootComponent()->AddRelativeLocation(_velocity * Time->DurationToFloat(deltaSeconds));
	_owner->GetRootComponent()->SetVelocity(_velocity);

	_force = UnitVector::Zero;
}

Engine::RigidBodyComponent* Engine::RigidBodyComponent::Create()
{
	return new RigidBodyComponent();
}
