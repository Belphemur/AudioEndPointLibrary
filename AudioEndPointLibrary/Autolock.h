#pragma once
namespace AudioEndPoint {
    class CAutolock
    {
    public:
        CAutolock(CRITICAL_SECTION *p_sect)
        {
            _section = p_sect;
            EnterCriticalSection(_section);
        }
        ~CAutolock()
        {
            LeaveCriticalSection(_section);
        }
    private:
        CRITICAL_SECTION *_section;
    };
}

