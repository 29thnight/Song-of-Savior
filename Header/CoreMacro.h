#pragma once
//최종 빌드시에는 FINAL을 정의해주세요.
#define FINAL

#undef min
#undef max

#undef interface
#define interface __interface
#define Super __super
#define MAX_OBJECT 20

#ifdef _DEBUG
#define new new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#endif

#define DENY_COPY(ClassName)						\
	ClassName(const ClassName&) = delete;			\
	ClassName(ClassName&&) = delete;				\
	ClassName& operator=(ClassName&&) = delete;		\
	ClassName& operator=(const ClassName&) = delete;

#define _SINGLETON_OPERATOR(ClassName)                        \
    public:                                                   \
        ClassName(const ClassName&) = delete;                 \
        ClassName& operator=(const ClassName&) = delete;      \
        ClassName(ClassName&&) = delete;                      \
        ClassName& operator=(ClassName&&) = delete;

#define CORECLASS()                 \
    struct AnimationClip;		    \
    struct InputActionValue;        \
    struct InputValue;              \
    __interface IColliderReceiver;  \
	__interface IInputAction;       \
	class Animation;                \
	class CoreBase;                 \
    class ACollision;               \
    class RCollision;               \
	class SCollision;               \
	class OCollision;               \
    class Level;                    \
	class Canvas;                   \
    class Texture;                  \
    class Object;                   \
    class Layer;                    \
    class World;					\
    class Actor;					\
    class State;					\
    class Actor;                    \
	class BoxComponent;		        \
	class StateMachine;			    \
	class StateTransition;			\
    class CameraActor;              \
    class TimeManager;              \
	class VideoManager;             \
	class CoreManager;              \
    class GraphicsManager;          \
    class CollisionManager;         \
    class TextureManager;           \
    class ObjectManager;            \
    class SceneManager;             \
    class InputManager;             \
	class FontManager;              \
    class InputAction;              \
	class BasicAnimationComponent;  \
	class AnimationComponent;       \
    class ActorComponent;           \
    class StateComponent;		    \
	class InputComponent;           \
    class CameraComponent;          \
	class CollisionComponent;       \
	class RenderComponent;          \
	class BitmapComponent;          \
	class SceneComponent;			\
	class TextRenderComponent;		\
	class TimerComponent;			\
	class LineRenderComponent;

//////////////////////////////////////////////
// Define wWinMain Release                  //
//////////////////////////////////////////////
#define DEFINE_WWINMAIN_RELEASE(GAME_SETTINGS_PATH, Proc) \
int APIENTRY wWinMain(_In_ HINSTANCE hInstance, \
	_In_opt_ HINSTANCE hPrevInstance, \
	_In_ LPWSTR lpszCmdParam, \
	_In_ int nCmdShow) \
{ \
	\
	\
	UNREFERENCED_PARAMETER(hPrevInstance); \
	UNREFERENCED_PARAMETER(lpszCmdParam); \
	\
	Client::ClientEntry winApp; \
	winApp._customWNDPROC = Proc;\
	winApp.CreateWorld(); \
	\
    Engine::GameSettings gameSetting{}; \
    \
    gameSetting.hInstance = hInstance; \
	gameSetting.pWorld = winApp.ContainWorld(); \
	winApp.LoadGameSettings(GAME_SETTINGS_PATH, gameSetting); \
	EngineInititalize(winApp, gameSetting); \
	\
	\
	return EXIT_SUCCESS; \
}

