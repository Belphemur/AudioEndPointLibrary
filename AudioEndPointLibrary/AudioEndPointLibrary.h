#include "DefSoundDeviceState.h"
#include "AudioEndPointLibraryImpl.h"
#include <utility>


// This class is exported from the AudioEndPointLibrary.dll
namespace AudioEndPoint {
	class AUDIOENDPOINTLIBRARY_API CAudioEndPointLibrary {
	public:
        ~CAudioEndPointLibrary();
	    HRESULT OnDeviceStateChanged(LPCWSTR pwstr_device_id, DWORD dw_new_state);
	    HRESULT OnDeviceAdded(LPCWSTR pwstr_device_id);
	    HRESULT OnDeviceRemoved(LPCWSTR pwstr_device_id);
	    HRESULT OnDefaultDeviceChanged(EDataFlow flow, ERole role, LPCWSTR pwstr_default_device_id);
	    static CAudioEndPointLibrary& GetInstance();
        AudioDeviceList GetPlaybackDevices(DefSound::EDeviceState state) const;
        AudioDeviceList GetRecordingDevices(DefSound::EDeviceState state) const;

	    IMMNotificationClientPtr& GetNotifClient();

        template <typename Observer>
        void RegisterObserver(const Event& event, Observer&& observer)
        {
            m_container.m_observers[event].push_back(std::forward<Observer>(observer));
        }

        template <typename Observer>
        void RegisterObserver(Event&& event, Observer&& observer)
        {
            m_container.m_observers[std::move(event)].push_back(std::forward<Observer>(observer));
        }
	    void Refresh();
    private:
        CAudioEndPointLibrary(void);
        CAudioEndPointLibrary(CAudioEndPointLibrary const&) = delete;
        void operator=(CAudioEndPointLibrary const&) = delete;
        void notify(const Event& event, AudioDevice* device) const;

        AudioEndPointLibraryImpl m_container;
	};


}
