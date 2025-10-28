#include <RCollision.h>
#include <ACollision.h>

#undef min
#undef max

bool Engine::RCollision::CheckCollision(ACollision* pCollision) const
{
	 // AABB의 좌표 및 크기 계산
    float thatLeft = pCollision->GetCollisionOffset().x - pCollision->GetCollisionSize().x * 0.5f;
    float thatRight = pCollision->GetCollisionOffset().x + pCollision->GetCollisionSize().x * 0.5f;
    float thatTop = pCollision->GetCollisionOffset().y - pCollision->GetCollisionSize().y * 0.5f;
    float thatBottom = pCollision->GetCollisionOffset().y + pCollision->GetCollisionSize().y * 0.5f;

    // 레이의 원점과 방향
    float rayOriginX = _origin.x;
    float rayDirectionX = _rayDirection.x;
    float rayOriginY = _origin.y;
    float rayDirectionY = _rayDirection.y;

    // 레이의 시작과 끝 좌표 계산
    float thisLeft = rayOriginX;
    float thisRight = rayOriginX + rayDirectionX;
    float thisTop = rayOriginY;
    float thisBottom = rayOriginY + rayDirectionY;

    // 초기 tEntry와 tExit 값
    float tEntry = 0.0f;
    float tExit = 1.0f;

    // x축에 대한 충돌 검사
    if (rayDirectionX == 0.0f)
    {
        if (thisLeft < thatLeft || thisRight > thatRight)
        {
            return false;
        }
    }
    else
    {
        float xInvEntry = 1.0f / rayDirectionX;
        float xEntry = (thatLeft - thisLeft) * xInvEntry;
        float xExit = (thatRight - thisLeft) * xInvEntry;

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
    if (rayDirectionY == 0.0f)
    {
        if (thisTop < thatTop || thisBottom > thatBottom)
        {
            return false;
        }
    }
    else
    {
        float yInvEntry = 1.0f / rayDirectionY;
        float yEntry = (thatTop - thisTop) * yInvEntry;
        float yExit = (thatBottom - thisTop) * yInvEntry;

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

    // 최종적으로 충돌 여부를 반환
    return tEntry <= tExit;
}

bool Engine::RCollision::CheckCollision(SCollision* pCollision) const
{
	return false;
}
