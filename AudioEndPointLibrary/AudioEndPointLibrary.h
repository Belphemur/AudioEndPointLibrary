#include <list>
#include "AudioDevice.h"
#include "DefSoundEndpointColl.h"

// This class is exported from the AudioEndPointLibrary.dll
namespace AudioEndPoint {
	typedef std::shared_ptr<AudioDevice> AudioDevicePtr;
	typedef std::list<AudioDevicePtr> AudioDeviceList;
	typedef std::unique_ptr<AudioDeviceList> AudioDeviceListPtr;

	class AUDIOENDPOINTLIBRARY_API CAudioEndPointLibrary {
		CAudioEndPointLibrary(void);
	public:
		static AudioDeviceListPtr GetAudioDevices(DefSound::EDeviceState state);
	};
}
