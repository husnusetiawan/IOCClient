#include <WebSocketClient.h>

#define HEARTBEAT_INTERVAL 15000UL
unsigned long lastheartbeat;

bool WebSocketClient::connect(char thehostname[], int theport,const char theprotocol[]) {
	if (!client.connect(thehostname, theport)) return false;
	hostname = thehostname;
	protocol = theprotocol;
	port = theport;
	sendHandshake(hostname,port,protocol);
	return readHandshake();
}

bool WebSocketClient::connected() {
	return client.connected();
}

void WebSocketClient::disconnect() {
	client.stop();
}

void WebSocketClient::monitor() {

	// heart beat
	unsigned long now = millis();
	if ((now - lastheartbeat) >= HEARTBEAT_INTERVAL) {
		lastheartbeat = now;
		if (client.connected()){
			const char *data = "{}";
			client.print((char)-127);
			client.print((char)strlen(data));
			client.print(data);
		}
	}

	*databuffer = 0;

	if (!client.connected()) {
		if (!client.connect(hostname, port)) return;
	}

	if (!client.available()) { return;}

	char which;
	while (client.available()) {
		readLine();
		dataptr = databuffer;
	}

	size_t sizes = strlen(databuffer);

	char* new_data = new char[sizes-2];
	for (int i = 0; i< sizes-2; i+=1){
	new_data[i] = databuffer[i+2];
	}
	new_data[sizes-2] = '\0';
	onData(new_data);

}

void WebSocketClient::onData(char* new_data){
    if (dataArrivedDelegate != NULL) 
		dataArrivedDelegate(*this, new_data);
}

void WebSocketClient::setDataArrivedDelegate(DataArrivedDelegate newdataArrivedDelegate) {
	  dataArrivedDelegate = newdataArrivedDelegate;
}

void WebSocketClient::sendHandshake(char hostname[],int port,const char protocol[]) {
	client.print(F("GET /"));
	client.println(F(" HTTP/1.1"));
	client.print(F("Host: "));
	client.print(F("ws://"));
	client.print(hostname);
	client.print(F(":"));
	client.print(port);
	client.println(F(""));
	client.println(F("Origin: *"));
	client.println(F("Upgrade: WebSocket"));	// must be camelcase ?!
	client.print(F("Sec-WebSocket-Key: "));
	client.println("258EAFA5-E914-47DA-95CA-C5AB0DC85B11");
	client.print(F("Sec-WebSocket-Version: "));
	client.println(F("13"));
	client.print(F("Sec-WebSocket-Protocol: "));
	client.println(protocol);
	client.println(F("Connection: Upgrade\r\n"));
}

bool WebSocketClient::waitForInput(void) {
unsigned long now = millis();
	while (!client.available() && ((millis() - now) < 30000UL)) {;}
	return client.available();
}

void WebSocketClient::eatHeader(void) {
	while (client.available()) {	// consume the header
		readLine();
		if (strlen(databuffer) == 0) break;
	}
}

bool WebSocketClient::readHandshake() {

	if (!waitForInput()) return false;

	readLine();
	if (atoi(&databuffer[9]) != 101) {
		while (client.available()) readLine();
		client.stop();
		return false;
	}
	eatHeader();
	monitor();		// treat the response as input
	return true;
}

void WebSocketClient::readLine() {
	dataptr = databuffer;
	while (client.available() && (dataptr < &databuffer[DATA_BUFFER_LEN-2])) {
		char c = client.read();
		if (c == 0) Serial.print(F("NULL"));
		else if (c == 255) Serial.print(F("0x255"));
		else if (c == '\r') {;}
		else if (c == '\n') break;
		// else if (c < 20) {;} // clear character ....
		else
			*dataptr++ = c;
	}
	*dataptr = 0;
}

void WebSocketClient::send(char *data){
	client.print((char)-127);
	client.print((char)strlen(data));
	client.print(data);
}
