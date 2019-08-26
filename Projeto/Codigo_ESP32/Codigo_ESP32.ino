#include <WiFi.h>
#include <Stepper.h>
#include "ESPAsyncWebServer.h"
#include "SPIFFS.h"

#define Pino_arduino 16
#define Led_Verde 19
#define Led_Vermelho 18

//WI-FI
const char* ssid     = "MATHEUS 2G";
const char* password = "12345670";

AsyncWebServer server(80);

//MOTOR
const int stepsPerRevolution = 500;

Stepper myStepper(stepsPerRevolution, 27, 25, 26, 33);

//#########################################
//    VARIAVEIS GLOBAIS
//#########################################

boolean estado_pino_arduino = false;
boolean aberto = false;
String estado_porta;

//#########################################
//    FUNÇÕES
//#########################################

void abir() {
  if(!aberto){
    myStepper.step(-512);
    aberto = true;
    digitalWrite(Led_Verde, HIGH);
    digitalWrite(Led_Vermelho, LOW);
    Serial.println("ABRIU !");
    delay(500);
  }
}

void fechar() {
  if(aberto){
    myStepper.step(512);
    aberto = false;
    digitalWrite(Led_Verde, LOW);
    digitalWrite(Led_Vermelho, HIGH);
    Serial.println("FECHOU !");
    delay(500);
  }
}

String processor(const String& var){
  Serial.println(var);
  if(var == "STATE"){
    if(aberto){
      estado_porta = "ABERTA";
    }
    else{
      estado_porta = "FECHADA";
    }
    Serial.println(estado_porta);
    return estado_porta;
  }
  return String();
}

//#########################################
//    SETUP
//#########################################

void setup()
{
  Serial.begin(115200);
  delay(10);

  pinMode(18, OUTPUT);
  pinMode(19, OUTPUT);

//#########################################
// Configurações do Wifi
//#########################################

  if(!SPIFFS.begin()){
      Serial.println("An Error has occurred while mounting SPIFFS");
      return;
  }

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
    Serial.println(WiFi.status());
  }

  Serial.println(WiFi.localIP());

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });

  server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/style.css", "text/css");
  });

  server.on("/ABRIR", HTTP_GET, [](AsyncWebServerRequest *request){
    abir();
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });

  server.on("/FECHAR", HTTP_GET, [](AsyncWebServerRequest *request){
    fechar();
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });

  server.begin();

//#########################################
// Configurações dos Motores
//#########################################

    myStepper.setSpeed(60);

}

void loop(){
  estado_pino_arduino = digitalRead(Pino_arduino);
  if(estado_pino_arduino){
    abir();
  }else{
    fechar();
  }
}
