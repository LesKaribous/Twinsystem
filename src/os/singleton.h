#pragma once

#define SINGLETON(X) public: \
    static inline X& instance(){return m_instance;} \
    X(const X&) = delete; \
    X(X&&) = delete; \
    X& operator=(const X&) = delete; \
    X& operator=(X&&) = delete; \
private: \
    static X m_instance;

#define SINGLETON_INSTANTIATE(className, name) className className::m_instance; className& name = className::instance();
#define SINGLETON_EXTERN(className, name) extern className& name;