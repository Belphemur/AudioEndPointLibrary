#pragma once
#include "AudioDeviceType.h"
#include <DefSoundEndpoint.h>
#include <DefSoundEndpointRole.h>
#include <mmdeviceapi.h>

namespace AudioEndPoint {
	class AUDIOENDPOINTLIBRARY_API AudioDevice
	{
		DefSound::CEndpoint endpoint;
		AudioDeviceType type;

	public:


		explicit AudioDevice(const DefSound::CEndpoint& endpoint, AudioDeviceType type = Playback)
			: endpoint{endpoint},
			  type{type}
		{
		}

		LPCWSTR GetFriendlyName()
		{
			return endpoint.m_FriendlyName.c_str();
		}

		LPCWSTR GetDescription()
		{
			return endpoint.m_DeviceDesc.c_str();
		}

		LPCWSTR GetID()
		{
			return endpoint.m_DeviceId.c_str();
		}

		LPCWSTR GetDeviceClassIconPath()
		{
			return endpoint.m_DeviceClassIconPath.c_str();
		}

		DefSound::EDeviceState GetDeviceState()
		{
			return endpoint.m_State.state;
		}

		AudioDeviceType GetType() const
		{
			return type;
		}

        DefSound::CEndpoint& GetEndPoint()
		{
            return endpoint;
		}

		__declspec(property(get = GetType)) AudioDeviceType Type;
		__declspec(property(get = GetFriendlyName)) LPCWSTR FriendlyName;
		__declspec(property(get = GetDescription)) LPCWSTR Description;
		__declspec(property(get = GetID)) LPCWSTR ID;
		__declspec(property(get = GetDeviceClassIconPath)) LPCWSTR DeviceClassIconPath;
		__declspec(property(get = GetDeviceState)) DefSound::EDeviceState DeviceState;


		
		void SetDefault(ERole role) const;
		bool IsDefault(ERole role) const;
		~AudioDevice();
	};
}

