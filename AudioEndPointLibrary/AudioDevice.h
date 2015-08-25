#pragma once
namespace AudioEndPoint {
	class AUDIOENDPOINTLIBRARY_API AudioDevice
	{
		int index;
		LPWSTR friendlyName;
		int state;
		bool isDefault;
		LPWSTR description;
		LPWSTR interfaceFriendlyName;
		LPWSTR id;

	public:


		AudioDevice(int index, LPCWSTR friendly_name, int state, bool is_default, LPCWSTR description, LPCWSTR interface_friendly_name, LPWSTR id)
			: index{ index },
			state{ state },
			isDefault{ is_default },
			id{ id }
		{
			friendlyName = _wcsdup(friendly_name);
			this->description = _wcsdup(description);
			this->interfaceFriendlyName = _wcsdup(interface_friendly_name);

		}



		int index1() const
		{
			return index;
		}

		__declspec(property(get = index1)) int Index;

		LPWSTR friendly_name() const
		{
			return friendlyName;
		}

		__declspec(property(get = friendly_name)) LPWSTR FriendlyName;

		int state1() const
		{
			return state;
		}

		__declspec(property(get = state1)) int State;

		bool is_default() const
		{
			return isDefault;
		}

		__declspec(property(get = is_default)) bool IsDefault;

		LPWSTR description1() const
		{
			return description;
		}

		__declspec(property(get = description1)) LPWSTR Description;

		LPWSTR interface_friendly_name() const
		{
			return interfaceFriendlyName;
		}

		__declspec(property(get = interface_friendly_name)) LPWSTR InterfaceFriendlyName;

		LPWSTR id1() const
		{
			return id;
		}

		__declspec(property(get = id1)) LPWSTR Id;
		void SetDefault();
		AudioDevice();
		~AudioDevice();
	};
}

