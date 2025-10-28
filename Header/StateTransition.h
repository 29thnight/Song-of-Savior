#pragma once
#include <CoreBase.h>
#include <CoreDefine.h>

namespace Engine
{
    CORECLASS()
	class StateTransition abstract : public CoreBase
	{
    public:
        using ConditionFunc = std::function<bool()>;

        virtual bool Initialize(State* from, State* to, ConditionFunc condition) abstract;
        virtual void Destroy() override abstract;

        bool CanTransition() const { return condition(); }
        State* GetNextState() const { return toState; }
		State* GetFromState() const { return fromState; }

    public:
		virtual void SerializeIn(nlohmann::ordered_json& object) {};
		virtual void SerializeOut(nlohmann::ordered_json& object) {};

    protected:
        State* fromState;
        State* toState;
        ConditionFunc condition;
	};
}
