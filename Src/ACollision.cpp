#include <ACollision.h>
#include <RCollision.h>
#include <Mathf.h>

#undef min
#undef max

void Engine::ACollision::SetCollisionScale(const Mathf::Vector2& collisionScale)
{
	_collisionSize.x *= collisionScale.x;
	_collisionSize.y *= collisionScale.y;

	_collisionOffset.x *= collisionScale.x;
	_collisionOffset.y *= collisionScale.y;
}

bool Engine::ACollision::CheckCollision(ACollision* pCollision) const
{
	float thisLeft = _collisionOffset.x - _collisionSize.x * 0.5f;
	float thisRight = _collisionOffset.x + _collisionSize.x * 0.5f;
	float thisTop = _collisionOffset.y - _collisionSize.y * 0.5f;
	float thisBottom = _collisionOffset.y + _collisionSize.y * 0.5f;

	float otherLeft = pCollision->GetCollisionOffset().x - pCollision->GetCollisionSize().x * 0.5f;
	float otherRight = pCollision->GetCollisionOffset().x + pCollision->GetCollisionSize().x * 0.5f;
	float otherTop = pCollision->GetCollisionOffset().y - pCollision->GetCollisionSize().y * 0.5f;
	float otherBottom = pCollision->GetCollisionOffset().y + pCollision->GetCollisionSize().y * 0.5f;

	if (thisRight < otherLeft || thisLeft > otherRight || thisBottom < otherTop || thisTop > otherBottom)
	{
		return false;
	}

	return true;
}

bool Engine::ACollision::CheckCollision(RCollision* pCollision) const
{
	float thisLeft = _collisionOffset.x - _collisionSize.x * 0.5f;
	float thisRight = _collisionOffset.x + _collisionSize.x * 0.5f;
	float thisTop = _collisionOffset.y - _collisionSize.y * 0.5f;
	float thisBottom = _collisionOffset.y + _collisionSize.y * 0.5f;

	Mathf::Vector2 rayOrigin = pCollision->GetOrigin();
    Mathf::Vector2 rayDirection = pCollision->GetRayDirection();

	float tEntry = 0.0f;
    float tExit = 1.0f;

    // x축에 대한 충돌 검사
    if (rayDirection.x == 0.0f)
    {
        if (rayOrigin.x < thisLeft || rayOrigin.x > thisRight)
        {
            return false;
        }
    }
    else
    {
        float xInvEntry = 1.0f / rayDirection.x;
        float xEntry = (thisLeft - rayOrigin.x) * xInvEntry;
        float xExit = (thisRight - rayOrigin.x) * xInvEntry;

        if (xEntry > xExit)
        {
            std::swap(xEntry, xExit);
        }

        tEntry = std::max(xEntry, tEntry);
        tExit = std::min(xExit, tExit);

        if (tEntry > tExit)
        {
            return false;
        }
    }

    // y축에 대한 충돌 검사
    if (rayDirection.y == 0.0f)
    {
        if (rayOrigin.y < thisTop || rayOrigin.y > thisBottom)
        {
            return false;
        }
    }
    else
    {
        float yInvEntry = 1.0f / rayDirection.y;
        float yEntry = (thisTop - rayOrigin.y) * yInvEntry;
        float yExit = (thisBottom - rayOrigin.y) * yInvEntry;

        if (yEntry > yExit)
        {
            std::swap(yEntry, yExit);
        }

        tEntry = std::max(yEntry, tEntry);
        tExit = std::min(yExit, tExit);

        if (tEntry > tExit)
        {
            return false;
        }
    }

    return true;
}
