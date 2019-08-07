/*
 #ESP32 Web Server with graphic buttons downloaded from web cache
 Gareth aka chiprobot 21/3/2016
*/
#include <WiFi.h>

const char* ssid     = "Place Wifi name here";
const char* password = "Place your Wifi password here";
int stepper=15; // stepper delay
WiFiServer server(80);

void setup()
{
 Serial.begin(115200);
 pinMode(16, OUTPUT);      // set Stepper direction pin mode  
 pinMode(17, OUTPUT);      // set Stepper1 pin mode
 pinMode(18, OUTPUT);      // set Stepper2 pin mode
 pinMode(19, OUTPUT);      // set Stepper3 pin mode

    // We start by connecting to a WiFi network
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
    Serial.println("Place this IP address into a browser window");
    server.begin();
}

int value = 0;

void loop(){
 WiFiClient client = server.available();   // listen for incoming clients

  if (client) {                             
    Serial.println("new client");          
    String currentLine = "";                   // make a String to hold incoming data from the client
    while (client.connected()) {            
      if (client.available()) {                // if there's client data
        char c = client.read();                // read a byte
          if (c == '\n') {                     // check for newline character, 
          if (currentLine.length() == 0) {     // if line is blank it means its the end of the client HTTP request
            client.println("<!DOCTYPE html>"); // open wrap the web page
            client.print("<html><head><meta name='viewport' content='initial-scale=1.0'><meta charset='utf-8'><style>#map {height: 100%;}html, body {height: 100%;margin: 0;padding: 0;}</style></head>");
            client.print("<body><h1>ESP32 WebServer Stepper Motor's</h1>");
        // Gui buttons start here
client.print("<input type=image style=width:33%;height:20% src='http://myrobotlab.org/sites/default/files/users/user25images/ButtonBlueLong.png'onmousedown=location.href='/dec1' >");
                client.print("<input type=image style=width:33%;height:20% src='http://myrobotlab.org/sites/default/files/users/user25images/nixiesmall1.png'>");
                client.print("<input type=image style=width:33%;height:20% src='http://myrobotlab.org/sites/default/files/users/user25images/ButtonOrangeLong.png'onmousedown=location.href='/inc1' >");
                   
                client.print("<input type=image style=width:33%;height:20% src='http://myrobotlab.org/sites/default/files/users/user25images/ButtonPinkLong.png'onmousedown=location.href='/dec2' >");
                client.print("<input type=image style=width:33%;height:20% src='http://myrobotlab.org/sites/default/files/users/user25images/nixiesmall2.png'>");
                client.print("<input type=image style=width:33%;height:20% src='http://myrobotlab.org/sites/default/files/users/user25images/ButtonGreenLong.png'onmousedown=location.href='/inc2' >"); 

                client.print("<input type=image style=width:33%;height:20% src='http://myrobotlab.org/sites/default/files/users/user25images/ButtonTurqLong.png'onmousedown=location.href='/dec3' >");
                client.print("<input type=image style=width:33%;height:20% src='http://myrobotlab.org/sites/default/files/users/user25images/nixiesmall3.png'>");
                client.print("<input type=image style=width:33%;height:20% src='http://myrobotlab.org/sites/default/files/users/user25images/ButtonPurpleLong.png'onmousedown=location.href='/inc3' >"); 
                
 client.print("<input type=image style=width:100%;height:20% src='http://myrobotlab.org/sites/default/files/users/user25images/ButtonRedLong.png'onmousedown=location.href='/stopall'...

 
