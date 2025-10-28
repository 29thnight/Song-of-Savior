#include <Actor.h>
#include <CollisionComponent.h>
#include <CoreManager.h>
#include <GraphicsManager.h>
#include <ACollision.h>
#include <RCollision.h>
#include <BoxComponent.h>
#include <LineColliderComponent.h>

bool Engine::LineColliderComponent::InitializeComponent()
{
	_pCollision = RCollision::Create();
	if (!_pCollision)
	{
		return false;
	}

	_collider = Collider::COLLIDER_LINE;
	_type = Collision::COLLISION_BLOCK;

	return true;
}

void Engine::LineColliderComponent::TickComponent(_duration deltaSeconds)
{
	SceneComponent::TickComponent(deltaSeconds);
	Mathf::Vector2 worldLocation = GetWorldLocation();
	_pCollision->SetOrigin(worldLocation);
}

bool Engine::LineColliderComponent::IsCollision(CollisionComponent* pOther)
{
	switch (pOther->GetColliderType())
	{
	case Collider::COLLIDER_AABB:
	{
		BoxComponent* pBox = dynamic_cast<BoxComponent*>(pOther);
		if (pBox)
		{
			return _pCollision->CheckCollision(pBox->GetCollision());
		}
	}
	break;
	default:
	break;
	}

	return false;
}

void Engine::LineColliderComponent::Render(_RenderTarget pRenderTarget)
{
	if (_type == Collision::COLLISION_IGNORE)
	{
		return;
	}

	Mathf::Matx3F Transform = _cameraMatrix;
	pRenderTarget->SetTransform(Transform);

	Mathf::Vector2 point = {
		_pCollision->GetOrigin().x,
		_pCollision->GetOrigin().y
	};

	ID2D1SolidColorBrush* m_pBrush = Graphics->GetBrush("Default");

	pRenderTarget->DrawLine(D2D1::Point2F(point.x - 5.0f, point.y), D2D1::Point2F(point.x + 5.0f, point.y), m_pBrush, 3.0f);

	Mathf::Vector2 direction = {
		_pCollision->GetRayDirection().x,
		_pCollision->GetRayDirection().y
	};

	pRenderTarget->DrawLine(D2D1::Point2F(point.x, point.y), D2D1::Point2F(point.x + direction.x, point.y + direction.y), m_pBrush, 3.0f);
}

void Engine::LineColliderComponent::SetAddOffset(const Mathf::Vector2& offsetVector)
{
	SetRelativeLocation(offsetVector);
}

void Engine::LineColliderComponent::SetSize(const Mathf::Vector2& sizeVector)
{
	_pCollision->SetRayDirection(sizeVector);
}

void Engine::LineColliderComponent::Destroy()
{
	Management->RemoveCollisionQueue(_owner->GetLayerIndex(), this);
	SafeDelete(_pCollision);
}

Engine::LineColliderComponent* Engine::LineColliderComponent::Create()
{
	return new LineColliderComponent();
}