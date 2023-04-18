#pragma once
#include <Arduino.h>
#include <memory>

//Useful macros
#define BIT(x) (1 << x)

template<typename T>
using Shared = std::shared_ptr<T>; //Shared pointer exist as lons as you need it.
template<typename T, typename ... Args>
constexpr Shared<T> CreateShared(Args&& ... args){
	return std::make_shared<T>(std::forward<Args>(args)...);
}

