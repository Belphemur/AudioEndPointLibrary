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
        AudioDeviceList GetPlaybackDevices(DefSound::EDeviceState state) const;
        AudioDeviceList GetRecordingDevices(DefSound::EDeviceState state) const;
        HRESULT RegisterNotificationClient();
        HRESULT UnRegisterNotificationClient();

	    AudioEndPointLibrarySignals& m_signals1()
	    {
	        return m_signals;
	    }

	    __declspec(property(get = m_signals1)) AudioEndPointLibrarySignals Signals;
	    void Refresh();
    private:
        CAudioEndPointLibrary(void);
        CAudioEndPointLibrary(CAudioEndPointLibrary const&) = delete;
        void operator=(CAudioEndPointLibrary const&) = delete;

        AudioEndPointLibraryImpl m_container;
        AudioEndPointLibrarySignals m_signals;
	};


}
