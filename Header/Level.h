#pragma once
#include <CoreDefine.h>
#include <Mathf.h>
#include <vector>

namespace Engine
{
	CORECLASS()
	class Level : public CoreBase
	{
	private:
		using ActorsList = std::list<Actor*>;

	protected:
		Level() = default;
		~Level() = default;

	public:
		virtual bool BeginPlay() PURE;
		virtual void Tick(_duration deltaSeconds);
		virtual void Fixed();
		virtual void EndPlay();
		virtual void DestroyPoint();

	public:
		virtual void SerializeIn(nlohmann::ordered_json& object) override;
		virtual void SerializeIn(nlohmann::ordered_json& object, std::vector<Actor*>* actorListOutput);
		virtual void SerializeOut(nlohmann::ordered_json& object) override;

	public:
		void SetWorld(World* pWorld) { _pWorld = pWorld; }
		bool InsertActor(Actor* pActor);
		bool AddActor(Actor* pActor);
		Actor* GetActor(_pstring name);

		template <typename T>
		std::vector<T*> FindActorsWithClassName();

		template<typename Act>
		Act* SpawnActor(int layerIndex, _pstring name, Act* pActor);

		template<typename Act>
		Act* SpawnActor(int layerIndex, _pstring name, Act* pActor, Mathf::Vector2 location);

		bool RemoveActor(_pstring name);
		bool TerminateActor(_pstring name);
		bool ReviveActor(_pstring name);

	public:
		void SetBeginPlay(bool isBeginPlay) { _isBeginPlay = isBeginPlay; _isEndPlay = !isBeginPlay; }
		bool IsBeginPlay() const { return _isBeginPlay; }
		void SetEndPlay(bool isEndPlay) { _isEndPlay = isEndPlay; _isBeginPlay = !isEndPlay; }
		bool IsEndPlay() const { return _isEndPlay; }

	public:
		void SettingLevelCamera(CameraActor* pCameraActor){ _pCameraActor = pCameraActor; }
		virtual void Destroy() override;

	protected:
		ActorsList	 _actors;
		World*		 _pWorld{ nullptr };
		CameraActor* _pCameraActor{ nullptr };
		int			 _levelIndex{ 0 };
		bool		 _isBeginPlay{ false };
		bool		 _isEndPlay{ false };

	private:
		bool spawnActor(int layerIndex, _pstring name, Actor* pActor);
		bool spawnActor(int layerIndex, _pstring name, Actor* pActor, Mathf::Vector2 location);

	};

	template<typename T>
	inline std::vector<T*> Level::FindActorsWithClassName()
	{
		static_assert(std::is_base_of_v<Actor, T>, "Is not Actor");

		std::vector<T*> list;
		for (auto actor : _actors)
		{
			if (!actor)
			{
				return list;
			}

			if (typeid(T).hash_code() == typeid(*actor).hash_code())
				list.push_back(dynamic_cast<T*>(actor));
		}
		return list;
	}

	template<typename Act>
	inline Act* Level::SpawnActor(int layerIndex, _pstring name, Act* pActor)
	{	
		static_assert(std::is_base_of_v<Engine::Actor, Act>, "Is not Actor");
		spawnActor(layerIndex, name, pActor);
		return pActor;
	}

	template<typename Act>
	inline Act* Level::SpawnActor(int layerIndex, _pstring name, Act* pActor, Mathf::Vector2 location)
	{
		static_assert(std::is_base_of_v<Engine::Actor, Act>, "Is not Actor");
		spawnActor(layerIndex, name, pActor, location);
		return pActor;
	}
}