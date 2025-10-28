#pragma once
#include <CoreDefine.h>

namespace Engine
{
	struct CollisionData;
	interface IColliderReceiver
	{
		virtual void NotifyActorBlock(CollisionData data) abstract;
		virtual void NotifyActorBeginOverlap(CollisionData data) abstract;
		virtual void NotifyActorEndOverlap(CollisionData data) abstract;
	};
}