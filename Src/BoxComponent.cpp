#include <Actor.h>
#include <CollisionComponent.h>
#include <CoreManager.h>
#include <GraphicsManager.h>
#include <ACollision.h>
#include <RCollision.h>
#include <LineColliderComponent.h>
#include <BoxComponent.h>

bool Engine::BoxComponent::InitializeComponent()
{
	_pCollision = ACollision::Create();
	if (!_pCollision)
	{
		return false;
	}

	_collider = Collider::COLLIDER_AABB;
	_type = Collision::COLLISION_BLOCK;

	return true;
}

void Engine::BoxComponent::TickComponent(_duration deltaSeconds)
{
	SceneComponent::TickComponent(deltaSeconds);
	Mathf::Vector2 worldLocation = GetWorldLocation();
	_pCollision->SetCollisionOffset(worldLocation);
}

bool Engine::BoxComponent::IsCollision(CollisionComponent* pOther)
{
	if(this->_owner == pOther->GetOwner())
	{
		return false;
	}

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
	case Collider::COLLIDER_LINE:
	{
		LineColliderComponent* pRay = dynamic_cast<LineColliderComponent*>(pOther);
		if (pRay)
		{
			return _pCollision->CheckCollision(pRay->GetCollision());
		}
	}
	break;
	default:
	break;
	}

	return false;
}

void Engine::BoxComponent::Render(_RenderTarget pRenderTarget)
{
	if (_type == Collision::COLLISION_IGNORE)
	{
		return;
	}

	Mathf::Matx3F Transform = _cameraMatrix;
	pRenderTarget->SetTransform(Transform);

	Mathf::Vector2 point = {
		_pCollision->GetCollisionOffset().x,
		_pCollision->GetCollisionOffset().y
	};

	ID2D1SolidColorBrush* m_pBrush = Graphics->GetBrush("Default");
	
	pRenderTarget->DrawLine(D2D1::Point2F(point.x - 5.0f, point.y), D2D1::Point2F(point.x + 5.0f, point.y), m_pBrush, 3.0f);
	pRenderTarget->DrawLine(D2D1::Point2F(point.x, point.y - 5.0f), D2D1::Point2F(point.x, point.y + 5.0f), m_pBrush, 3.0f);

	Mathf::RectF rect = {
		point.x - _pCollision->GetCollisionSize().x * 0.5f,
		point.y - _pCollision->GetCollisionSize().y * 0.5f,
		point.x + _pCollision->GetCollisionSize().x * 0.5f,
		point.y + _pCollision->GetCollisionSize().y * 0.5f
	};

	pRenderTarget->DrawRectangle(D2D1::RectF(rect.left, rect.top, rect.right, rect.bottom), m_pBrush, 3.0f);
}

void Engine::BoxComponent::SetAddOffset(const Mathf::Vector2& offsetVector)
{
	SetRelativeLocation(offsetVector);
}

void Engine::BoxComponent::SetSize(const Mathf::Vector2& sizeVector)
{
	_pCollision->SetCollisionSize(sizeVector);
}

void Engine::BoxComponent::Destroy()
{
	SafeDelete(_pCollision);
	Management->RemoveCollisionQueue(_owner->GetLayerIndex(),this);
	Super::Destroy();
}

Engine::BoxComponent* Engine::BoxComponent::Create()
{
	return new BoxComponent;
}
