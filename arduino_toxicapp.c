/*

 Author: Dawid Poslinski http://github.com/poslinskinet

 Photoresistor Part (Connection)
 http://nakkaya.com/images/post/photoresistor.png

 Connect the photoresistor one leg to pin 0, and pin to +5V
 Connect a resistor (around 10k is a good value, higher
 values gives higher readings) from pin 0 to GND.

 ----------------------------------------------------

 PhotoR     10K
 +5    o---/\/\/--.--/\/\/---o GND
 |
 Pin 0 o-----------

 ----------------------------------------------------

 WiFi Part (Connection)
 http://www.whatimade.today/content/images/2015/01/ESP8266_arduino_bb.jpg
 */

// CONFIGURATION START

#include <SoftwareSerial.h>

#define SSID "wifi_id"
#define PASSWORD "secret"

#define DST_IP "domain.com"

SoftwareSerial esp8266(10, 11);

int roomNumber=2;

// CONFIGURATION END

int lightPin=0;           //define a pin for Photo resistor
int ledPin=13;            //define a pin for LED
int resetPin=12;			// connect D12 pin with reset
int brightSignalLimit=350; // define when photoresistor should change state

boolean lastOptoState = false;
boolean debug = true;

String string = "";
char ch;

void setup()
{
	digitalWrite(resetPin, HIGH);
	pinMode(resetPin, OUTPUT);

	Serial.begin(9600);  //Begin serial communication
	esp8266.begin(9600);

	Serial.println("App start...");
	delay(5000);
	Serial.println("Start in 5...");
	delay(1000);
	Serial.println("4...");
	delay(1000);
	Serial.println("3...");
	delay(1000);
	Serial.println("2...");
	delay(1000);
	Serial.println("1...");
	delay(1000);

	setupWiFi();
	lastOptoState = optoState();
	updateRemoteOptoState(lastOptoState);
}

void loop()
{
	boolean currentOptoState = optoState();
	if ( lastOptoState != currentOptoState )
	{
		lastOptoState = currentOptoState;
		updateRemoteOptoState(lastOptoState);
	}

	analogWrite(ledPin, currentOptoState ? 0 : 150);

	delay(1000);
}

boolean optoState()
{
	//send the value to the ledPin. Depending on value of resistor
	//you have  to divide the value. for example,
	//with a 10k resistor divide the value by 2, for 100k resistor divide by 4.
	int analogOptoState = (analogRead(lightPin)/2);
	Serial.println("Opto state");
	Serial.println(analogOptoState);
	if ( analogOptoState < brightSignalLimit )
	{
		return false;
	}

	return true;
}

void setupWiFi()
{
	restartWiFi();

	boolean connected=false;
	while (!connected)
	{
		if(connectWiFi())
		{
			connected = true;
			break;
		}
	}
	delay(2000);

	printer("AT+CIPMUX=0");
}

void restartWiFi()
{
	while(!Serial);
	while(!esp8266);

	while(esp8266.available()>0)
	{
		esp8266.read();
	}

	esp8266.println("AT");

	boolean reset = true;
	while ( reset )
	{
		esp8266.flush();
		esp8266.println("AT+RST");
		if(esp8266.find("Ready")||esp8266.find("ready"))
		{
			Serial.println("Module is ready");
			reset = false;
		}
		else
		{
			Serial.println(esp8266.read());
			Serial.println("Module have no response.");
		}
	}

	delay(1000);
}

void updateRemoteOptoState(boolean state)
{
	String valueToSend = state ? "true " : "false";
	long length = state ? 65 : 66;

	String cmd = "AT+CIPSTART=\"TCP\",\"";
	cmd += DST_IP;
	cmd += "\",80";
	esp8266.println(cmd);
	Serial.println(cmd);
	if(esp8266.find("Error")) return;
	cmd = apiRequest(valueToSend);
	String cip_cmp="AT+CIPSEND=";
	cip_cmp+= length;
	printer(cip_cmp);
	while(!esp8266.find(">"))
	{
		Serial.println("No response...");

		reset();
	}

	printer(cmd);
	printer("host:domain.com");
	printer("");
	printer("");
	delay(1500);

	while (esp8266.available())
	{
		char c = esp8266.read();
		Serial.write(c);
		if(c=='\r') Serial.print('\n');
	}
	Serial.println("====");
}

String apiRequest(String valueToSend)
{
	String cmd="GET /toxic.php?room=";
	cmd+= roomNumber;
	cmd+="&status=";
	cmd+= valueToSend;
	cmd+=" HTTP/1.0";
	return cmd;
}

boolean connectWiFi()
{
	esp8266.println("AT+CWMODE=1");
	String cmd="AT+CWJAP=\"";
	cmd+=SSID;
	cmd+="\",\"";
	cmd+=PASSWORD;
	cmd+="\"";
	printer(cmd);
	delay(3000);
	if(esp8266.find("OK"))
	{
		Serial.println("OK, Connected to WiFi.");
		return true;
	}
	else
	{
		Serial.println("Can not connect to the WiFi.");
		return false;
	}
}

void reset()
{
	Serial.println("Reset");
	digitalWrite(resetPin, LOW);
}

void printer(String cmd)
{
	Serial.println(cmd);
	esp8266.println(cmd);
}
