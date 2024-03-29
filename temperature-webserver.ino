/*

  Influx Line Protocol Web Server - extenstion to to 'Web Server'

  NodeMCU Web Server with 1wire temperature sensors
  
  TODO: 
    - Refactor and clean up for public release
    - Remove commented blocks
    - Better doc up top

  Created to tinker by Kelsey Comstock 23 Jan 2019, courtesy of:
 
  David A. Mellis   - original creator of "Web Server" for Arduino - 18 Dec 2009
  Tom Igoe          - modified 9 Apr 2012
  Arturo Guadalupi  - modified 02 Sept 2015

*/

#include <SPI.h>
#include <Ethernet.h>
#include <ESP8266WiFi.h>
//#include <DHT.h>
//#include <OneWire.h>

//OneWire ds(7); // instance ds of OneWire on pin 7



//#define DHT11_PIN 7
//#define DHTYPE DHT11

//DHT d(5, DHTYPE);

// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
};
IPAddress ip(10, 27, 40, 9);

// Initialize the Ethernet server library
// with the IP address and port you want to use
// (port 80 is default for HTTP):
EthernetServer server(80);


void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }


  // start the Ethernet connection and the server:
  Ethernet.begin(mac, ip);
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
