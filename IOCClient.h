#include "Arduino.h"
#include <Ethernet.h>
#include "SPI.h"
#include "WebSocketClient.h"
#include "ArduinoJson.h"

// Length of static data buffers
#define DATA_BUFFER_LEN 120
#define SID_LEN 24

class IOCClient : public WebSocketClient {
    public:
        IOCClient();
		typedef void (*SwitchEvent)(int switch_id, bool status);
		typedef void (*ScheduleSwitchEvent)(int switch_id, long up_time, long down_time);
		void sendData(int sensor_id, long time, float data);
		void setSwitchEvent(SwitchEvent onSwitchChanged);
		void setScheduleSwitchEvent(ScheduleSwitchEvent onScheduleSwitchChanged);

	private:
		SwitchEvent switchEvent;
		ScheduleSwitchEvent scheduleSwitchEvent;
		void onActionSwitch(JsonObject& data);
		void onActionSchedule(JsonObject& data);

	protected:
		void onData(char* data);
};
