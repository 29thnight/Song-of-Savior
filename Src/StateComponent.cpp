#include <StateComponent.h>
#include <StateMachine.h>
#include <State.h>
#include <StateTransition.h>

bool Engine::StateComponent::InitializeComponent()
{
	_stateMachine = StateMachine::Create();

    return true;
}

void Engine::StateComponent::BeginPlay()
{
}

void Engine::StateComponent::TickComponent(_duration deltaSeconds)
{
	_stateMachine->UpdateCurrentState();
}

void Engine::StateComponent::EndPlay()
{
}

void Engine::StateComponent::AddState(int stateNumber, _pstring stateName)
{
	_stateMachine->AddState(stateNumber, stateName);
}

void Engine::StateComponent::AddState(int stateNumber, State* state)
{
	_stateMachine->AddState(stateNumber, state);
}

void Engine::StateComponent::AllUnbind()
{
	_stateMachine->AllUnbind();
}

Engine::State* Engine::StateComponent::GetState(int stateNumber)
{
	return _stateMachine->GetState(stateNumber);
}

void Engine::StateComponent::SetCurrentState(int stateNumber)
{
	_stateMachine->SetCurrentState(stateNumber);
}

int Engine::StateComponent::GetCurrentStateIndex()
{
	return _stateMachine->GetCurrentState()->GetStateID();
}

void Engine::StateComponent::UpdateCurrentState(_duration deltaSeconds)
{
	_stateMachine->UpdateCurrentState();
}

void Engine::StateComponent::AddTransition(int stateNumber, StateTransition* transition)
{
	_stateMachine->GetState(stateNumber)->AddTransition(transition);
}

const std::vector<Engine::StateTransition*>& Engine::StateComponent::GetTransitions(int stateNumber) const
{
	return _stateMachine->GetState(stateNumber)->GetTransitions();
}

Engine::State* Engine::StateComponent::GetCurrentState()
{
	return _stateMachine->GetCurrentState();
}

Engine::StateComponent* Engine::StateComponent::Create()
{
	return new StateComponent();
}

void Engine::StateComponent::Destroy()
{
	SafeDelete(_stateMachine);
}
