#pragma once
#include <CoreBase.h>
#include <CoreDefine.h>
#include <Delegate.h>

namespace Engine
{
	CORECLASS()
	class State : public CoreBase
	{
	public:
		explicit State() = default;
		virtual ~State() = default;

	public:
		virtual void Destroy() override {}

	public:
		virtual void SerializeIn(nlohmann::ordered_json& object) {};
		virtual void SerializeOut(nlohmann::ordered_json& object) {};

		void OnEnterState();
		void OnUpdateState();
		void OnExitState();


		void SetStateName(const string& name) { stateName = name; }
		void SetOwner(Actor* owner) { pOwner = owner; }
		void SetStateID(int id) { stateID = id; }

		Actor* GetOwner() const { return pOwner; }
		int GetStateID() const { return stateID; }

		void AddTransition(StateTransition* transition) { _transitions.push_back(transition); }
		const std::vector<StateTransition*>& GetTransitions() const { return _transitions; }

	public:
		Delegate<>		 OnEnter;
		Delegate<>	     OnUpdate;
		Delegate<>       OnExit;

	private:
		std::vector<StateTransition*> _transitions;
		Actor*	pOwner{ nullptr };
		int		stateID{ 0 };
		string	stateName;
	};
}