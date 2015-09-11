#pragma once
#include <memory>

using namespace System::Threading;

ref class Lock
{
    Object^ m_pObject;
public:
    Lock(Object ^ pObject) : m_pObject(pObject)
    {
        Monitor::Enter(m_pObject);
    }
    ~Lock()
    {
        Monitor::Exit(m_pObject);
    }
};
template <typename T>
ref struct SharedNativePtr
{
private:
    std::shared_ptr<T>* m_sharedPtr;
public:
    SharedNativePtr() : m_sharedPtr(__nullptr) {}

    ~SharedNativePtr()
    {
        Reset();
    }

    inline bool IsValid()
    {
        return m_sharedPtr;
    }

    inline T* operator->()
    {
        return m_sharedPtr->get();
    }

    !SharedNativePtr()
    {
        Reset();
    }

    std::shared_ptr<T> Get()
    {
        return std::shared_ptr<T>(*m_sharedPtr);
    }

    std::shared_ptr<T>& GetRef()
    {
        return *m_sharedPtr;
    }

    void Reset()
    {
        Lock lock(this);
        if (m_sharedPtr)
        {
            delete m_sharedPtr;
            m_sharedPtr = __nullptr;
        }
    }
    void Reset(std::shared_ptr<T>& ptr)
    {
        Lock lock(this);
        if (!m_sharedPtr || ptr != *m_sharedPtr)
        {
            if (m_sharedPtr)
            {
                delete m_sharedPtr;
                m_sharedPtr = __nullptr;
            }
            m_sharedPtr = new std::shared_ptr<T>(ptr);
        }
    }
};
