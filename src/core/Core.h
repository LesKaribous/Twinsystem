#pragma once
#include <memory>

//Useful macros
#define BIT(x) (1 << x)
#define BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)


//Smart pointers
template<typename T>
using Scope = std::unique_ptr<T>; //Scope pointer exist only in the current scope.
template<typename T, typename ... Args>
constexpr Scope<T> CreateScope(Args&& ... args){
	return std::make_unique<T>(std::forward<Args>(args)...);
}

template<typename T>
using Shared = std::shared_ptr<T>; //Shared pointer exist as lons as you need it.
template<typename T, typename ... Args>
constexpr Shared<T> CreateShared(Args&& ... args){
	return std::make_shared<T>(std::forward<Args>(args)...);
}

