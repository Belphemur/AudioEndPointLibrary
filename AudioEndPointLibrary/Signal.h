#pragma once
#include <functional>
#include <list>
#include <mutex> 

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

        Signal() = default;
        virtual ~Signal() = default;

        template <typename Observer>
        FunctionInfo Register(Observer&& observer)
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            FunctionInfo info;
            info.m_signal = this;
            info.m_function = std::forward<Observer>(observer);
            m_observers.push_back(info);

            return info;
        }


        void Notify(Args ... Params)
        {
            std::lock_guard<std::mutex> lock(m_mutex);
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
            std::lock_guard<std::mutex> lock(m_mutex);
            m_observers.remove_if([&functionInfo](FunctionInfo function)
                {
                    return functionInfo.m_id == function.m_id;
                });
        }

    private:
        Signal(const Signal&) = delete;
        Signal& operator=(const Signal&) = delete;
        std::list<FunctionInfo> m_observers;
        std::mutex m_mutex;
    };
}
