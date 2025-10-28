#include <Transform.h>

void Engine::Transform::AddRelativeRotation(float rotation)
{
	_RelativeRotation += rotation;
	_RelativeRotation = fmodf(_RelativeRotation, 360.f);
}

Mathf::Vector2 Engine::Transform::GetWorldLocation()
{
	_WorldLocation.x = _WorldTransform._31;
	_WorldLocation.y = _WorldTransform._32;

	return _WorldLocation;
}