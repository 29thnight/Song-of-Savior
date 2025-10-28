#pragma once
#include <CoreDefine.h>

namespace Engine
{
	class Texture;
	struct AnimationClip
	{
		string clipName{};
		int clipIndex{};
		int maxFrame{};
		double duration{};
		float frameTime{};
		bool isLoop{};

		AnimationClip(const char* name) : clipName(name), clipIndex(0) {};
	};
}

namespace Engine
{
	class CollisionComponent;
	struct CollisionData
	{
		CollisionComponent* thisComponent{ nullptr };
		CollisionComponent* otherComponent{ nullptr };
	};
}

namespace Engine
{
	struct FadeSound
	{
		float current{ 1.f };
		float target{ 1.f };
	};
}

namespace Engine
{
	class World;
	struct GameSettings
	{
		World*		 pWorld{};
		const TCHAR* title{};
		HINSTANCE	 hInstance{};
		HWND		 hWnd{};
		int 		 width{ 1920 };
		int 		 height{ 1080 };
		float		 timeScale{ 1.f };
		int			 fixedCount{ 50 };
		int			 layerSize{ LAYER::END };
		int			 maxSoundGroup{};
		//±ÍÂú¾Æ!!!!! ÀÌ°Å ´Ù ¾²±â ±ÍÂú¾Æ!!!!
		GameSettings() = default;
		GameSettings(
			World* pWorld, 
			const TCHAR* title, 
			HINSTANCE hInstance, 
			HWND hWnd, 
			int width, 
			int height, 
			float timeScale, 
			int fixedCount, 
			int layerSize, 
			int maxSoundGroup
		)
			: pWorld{ pWorld }, title{ title }, hInstance{ hInstance }, 
			  hWnd{ hWnd }, width{ width }, height{ height }, timeScale{ timeScale }, 
			  fixedCount{ fixedCount }, layerSize{ layerSize }, maxSoundGroup{ maxSoundGroup } {}

		GameSettings(const GameSettings& settings) = default;
		GameSettings(GameSettings&& settings) = default;
		~GameSettings() = default;

		GameSettings& operator=(const GameSettings& settings) = default;
		GameSettings& operator=(GameSettings&& settings) = default;
	};
}

namespace Engine
{
	struct InputValue
	{
		InputDevice device;
		InputType type;
		unsigned int key;
		float value;
		bool state;
		long x, y;
		float timeToLastInput{};

		InputValue(InputDevice _device, InputType _type, unsigned int _key, float _value, bool _state, long x, long y)
			: device(_device), type(_type), key(_key), value(_value), state(_state), x(x), y(y) {}
	};
}