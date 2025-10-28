#include <State.h>
#include <Delegate.h>

void Engine::State::OnEnterState()
{
	OnEnter.Broadcast();
}

void Engine::State::OnUpdateState()
{
	OnUpdate.Broadcast();
}

void Engine::State::OnExitState()
{
	OnExit.Broadcast();
}
