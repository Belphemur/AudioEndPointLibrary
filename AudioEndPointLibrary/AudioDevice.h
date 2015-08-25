#pragma once
#include "DefSoundEndpointColl.h"
namespace AudioEndPoint {
	class AUDIOENDPOINTLIBRARY_API AudioDevice
	{
		LPWSTR friendlyName;
		bool* isDefault;
		LPWSTR description;
		LPWSTR interfaceFriendlyName;
		LPWSTR id;
		DefSound::CEndpoint end_point_;

	public:


		void SetDefault(ERole role);
		AudioDevice();
		~AudioDevice();
	};
}

