#include <CollisionComponent.h>
#include <Actor.h>
#include <CoreManager.h>
#include <ACollision.h>

void Engine::CollisionComponent::Destroy()
{
#ifdef DEBUG
	Management->RemoveRenderQueue(_owner->GetLayerIndex(), this);
#endif // DEBUG
}

void Engine::CollisionComponent::ProcessCollision()
{
	for(auto& pOtherComponent : _currentCollisionState)
	{
		if (_previousCollisionState.find(pOtherComponent) == 
			_previousCollisionState.end())
		{
			_owner->NotifyActorBeginOverlap({this, pOtherComponent});
		}	
	}

	for (auto& pOtherComponent : _previousCollisionState)
	{
		if (_currentCollisionState.find(pOtherComponent) ==
			_currentCollisionState.end())
		{
			_owner->NotifyActorEndOverlap({this, pOtherComponent});
		}
	}
}

void Engine::CollisionComponent::ProcessCollision(CollisionComponent* pOtherComponent)
{
	_owner->NotifyActorBlock({this, pOtherComponent});
}

void Engine::CollisionComponent::AddCollisionState(CollisionComponent* pOtherComponent)
{
	_currentCollisionState.insert(pOtherComponent);
}

void Engine::CollisionComponent::ClearCollisionState()
{
	_previousCollisionState.clear();
	_currentCollisionState.clear();
}

void Engine::CollisionComponent::UpdateCollisionState()
{
	_previousCollisionState = _currentCollisionState;
	_currentCollisionState.clear();
}

void Engine::CollisionComponent::AddColliderInLayer()
{
	#ifdef _DEBUG
		AddRenderQueueInLayer();
	#endif // _DEBUG

	int index = _owner->GetLayerIndex();
	Management->AddCollisionQueue(index, this);
}
