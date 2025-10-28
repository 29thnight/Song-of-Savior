#pragma once
#include <CoreBase.h>
#include <CoreDefine.h>

namespace Engine
{
    CORECLASS()
	class StateMachine : public CoreBase
    {
    public:
        void AddState(int stateNumber, string StateName);
        void AddState(int stateNumber, State* state);
        State* GetState(int index);
        void SetCurrentState(int index);
        void UpdateCurrentState();
		State* GetCurrentState() const { return currentState; }

        void AllUnbind();

    public:
		virtual void SerializeIn(nlohmann::ordered_json& object) {};
		virtual void SerializeOut(nlohmann::ordered_json& object) {};

    public:
		static StateMachine* Create() { return new StateMachine(); }
        virtual void Destroy() override;

    private:
        void CheckTransitions();
        void SetCurrentState(State* state);

		std::unordered_map<int, State*> states;
        State* currentState = nullptr;
    };
}