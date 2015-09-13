#pragma once
#include <functional>
#include <list>

namespace AudioEndPoint
{
    static ULONG s_signal_id;

    template <typename... Args>
    class Signal
    {
    public:

        struct FunctionInfo
        {
            Signal<Args ...>* m_signal;
            std::function<void(Args ...)> m_function;
            ULONG m_id = ++s_signal_id;
        };

        Signal()
        {
            InitializeCriticalSectionAndSpinCount(&m_critical_section,
                0x00000400);
        }

        ~Signal()
        {
            DeleteCriticalSection(&m_critical_section);            
        }

        template <typename Observer>
        FunctionInfo Register(Observer&& observer)
        {
            EnterCriticalSection(&m_critical_section);

            FunctionInfo info;
            info.m_signal = this;
            info.m_function = std::forward<Observer>(observer);
            m_observers.push_back(info);

            LeaveCriticalSection(&m_critical_section);

            return info;
        }


        void Notify(Args ... Params)
        {
            EnterCriticalSection(&m_critical_section);
            if (m_observers.size() > 0)
            {
                for (const auto& function : m_observers)
                {
                    function.m_function(Params...);
                }
            }
            LeaveCriticalSection(&m_critical_section);
        }

        void Unregister(FunctionInfo functionInfo)
        {
            EnterCriticalSection(&m_critical_section);
            m_observers.remove_if([&functionInfo](FunctionInfo function)
                {
                    return functionInfo.m_id == function.m_id;
                });
            LeaveCriticalSection(&m_critical_section);
        }

    private:
        Signal(const Signal&) = delete;
        Signal& operator=(const Signal&) = delete;
        std::list<FunctionInfo> m_observers;
        CRITICAL_SECTION m_critical_section;
    };
}
