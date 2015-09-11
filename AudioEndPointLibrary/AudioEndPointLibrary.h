#include "DefSoundDeviceState.h"
#include "AudioDevicesContainer.h"

// This class is exported from the AudioEndPointLibrary.dll
namespace AudioEndPoint {

	class AUDIOENDPOINTLIBRARY_API CAudioEndPointLibrary {
	public:
        ~CAudioEndPointLibrary();
        static CAudioEndPointLibrary& GetInstance();
        AudioDeviceList GetPlaybackDevices(DefSound::EDeviceState state) const;
        AudioDeviceList GetRecordingDevices(DefSound::EDeviceState state) const;
    private:
        CAudioEndPointLibrary(void);
        CAudioEndPointLibrary(CAudioEndPointLibrary const&) = delete;
        void operator=(CAudioEndPointLibrary const&) = delete;
        AudioDevicesContainer m_container;
	};
}
