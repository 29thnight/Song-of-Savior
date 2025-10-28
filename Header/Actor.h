#pragma once
#include <CoreDefine.h>
#include <Object.h>
#include <Mathf.h>
#include <IColliderReceiver.h>
#include <ObjectPtr.h>
#include <ISetupInputComponent.h>
#include <ActorComponent.h>

/* 
파생 엑터는 다음 내용을 사용

namespace Engine
{
	CORECLASS()
}

namespace Client
{
	class DerivedActor : public Engine::Actor
	{
		using Base = Engine::Actor;
	private:
		DerivedActor() = default;
		virtual ~DerivedActor() = default;

	public:
		virtual void BeginPlay() override;
		virtual void Tick(_duration deltaSeconds) override;
		virtual void Fixed() override;
		virtual void EndPlay() override;

	public:
		virtual void NotifyActorBlock(Engine::CollisionData data) override;
		virtual void NotifyActorBeginOverlap(Engine::CollisionData data) override;
		virtual void NotifyActorEndOverlap(Engine::CollisionData data) override;

	public:
		virtual void SetupInputComponent(Engine::InputComponent* inputComponent) override;

	public:
		virtual void SerializeIn(nlohmann::ordered_json& object) override;
		virtual void SerializeOut(nlohmann::ordered_json& object) override;

	public:
		static DerivedActor* Create() { return new DerivedActor; }
		void Destroy() override { Actor::Destroy(); }

	private:

	};
}

*/

namespace Engine
{
	CORECLASS()
	class Actor abstract : public Object, public IColliderReceiver, public ISetupInputComponent
	{
	protected:
		explicit Actor() = default;
		virtual ~Actor() = default;

	public:
		virtual void BeginPlay() abstract;
		virtual void Tick(_duration deltaSeconds) abstract;
		virtual void Fixed() abstract;
		virtual void EndPlay() abstract;
		virtual void NotifyActorBlock(CollisionData data) override {};
		virtual void NotifyActorBeginOverlap(CollisionData data) override {};
		virtual void NotifyActorEndOverlap(CollisionData data) override {};
		virtual void SetupInputComponent(InputComponent* inputComponent) override {};
		virtual void ReviveInitialize() {};
		virtual void SetName(_pstring name) override;

	public:
		virtual void SerializeIn(nlohmann::ordered_json& object);
		virtual void SerializeOut(nlohmann::ordered_json& object);

	public:
		template <typename T>
		T* AddComponent(_pstring name)
		{
			static_assert(std::is_base_of<ActorComponent, T>::value, "T must derive form Component");

			T* pComponent = T::Create();
			pComponent->SetOwner(this);
			pComponent->SetName(name);

			if constexpr (std::is_base_of<SceneComponent, T>::value)
			{
				if (nullptr != _pRootComponent)
				{
					SceneComponent* pSceneComponent = dynamic_cast<SceneComponent*>(pComponent);
					pSceneComponent->AttachToComponent(_pRootComponent);
				}
			}
			pComponent->InitializeComponent();
			ObjectPtr<ActorComponent> pComponentPtr(pComponent);
			_vecComponents.push_back(std::move(pComponentPtr));
			return pComponent;
		}

		template <typename T>
		T* GetComponent(_pstring name)
		{
			for (auto& component : _vecComponents)
			{
				if (!strcmp(component->GetName(),name))
					return dynamic_cast<T*>(component.get());
			}
			return nullptr;
		}

	public:
		bool operator==(_pstring name) const { return !strcmp(this->_name.c_str(), name); }
		bool operator!=(_pstring name) const { return strcmp(this->_name.c_str(), name); }

	public:
		void SetTextureName(_pwstring textureName) { _textureName = textureName; }
		bool SetRootComponent(SceneComponent* pRootComponent);
		SceneComponent* GetRootComponent() const { return _pRootComponent; }

	public:
		void SetLayerIndex(int layerIndex) { _layerIndex = layerIndex; }
		int GetLayerIndex() const { return _layerIndex; }

	public:
		void AttachToActor(Actor* pParent);
		void DetachFromActor();
		Actor* GetParent() const { return _pParent; }

	public:
		void SetWorld(World* world) { _pOwnerWorld = world; }
		World* GetWorld() const { return _pOwnerWorld; }

	public:
		void AddTexture(Texture* pTexture) { _vecTextures.push_back(pTexture); }
		size_t GetTextureSize() const { return _vecTextures.size(); }
		Texture* GetFrame(int index) const { return _vecTextures[index]; }
		Mathf::Vector2 GetBitmapSize(int index) const;

	public:
		void AddAnimation(Animation* pAnimation) { _vecAnimations.push_back(pAnimation); }
		size_t GetAnimationSize() const { return _vecAnimations.size(); }
		Animation* GetAnimation(int index) const { return _vecAnimations[index]; }

	protected:
		void PushBackComponent(ActorComponent* pComponent);

	protected:
		void Destroy() override;

	protected:
		SceneComponent*				_pRootComponent{ nullptr };
		Actor*						_pParent{ nullptr };
		World*						_pOwnerWorld{ nullptr };
		int							_layerIndex{ 0 };
		std::wstring				_textureName;

	protected:
		std::vector<Actor*>						_vecChildren;
		std::vector<ObjectPtr<ActorComponent>>	_vecComponents;
		std::vector<Texture*>					_vecTextures;
		std::vector<Animation*>					_vecAnimations;
	};
}