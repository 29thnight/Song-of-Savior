#pragma once 
#include <CoreBase.h>
#include <CoreDefine.h>
#include <CoreFunction.h>

namespace Engine
{
	CORECLASS()
}

// Type trait to ensure that T is derived from CoreBase
template <typename T>
struct IsCoreBaseDerived
{
    static constexpr bool value = std::is_base_of<Engine::CoreBase, T>::value;
};

template <typename T, typename = std::enable_if_t<IsCoreBaseDerived<T>::value>>
class ObjectPtr final
{
public:
    constexpr ObjectPtr() noexcept : _ptr(nullptr), _refCount(nullptr) {}
    constexpr ObjectPtr(std::nullptr_t) noexcept : _ptr(nullptr), _refCount(nullptr) {}
    explicit ObjectPtr(T* ptr) noexcept : _ptr(ptr), _refCount(ptr ? new std::atomic<long>(1) : nullptr) {}
    ObjectPtr(const ObjectPtr& ptr) noexcept : _ptr(ptr._ptr), _refCount(ptr._refCount) 
    {
		if (_refCount)
		{
			(*_refCount)++;
		}
    }
    ObjectPtr(ObjectPtr&& ptr) noexcept : _ptr(std::exchange(ptr._ptr, nullptr)), _refCount(std::exchange(ptr._refCount, nullptr)) {}
	~ObjectPtr() noexcept { releaseIfUnreferenced(); }

public:
	ObjectPtr& operator=(std::nullptr_t) noexcept 
	{ 
        reset();
		return *this; 
	}

	ObjectPtr& operator=(T* ptr) noexcept 
	{ 
        if (_ptr != ptr)
        {
            ObjectPtr(ptr).swap(*this);  // Temporary ObjectPtr for safe resource management
        }
        return *this;
	}

	ObjectPtr& operator=(const ObjectPtr& ptr) noexcept
	{ 
        if (this != &ptr)
        {
            ObjectPtr(ptr).swap(*this);  // Temporary ObjectPtr for safe resource management
        }
        return *this;
	}

	ObjectPtr& operator=(ObjectPtr&& ptr) noexcept
	{ 
        if (this != &ptr)
        {
            reset();
            _ptr = std::exchange(ptr._ptr, nullptr);
            _refCount = std::exchange(ptr._refCount, nullptr);
        }
        return *this; 
	}

public:
	T* operator->() const noexcept { return _ptr; }
	T& operator*() const noexcept { return *_ptr; }
	operator bool() const noexcept { return _ptr != nullptr; }

public:
	bool operator==(const ObjectPtr& ptr) const noexcept { return _ptr == ptr._ptr; }

public:
	T* get() const noexcept { return _ptr; }

    long use_count() const noexcept
    {
        return _refCount ? _refCount->load() : 0;
    }

    void swap(ObjectPtr& other) noexcept
    {
        std::swap(_ptr, other._ptr);
        std::swap(_refCount, other._refCount);
    }

public:
	void reset(T* ptr = nullptr) noexcept
    {
        if (_ptr != ptr)
        {
            ObjectPtr(ptr).swap(*this);  // Temporary ObjectPtr for safe resource management
        }
    }

    void reset(T* ptr, std::atomic<long>* refCount) noexcept
    {
        if (_ptr != ptr)
        {
            ObjectPtr(ptr, refCount).swap(*this);  // Temporary ObjectPtr for safe resource management
        }
    }

private:
	void releaseIfUnreferenced() noexcept
	{
        if (_refCount && --(*_refCount) == 0)
        {
            Engine::SafeDelete(_ptr);
            delete _refCount;
        }
	}

private:
	T* _ptr{ nullptr };
    std::atomic<long>* _refCount{ nullptr };
};

template <typename T>
inline ObjectPtr<T> MakeObjectPtr()
{
	static_assert(std::is_base_of<Engine::CoreBase, T>::value, "T must Has Terminate()");
	return ObjectPtr<T>();
}

template <typename T, typename U>
inline ObjectPtr<T> MakeObjectPtr(U* ptr)
{
	static_assert(std::is_base_of<Engine::CoreBase, T>::value, "T must Has Terminate()");
	return ObjectPtr<T>(ptr);
}

template <typename T>
inline ObjectPtr<T> MakeObjectPtr(const ObjectPtr<T>& ptr)
{
	static_assert(std::is_base_of<Engine::CoreBase, T>::value, "T must Has Terminate()");
	return ObjectPtr<T>(ptr);
}

template <typename T>
inline ObjectPtr<T> MakeObjectPtr(ObjectPtr<T>&& ptr)
{
	static_assert(std::is_base_of<Engine::CoreBase, T>::value, "T must Has Terminate()");
	return ObjectPtr<T>(std::move(ptr));
}