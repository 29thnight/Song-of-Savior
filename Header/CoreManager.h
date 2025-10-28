#pragma once
#include <CoreDefine.h>
#include <CoreBase.h>
#include <Singleton.h>

namespace Engine
{
	CORECLASS()
	class CoreManager : public CoreBase, public Singleton<CoreManager>
	{
	private:
		friend class Singleton;

	private:
		using DestroyList = std::deque<Object*>;

	private:
		explicit CoreManager();
		virtual ~CoreManager() = default;

	public:
		void BeginPlay();
		void Tick();
		void Fixed(int count);
		void Render();
		void EndPlay();
		void DestroyPoint();
		/** 이번 Tick이 끝난 뒤 DestroyPoint() 함수를 호출해줍니다.*/
		void ScheduleDestroyPoint() { _isScheduleDestroyPoint = true; }

	private:
		void ScheduleDestroy();

	public:
		virtual void SerializeIn(nlohmann::ordered_json& object) {};
		virtual void SerializeOut(nlohmann::ordered_json& object) {};

	public:
		void Initialize(GameSettings&& gameSettings);
		void OnFinalDestroy() { _isDestroy = true; }
		bool IsFinalDestroy() const { return _isDestroy; }

	public:
		void AddDestroyList(Object* pObject);
		void EraseActorMap(Object* pObject);
		Object* FindDestroyList(_pstring name);
		void UnRegisterDestroyList(Object* pObject);

	public:
		Layer* GetLayer(int layerIndex);
		void AddRenderQueue(int layerIndex, RenderComponent* pRenderComponent);
		void RemoveRenderQueue(int layerIndex, RenderComponent* pRenderComponent);

	public:
		void AddCollisionQueue(int layerIndex, CollisionComponent* pCollsionComponent);
		void RemoveCollisionQueue(int layerIndex, CollisionComponent* pCollsionComponent);

	public:
		ReadOnly_Property(GameSettings, setting)
		_Get(setting)
		{
			return _gameSettings;
		}

		_Property(IWICImagingFactory*, WICFactory)
		_Get(WICFactory)
		{
			return _pWICFactory;
		}
		_Set(WICFactory)
		{
			if (!_pWICFactory)
				_pWICFactory = value;
		}

		_Property(_RenderTarget, renderTarget)
		_Get(renderTarget)
		{
			return _renderTarget;
		}
		_Set(renderTarget)
		{
			if(!_renderTarget)
				_renderTarget = value;
		}

	private:
		void Destroy() override;

	private:
		CollisionManager* _pCollisionManager{ nullptr };

	private:
		IWICImagingFactory*	_pWICFactory{};
		_RenderTarget		_renderTarget{ nullptr };

	private:
		DestroyList	 _toBeDestroyed;
		GameSettings _gameSettings;
		_duration	 _elapsedTick{ 0 };
		bool 		 _isDestroy{ false };
		bool		 _isScheduleDestroyPoint{ false };
	};
}

inline Engine::CoreManager* Management = Engine::CoreManager::GetInstance();
inline std::function<void()> ManagementTerminate = []() { Management->Terminate(); };