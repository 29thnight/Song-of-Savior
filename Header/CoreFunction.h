#pragma once
#include <CoreBase.h>
#include <CoreDefine.h>
#include <Texture.h>

namespace Engine
{
	CORECLASS()

	template <typename T>
	void SafeDelete(T*& ptr)
	{
		if (ptr)
		{
			ptr->Terminate();
			ptr = nullptr;
		}
	}

	template <typename T>
	void SafeRelease(T*& ptr)
	{
		if (ptr)
		{
			ptr->Release();
			ptr = nullptr;
		}
	}

	template <typename T>
	ULONG GetRefCount(const SmartPtr<T>& p)
	{
		T* temp = p.get();

		ULONG ret = 0;
		if (temp != nullptr)
		{
			ret = temp->AddRef();
			ret = temp->Release();
		}
	
		return ret;
	}
}