//////////////////////////////////////////////
// Define wWinMain Debug				    //
//////////////////////////////////////////////
#define DEFINE_WWINMAIN_DEBUG(DEBUG_LINE,GAME_SETTINGS_PATH, Proc) \
int APIENTRY wWinMain(_In_ HINSTANCE hInstance, \
	_In_opt_ HINSTANCE hPrevInstance, \
	_In_ LPWSTR lpszCmdParam, \
	_In_ int nCmdShow) \
{ \
	\
	_CrtSetBreakAlloc(DEBUG_LINE); \
	\
	UNREFERENCED_PARAMETER(hPrevInstance); \
	UNREFERENCED_PARAMETER(lpszCmdParam); \
	\
	Client::ClientEntry winApp; \
	winApp._customWNDPROC = Proc;\
	winApp.CreateWorld(); \
	\
    Engine::GameSettings gameSetting{}; \
    \
    gameSetting.hInstance = hInstance; \
	gameSetting.pWorld = winApp.ContainWorld(); \
	winApp.LoadGameSettings(GAME_SETTINGS_PATH, gameSetting); \
	EngineInititalize(winApp, gameSetting); \
	\
	_CrtDumpMemoryLeaks(); \
	\
	return EXIT_SUCCESS; \
}

//////////////////////////////////////////////
// Define Entry Code                        //
//////////////////////////////////////////////
#define EngineInititalize(entryName, gameSettingName) \
entryName.Initialize(std::move(gameSettingName)); \
entryName.MainLoop(); \
entryName.Terminate();

//////////////////////////////////////////////
// Define Delegate                          //
//////////////////////////////////////////////

#define DECLARE_DELEGATE(DelegateName, ...) \
    using DelegateName = Delegate<__VA_ARGS__>;

//////////////////////////////////////////////
// Define Property                          //
//////////////////////////////////////////////

#define _Property(t,n)  __declspec( property ( put = property__set_##n, get = property__get_##n ) ) t n;\
	typedef t property__tmp_type_##n;
#define ReadOnly_Property(t,n) __declspec( property (get = property__get_##n) ) t n;\
	typedef t property__tmp_type_##n;
#define WriteOnly_Property(t,n) __declspec( property (put = property__set_##n) ) t n;\
	typedef t property__tmp_type_##n;
#define _Get(n) property__tmp_type_##n property__get_##n() 
#define _Set(n) void property__set_##n(const property__tmp_type_##n& value)

//////////////////////////////////////////////
// Define KeyArrow                          //
//////////////////////////////////////////////

#define DIK_AXIS 0x00001011

//////////////////////////////////////////////
// Define GAMEPAD                           //
//////////////////////////////////////////////

#define GAMEPAD_NORMALIZE 32767.f

#define DIP_LX 0x00020010
#define DIP_LY 0x00020011
#define DIP_RX 0x00020012
#define DIP_RY 0x00020013

#define DIP_LT 0x00020014
#define DIP_RT 0x00020015

#define DIP_DPAD_UP          0x00020000
#define DIP_DPAD_DOWN        0x00020001
#define DIP_DPAD_LEFT        0x00020002
#define DIP_DPAD_RIGHT       0x00020003

#define DIP_MENU             0x00020004
#define DIP_VIEW             0x00020005

#define DIP_LEFT_THUMB       0x00020006
#define DIP_RIGHT_THUMB      0x00020007

#define DIP_LEFT_SHOULDER    0x00020008
#define DIP_RIGHT_SHOULDER   0x00020009

#define DIP_A                0x0002000C
#define DIP_B                0x0002000D
#define DIP_X                0x0002000E
#define DIP_Y                0x0002000F

#define DIP_MAX              16

//////////////////////////////////////////////
// Define MOUSE                             //
//////////////////////////////////////////////

#define DIM_LB      0x00010000
#define DIM_RB      0x00010001
#define DIM_WB      0x00010002

#define DIM_SCROLL  0x00010004
#define DIM_MOVE    0x00010005

#define DIM_END     3

//////////////////////////////////////////////
// Define DebugPrintf					    //
//////////////////////////////////////////////
//#define FINAL
#ifdef FINAL
#define DebugPrintf(...)
#endif // !FINAL
#ifndef FINAL
#ifdef _DEBUG
#define DebugPrintf(fmt, ...) printf(fmt, __VA_ARGS__)
#elif NDEBUG
#define DebugPrintf(fmt, ...) printf(fmt, __VA_ARGS__)
#endif // _DEBUG
#endif // !FINAL





