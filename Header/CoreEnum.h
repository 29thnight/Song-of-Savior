#pragma once

namespace LAYER
{
	enum LayerIndex
	{
		BACKGROUND = 0,
		BACKOBJECT,
		OBJECT,
		UI,
		FRONTOBJECT,
		END
	};
}

enum BufferIndex
{
	BACKBUFFER = 0,
	FRONTBUFFER,
	BUFFER_END
};

namespace Collider
{
	enum Collider
	{
		COLLIDER_NONE = -1,
		COLLIDER_AABB = 0,
		COLLIDER_OBB,
		COLLIDER_SPHERE,
        COLLIDER_LINE,
		COLLIDER_END
	};
}

namespace Collision
{
	enum Collision
	{
		COLLISION_IGNORE = 0,
		COLLISION_OVERLAP,
		COLLISION_BLOCK,
		COLLISION_END
	};
}

enum class InputDevice
{
	KEYBOARD,
	MOUSE,
	GAMEPAD,
	JOYSTICK,
	END
};

enum class InputType
{
	IDLE = -1,
	PRESS,
	RELEASE,
	HELD,
	AXIS,
	SCROLL,
	MOVE,
	TRIGGER,
	END
};

enum class TriggerEvent
{
	Entered,
	Triggered,
	Completed,
	END
};

enum MouseState
{
	DIM_LB,
	DIM_RB,
	DIM_WB,
	DIM_END
};

#undef INFINITE
enum SoundLoop
{
	LOOP_INFINITE = -1,
	NOT_LOOP = 0
};

enum SoundChannel
{
	CHANNEL_BGM = 0,
	CHANNEL_SFX_1,
	CHANNEL_SFX_2,
	CHANNEL_SFX_3,
	CHANNEL_SFX_4,
	CHANNEL_SFX_5,
	CHANNEL_UI,
	CHANNEL_END
};