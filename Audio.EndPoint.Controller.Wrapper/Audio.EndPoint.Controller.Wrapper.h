// Audio.EndPoint.Controller.Wrapper.h

#pragma once
#include "IAudioDevice.h"
#include "Events.h"

using namespace System;
using namespace System::Collections::Generic;

namespace AudioEndPointControllerWrapper {


	public ref class AudioController
	{
	public:
        static event System::EventHandler<DeviceAddedEvent^>^ DeviceAdded;
        static event System::EventHandler<DeviceRemovedEvent^>^ DeviceRemoved;
        static event System::EventHandler<DeviceStateChangedEvent^>^ DeviceStateChanged;
        static event System::EventHandler<DeviceDefaultChangedEvent^>^ DeviceDefaultChanged;       

		static List<IAudioDevice^>^ GetActivePlaybackDevices();
		static List<IAudioDevice^>^ GetAllPlaybackDevices();
		static List<IAudioDevice^>^ GetPlaybackDevices(DeviceState state);
		static List<IAudioDevice^>^ GetRecordingDevices(DeviceState state);
		static List<IAudioDevice^>^ GetActiveRecordingDevices();
		static List<IAudioDevice^>^ GetAllRecordingDevices();

        static Boolean^ IsNotificationAvailable();

    internal:
        static void RaiseAdded(DeviceAddedEvent^ devEvent);
        static void RaiseRemoved(DeviceRemovedEvent^ devEvent);
        static void RaiseState(DeviceStateChangedEvent^ devEvent);
        static void RaiseDefault(DeviceDefaultChangedEvent^ devEvent);
    private:
        AudioController() {};

	};
}
