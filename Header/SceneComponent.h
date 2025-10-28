#pragma once
#include <CoreDefine.h>
#include <ActorComponent.h>
#include <Mathf.h>
#include <Transform.h>

namespace Engine
{
	CORECLASS()
	class SceneComponent : public ActorComponent, public Transform
	{
	protected:
		explicit SceneComponent() = default;
		virtual ~SceneComponent() = default;

	public:
		virtual bool InitializeComponent() override { return true; }
		virtual void BeginPlay() override {};
		virtual void TickComponent(_duration deltaSeconds) override;
		virtual void EndPlay() override {};

	public:
		virtual void SerializeIn(nlohmann::ordered_json& object);
		virtual void SerializeOut(nlohmann::ordered_json& object);

	public:
		//SceneComponent
		void AttachToComponent(SceneComponent* pParent);

	public:
		//Transform
		void UpdateTransform() override;
		void UpdateVelocity() override;

	protected:
		virtual void Destroy() override { };

	protected:
		SceneComponent*				 _parent = nullptr;
		std::vector<SceneComponent*> _children;

	public:
		static SceneComponent* Create() { return new SceneComponent; }

	};
}