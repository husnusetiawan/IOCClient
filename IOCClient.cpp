#include <IOCClient.h>
#include "ArduinoJson.h"
 
void IOCClient::onActionSwitch(JsonObject& data){

	int switch_id = data["id"];
	bool status = data["status"];

	if (switchEvent != NULL)
		switchEvent(switch_id, status);
}

void IOCClient::onActionSchedule(JsonObject& data){

	int switch_id = data["id"];
	long up_time = data["up_time"];
	long down_time = data["down_time"];

	if (switchEvent != NULL)
		scheduleSwitchEvent(switch_id, up_time, down_time);
}

void IOCClient::onData(char *buffer_data) {  

	StaticJsonBuffer<200> jsonBuffer;
	JsonObject& root = jsonBuffer.parseObject(buffer_data);

	const char* action = root["action"];
	JsonObject& data = root["data"];

	if (strcmp(action, "switch") == 0){
		onActionSwitch(data);
	}
	else if (strcmp(action, "schedule") == 0){
		onActionSchedule(data);
	}
}

IOCClient::IOCClient()
{

}

void IOCClient::sendData(int sensor_id, long time, float data)
{
	StaticJsonBuffer<300> jsonBuffer;

	JsonObject& root = jsonBuffer.createObject();
	root["action"] = "data";

	JsonObject& data_json = jsonBuffer.createObject();

	data_json["id"] = sensor_id;
	data_json["time"] = time;
	data_json["data"] = data;

	root["data"] = data_json;

    int len = root.measureLength() + 1;
    char rootstr[len];
    root.printTo(rootstr, len);
    rootstr[len + 1] = '\0';
	send(rootstr);
}

void IOCClient::setSwitchEvent(SwitchEvent onSwitchChanged){
	switchEvent = onSwitchChanged;
}

void IOCClient::setScheduleSwitchEvent(ScheduleSwitchEvent onScheduleSwitchChanged){
	scheduleSwitchEvent = onScheduleSwitchChanged;
}