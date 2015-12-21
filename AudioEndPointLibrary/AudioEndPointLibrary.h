#include "DefSoundDeviceState.h"
#include "AudioEndPointLibraryImpl.h"


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
	    AudioDeviceList GetPlaybackDevices(DefSound::EDeviceState state);
        AudioDeviceList GetRecordingDevices(DefSound::EDeviceState state);
        HRESULT RegisterNotificationClient();
        HRESULT UnRegisterNotificationClient() const;

	    AudioEndPointLibrarySignals& m_signals1()
	    {
	        return m_signals;
	    }

	    __declspec(property(get = m_signals1)) AudioEndPointLibrarySignals Signals;
    private:
        CAudioEndPointLibrary(void);
        CAudioEndPointLibrary(CAudioEndPointLibrary const&) = delete;
        void operator=(CAudioEndPointLibrary const&) = delete;
        void Refresh();

        AudioEndPointLibraryImpl m_container;
        AudioEndPointLibrarySignals m_signals;
        AudioEndPointLibraryDevicesImpl m_devices_lists;
	};


}
