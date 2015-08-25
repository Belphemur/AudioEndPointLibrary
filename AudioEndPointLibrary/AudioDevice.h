#pragma once
namespace AudioEndPoint {
	class AUDIOENDPOINTLIBRARY_API AudioDevice
	{
		LPWSTR friendlyName;
		bool* isDefault;
		LPWSTR description;
		LPWSTR interfaceFriendlyName;
		LPWSTR id;

	public:


		void SetDefault(ERole role);
		AudioDevice();
		~AudioDevice();
	};
}

