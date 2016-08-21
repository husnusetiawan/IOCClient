/*
	2016 Husnu Setiawan
*/
#include "Arduino.h"
#include <Ethernet.h>
#include "SPI.h"

// Length of static data buffers
#define DATA_BUFFER_LEN 120
#define SID_LEN 24

class WebSocketClient {
	public:
		typedef void (*DataArrivedDelegate)(WebSocketClient client, char *data);
		bool connect(char hostname[], int port = 80, const char protocol[] = "");
        bool connected();
        void disconnect();
		void monitor();
		void setDataArrivedDelegate(DataArrivedDelegate dataArrivedDelegate);
		void send(char *data);

	private:
        void sendHandshake(char hostname[], int port,const  char protocol[]);
        EthernetClient client;
        DataArrivedDelegate dataArrivedDelegate;
        bool readHandshake();
		void readLine();
		char *dataptr;
		char databuffer[DATA_BUFFER_LEN];
		char sid[SID_LEN];
		char *hostname;
		int port;
		const char *protocol;
		bool waitForInput(void);
		void eatHeader(void);

	protected:
		virtual void onData(char* data);
};
