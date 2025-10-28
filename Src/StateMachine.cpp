#include <StateMachine.h>
#include <State.h>
#include <StateTransition.h>

void Engine::StateMachine::AddState(int stateNumber, string StateName)
{
	State* state = new State();
	state->SetStateID(stateNumber);
	state->SetStateName(StateName);
	states[stateNumber] = state;
}

void Engine::StateMachine::AddState(int stateNumber, State* state)
{
	state->SetStateID(stateNumber);
	states[stateNumber] = state;
}

Engine::State* Engine::StateMachine::GetState(int index)
{
    return states[index];
}

void Engine::StateMachine::SetCurrentState(int index)
{
    if (currentState != nullptr) 
    {
        currentState->OnExitState();
    }
    currentState = GetState(index);
    if (currentState != nullptr) 
    {
        currentState->OnEnterState();
    }
}

void Engine::StateMachine::UpdateCurrentState()
{
    if (currentState != nullptr) 
    {
        currentState->OnUpdateState();
        CheckTransitions();
    }
}

void Engine::StateMachine::AllUnbind()
{
    for(auto& state : states)
    {
		state.second->OnEnter.Unbind();
		state.second->OnUpdate.Unbind();
		state.second->OnExit.Unbind();
    }
}

void Engine::StateMachine::Destroy()
{
	for (auto state : states)
	{
		SafeDelete(state.second);
	}
	states.clear();
	currentState = nullptr;
}

void Engine::StateMachine::CheckTransitions()
{
    for (auto transition : currentState->GetTransitions()) 
    {
        if (transition->CanTransition()) 
        {
            SetCurrentState(transition->GetNextState());
            break;
        }
    }
}

void Engine::StateMachine::SetCurrentState(State* state)
{
    if (currentState != nullptr) 
    {
        currentState->OnExitState();
    }
    currentState = state;
    if (currentState != nullptr) 
    {
        currentState->OnEnterState();
    }
}
