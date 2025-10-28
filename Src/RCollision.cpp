#include <RCollision.h>
#include <ACollision.h>

#undef min
#undef max

bool Engine::RCollision::CheckCollision(ACollision* pCollision) const
{
	 // AABB�� ��ǥ �� ũ�� ���
    float thatLeft = pCollision->GetCollisionOffset().x - pCollision->GetCollisionSize().x * 0.5f;
    float thatRight = pCollision->GetCollisionOffset().x + pCollision->GetCollisionSize().x * 0.5f;
    float thatTop = pCollision->GetCollisionOffset().y - pCollision->GetCollisionSize().y * 0.5f;
    float thatBottom = pCollision->GetCollisionOffset().y + pCollision->GetCollisionSize().y * 0.5f;

    // ������ ������ ����
    float rayOriginX = _origin.x;
    float rayDirectionX = _rayDirection.x;
    float rayOriginY = _origin.y;
    float rayDirectionY = _rayDirection.y;

    // ������ ���۰� �� ��ǥ ���
    float thisLeft = rayOriginX;
    float thisRight = rayOriginX + rayDirectionX;
    float thisTop = rayOriginY;
    float thisBottom = rayOriginY + rayDirectionY;

    // �ʱ� tEntry�� tExit ��
    float tEntry = 0.0f;
    float tExit = 1.0f;

    // x�࿡ ���� �浹 �˻�
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

    // y�࿡ ���� �浹 �˻�
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

    // ���������� �浹 ���θ� ��ȯ
    return tEntry <= tExit;
}

bool Engine::RCollision::CheckCollision(SCollision* pCollision) const
{
	return false;
}
