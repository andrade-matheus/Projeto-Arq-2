#include <WiFi.h>
#include <Stepper.h>
#include "ESPAsyncWebServer.h"
#include "SPIFFS.h"

#define Pino_in_arduino 4
#define Pino_out_ardino 34
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

boolean estado_Pino_in_arduino = false;
boolean estado_passado = false;
boolean aberto = false;
String estado_porta;

//#########################################
//    FUNÇÕES
//#########################################

void abrir() {
  if(!aberto){
    myStepper.step(512);
    aberto = true;
    digitalWrite(Led_Verde, HIGH);
    digitalWrite(Led_Vermelho, LOW);
    Serial.println("ABRIU !");
    delay(100);
  }
}

void fechar() {
  if(aberto){
    myStepper.step(-512);
    aberto = false;
    digitalWrite(Led_Verde, LOW);
    digitalWrite(Led_Vermelho, HIGH);
    Serial.println("FECHOU !");
    delay(100);
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

  pinMode(Pino_in_arduino, INPUT);
  pinMode(Pino_out_ardino, OUTPUT);
  pinMode(Led_Verde, OUTPUT);
  pinMode(Led_Vermelho, OUTPUT);

//#########################################
// Configurações do Wifi
//#########################################

  if(!SPIFFS.begin()){
      Serial.println("An Error has occurred while mounting SPIFFS");
      return;
  }

  WiFi.begin(ssid, password);

  int contagem = 0;

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
    Serial.println(WiFi.status());
    digitalWrite(Led_Vermelho, HIGH);
    delay(1000);
    digitalWrite(Led_Vermelho, LOW);
    if(contagem >= 5){
      Serial.println("REBOOTING ...");
      ESP.restart();
    }
    contagem ++;
  }

  for(int i=0; i<3; i++){
    digitalWrite(Led_Verde, HIGH);
    delay(500);
    digitalWrite(Led_Verde, LOW);
    delay(500);
  }

  Serial.println(WiFi.localIP());

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });

  server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/style.css", "text/css");
  });

  server.on("/ABRIR", HTTP_GET, [](AsyncWebServerRequest *request){
    abrir();
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

  digitalWrite(Led_Vermelho, HIGH);
}

void loop(){
  estado_passado = estado_Pino_in_arduino;
  estado_Pino_in_arduino = digitalRead(Pino_in_arduino);
  if(estado_Pino_in_arduino != estado_passado){
    if(estado_Pino_in_arduino){
      abrir();
    }else{
      fechar();
    }
  }
}
