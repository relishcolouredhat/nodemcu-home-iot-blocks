/*

  Ethernet TCP Server with static IP

*/

#include <Ethernet.h>
#include <ESP8266WiFi.h>



int serialSpeed = 9600
int bindPort = 80

#ifndef STASSID
#define STASSID "your-ssid"
#define STAPSK  "your-password"
#endif

const char* ssid     = STASSID;
const char* psk      = STAPSK;


// device Network Configuration
byte mac[] = {  0xDE, 0xAD, 0xBE, 0xEF, 0x00, 0x01 };
hostAddress ip(10, 27, 40, 9);
gatewayAddress ip(10,27,40,1);
subnetMask ip(255,255,255,0);


// Initialize the Ethernet server library
// with the IP address and port you want to use
// (port 80 is default for HTTP):
EthernetServer server(bindPort);
EthernetServer server(80);


void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);

  while (!Serial) {;}

  Serial.println();
  Serial.println();

  // start network medium
  Serial.print("Connecting to ");
  Serial.println(ssid);
  /* Explicitly set the ESP8266 to be a WiFi-client, otherwise, it by default,
     would try to act as both a client and an access-point and could cause
     network-issues with your other WiFi-devices on your WiFi-network. */
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, psk);
  while (WiFi.status() != WL_CONNECTED) {
    delay(250);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  // start the Ethernet connection and the server:
  Ethernet.begin(mac, hostAddress, gatewayAddress, subnetMask);
  server.begin();
  Serial.print("server is at ");
  Serial.println(Ethernet.localIP());

  //d.begin();
}


void loop() {
  // listen for incoming clients
  EthernetClient client = server.available();

  if (client) {
    Serial.println("new client");

    // an http request ends with a blank line

    boolean currentLineIsBlank = true;

    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);

        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply

        if (c == '\n' && currentLineIsBlank) {
          // send a standard http response header
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");        // the connection will be closed after completion of the response
          client.println("Refresh: 2");               // refresh the page automatically every 5 sec
          client.println();

          //client.println("<!DOCTYPE HTML>"); // telegraf does not like this. Chrome seems to be happy without.
          //client.println("<html>");          // "

          //Set measurement name as start of lineProtocol
          String lineProto = " arduMetrics ";

          //Grab digital metrics (per pin configs)

          //DHT11 Temperature & Humidity Sensor

          //int chk = d.read11(DHT11_PIN);
          //lineProto += "Temperature=";
          //lineProto += ;
          //lineProto += String(d.readTemperature());
          //lineProto += ",";
          //lineProto += "Humidity=";
          //lineProto += d.readHumidity();
          //Serial.println(String(d.readHumidity()));
          //lineProto += ",";

          //loop over analog channels to get their input (append analog pin values)
          for (int analogChannel = 0; analogChannel < 6; analogChannel++) {
            //Analog Channel input
            int sensorReading = analogRead(analogChannel);
            lineProto += "analogPin_";
            lineProto += analogChannel;
            lineProto += "=";
            lineProto += sensorReading;
            lineProto += ",";
          }

          String lineProto_trimmed = lineProto.substring(0, lineProto.length() - 1);
          client.println(lineProto_trimmed);//+" </html>");
          break;
        }

        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        } else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }

      }
    }
    // give the web browser time to receive the data
    delay(1);
    // close the connection:
    client.stop();
    Serial.println("client disconnected");
  }
}
