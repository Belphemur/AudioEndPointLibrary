#include "DefSoundDeviceState.h"
#include "AudioDevicesContainer.h"


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

	    IMMNotificationClientPtr& GetNotifClient()
	    {
	        return m_container.m_notif_client;
	    }

	    void Refresh();
    private:
        CAudioEndPointLibrary(void);
        CAudioEndPointLibrary(CAudioEndPointLibrary const&) = delete;
        void operator=(CAudioEndPointLibrary const&) = delete;
        AudioDevicesContainer m_container;
	};
}
