#pragma once
#include <CoreDefine.h>

template <typename T>
class Singleton
{
	_SINGLETON_OPERATOR(Singleton)

protected:
	Singleton() = default;
	virtual ~Singleton() = default;

public:
	static T* GetInstance()
	{
		if (!_pInstance)
			_pInstance = new T;

		return _pInstance;
	}

private:
	static T* _pInstance;
};

template <typename T>
T* Singleton<T>::_pInstance = nullptr;