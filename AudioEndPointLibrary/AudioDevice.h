#pragma once
#include "DefSoundEndpointColl.h"
namespace AudioEndPoint {
	class AUDIOENDPOINTLIBRARY_API AudioDevice
	{
		DefSound::CEndpoint endpoint;

	public:
		explicit AudioDevice(const DefSound::CEndpoint endpoint)			
		{
			this->endpoint = endpoint;
		}

		inline LPCWSTR GetFriendlyName()
		{
			return endpoint.m_FriendlyName.c_str();
		}

		inline LPCWSTR GetDescription()
		{
			return endpoint.m_DeviceDesc.c_str();
		}

		inline LPCWSTR GetID()
		{
			return endpoint.m_DeviceId.c_str();
		}

		inline LPCWSTR GetDeviceClassIconPath()
		{
			return endpoint.m_DeviceClassIconPath.c_str();
		}

		__declspec(property(get = GetFriendlyName)) LPCWSTR FriendlyName;
		__declspec(property(get = GetDescription)) LPCWSTR Description;
		__declspec(property(get = GetID)) LPCWSTR ID;
		__declspec(property(get = GetDeviceClassIconPath)) LPCWSTR DeviceClassIconPath;

		
		void SetDefault(ERole role);
		bool IsDefault(ERole role);
		~AudioDevice();
	};
}

