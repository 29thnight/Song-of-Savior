#pragma once
#include <CoreDefine.h>

template <typename T>
class Random
{
public:
    static_assert(std::is_arithmetic<T>::value, "Type T must be arithmetic.");

    Random(T min, T max)
        : _generator(_device()), _distribution(min, max)
    {
    }

    std::vector<T> Generate(size_t count)
    {
        std::vector<T> numbers;
        numbers.reserve(count);
        for (size_t i = 0; i < count; ++i)
        {
            numbers.push_back(_distribution(_generator));
        }
        return numbers;
    }

    T Generate() 
	{
        T result = _distribution(_generator);
        return result;
	}

private:
    std::random_device  _device;
    std::mt19937        _generator;
    std::conditional_t<std::is_integral<T>::value, std::uniform_int_distribution<T>, std::uniform_real_distribution<T>> _distribution;
};