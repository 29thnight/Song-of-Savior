#pragma once
#include <CoreDefine.h>
#include <ActorComponent.h>
#include <State.h>

namespace Engine
{
	CORECLASS()
	class StateComponent : public ActorComponent
	{
	protected:
		explicit StateComponent() = default;
		virtual ~StateComponent() = default;

	public:
		virtual bool InitializeComponent() override;
		virtual void BeginPlay() override;
		virtual void TickComponent(_duration deltaSeconds) override;
		virtual void EndPlay() override;

	public:
		virtual void SerializeIn(nlohmann::ordered_json& object) {};
		virtual void SerializeOut(nlohmann::ordered_json& object) {};

		template<typename T>
		void AddState(int stateNumber, T* state)
		{
			static_assert(std::is_base_of<State, T>::value, "T is not derived from State");

			State* stateConvert = dynamic_cast<State*>(state);
			stateConvert->SetOwner(_owner);
			AddState(stateNumber, stateConvert);
		}

		template<typename T>
		T* GetState(int stateNumber)
		{
			static_assert(std::is_base_of<T, State>::value, "T is not derived from State");
			State* state = GetState(stateNumber);

			return dynamic_cast<T*>(state);
		}

	public:
		void AddState(int stateNumber, _pstring stateName);
		void AddState(int stateNumber, State* state);
		void AllUnbind();
		State* GetState(int stateNumber);
		void SetCurrentState(int stateNumber);
		int GetCurrentStateIndex();
		void UpdateCurrentState(_duration deltaSeconds);

		void AddTransition(int stateNumber, StateTransition* transition);
		const std::vector<StateTransition*>& GetTransitions(int stateNumber) const;

		State* GetCurrentState();
	public:
		static StateComponent* Create();
		virtual void Destroy() override;

	private:
		StateMachine* _stateMachine{ nullptr };
	};
}