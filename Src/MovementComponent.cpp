#include "MovementComponent.h"
#include "SceneComponent.h"
#include "Transform.h"
#include "Actor.h"
#include "TimeManager.h"

#undef min
#undef max

bool Engine::MovementComponent::InitializeComponent()
{
	//default settings
	_moveSpeed = 150.f;
	_gravityFactor = 550.f;
	_calculatedGravity = _gravity * _gravityFactor;
	_jumpPower = 1700.f;
	_limitSpeed = 100.0f;

	_acceleration = 4.f;
	_friction = 0.55f;
	_targetSpeed = 0.f;

	return true;
}

void Engine::MovementComponent::BeginPlay()
{

}

void Engine::MovementComponent::TickComponent(_duration deltaSeconds)
{
	if(!_pRootComponent)
	{
		SetRootComponent(_owner->GetRootComponent());
	}

	_ownerCurrentLocation = _pRootComponent->GetWorldLocation();

	_velocity.y += _calculatedGravity * Time->DurationToFloat(deltaSeconds);
	//std::cout << _velocity.y << std::endl;
	if (_velocity.x > 0)
	{
		_velocity.x = std::max(_velocity.x, -_limitSpeed);
	}
	else
	{
		_velocity.x = std::min(_velocity.x, _limitSpeed);
	}

	_pRootComponent->AddRelativeLocation(_velocity * Time->DurationToFloat(deltaSeconds));
	_pRootComponent->SetVelocity(_velocity);

	_velocity.x *= _friction;

	if (_ownerCurrentLocation.y > _groundPosition)
	{
		_pRootComponent->SetRelativeY(_groundPosition);
		_velocity.y = -1.f;
		_isGrounded = true;
	}
		
	_isJumping = false;
}

void Engine::MovementComponent::EndPlay()
{
}

void Engine::MovementComponent::Move(float deltaSeconds)
{
	_velocity.x = _inputDirection.x * _moveSpeed;
}

void Engine::MovementComponent::Run(float deltaSeconds)
{
	if(_isRunning)
		_velocity.x = _inputDirection.x * _acceleration * _moveSpeed;
}

void Engine::MovementComponent::Jump(float deltaSeconds)
{
	if (_isGrounded && !_isJumping)
	{
		_velocity.y = _inputDirection.y * _jumpPower;
		_isGrounded = false;
		_isJumping = true;
	}
}

void Engine::MovementComponent::DiagonalMove(float deltaSeconds)
{
	_velocity.x = _inputDirection.x * _moveSpeed;
	_velocity.y = _inputDirection.y * _jumpPower;
}

Engine::MovementComponent* Engine::MovementComponent::Create()
{
	return new MovementComponent();
}
