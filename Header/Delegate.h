#pragma once
#include <CoreDefine.h>

template <typename... Args>
class Delegate {
public:
    using FuncType = std::function<void(Args...)>;

    void Bind(FuncType func) {
        functions.push_back(func);
    }

    template <typename T>
    void AddDynamic(T* obj, void (T::*func)(Args...)) {
        Bind([=](Args... args) {
            (obj->*func)(args...);
        });
    }

    template <typename T, typename U>
    void AddDynamic(T* obj, void (U::*func)(Args...)) {
        Bind([=](Args... args) {
            (obj->*func)(args...);
        });
    }

	template <typename T>
	void AddLambda(T func) {
		Bind([=](Args... args) {
			func(args...);
			});
	}

    void Broadcast(Args... args) {
        for (auto& func : functions) {
            func(args...);
        }
    }

	void Unbind() {
		for (auto& func : functions)
			func = nullptr;
	}

private:
    std::vector<FuncType> functions;
};