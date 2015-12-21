#pragma once
#include <functional>
#include <list>
#include "Autolock.h"

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
            !InitializeCriticalSectionAndSpinCount(&this->_section, 0x00000400);
        }

        ~Signal()
        {
               
        }

        template <typename Observer>
        FunctionInfo Register(Observer&& observer)
        {

            CAutolock(&this->_section);
            FunctionInfo info;
            info.m_signal = this;
            info.m_function = std::forward<Observer>(observer);
            m_observers.push_back(info);



            return info;
        }


        void Notify(Args ... Params)
        {
            CAutolock(&this->_section);
            if (m_observers.size() > 0)
            {
                for (const auto& function : m_observers)
                {
                    function.m_function(Params...);
                }
            }

        }

        void Unregister(FunctionInfo functionInfo)
        {
            CAutolock(&this->_section);
            m_observers.remove_if([&functionInfo](FunctionInfo function)
                {
                    return functionInfo.m_id == function.m_id;
                });

        }

    private:
        Signal(const Signal&) = delete;
        Signal& operator=(const Signal&) = delete;
        std::list<FunctionInfo> m_observers;
        CRITICAL_SECTION _section;
    };
}
