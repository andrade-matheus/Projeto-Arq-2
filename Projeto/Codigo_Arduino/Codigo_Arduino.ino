#include <LiquidCrystal.h>
#include <Adafruit_Fingerprint.h>
#include <Keypad.h>

#define Sinal_ESP 10

//#########################################
//    VARIAVEIS GLOBAIS
//#########################################

boolean aberto = false;

/********** LEITOR BIOMETRICO **********/
SoftwareSerial mySerial(2, 3);
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);
uint8_t id;

/********** LCD **********/
LiquidCrystal lcd(12, 11, 9, 8, 7, 6);

/********** TECLADO MEMBRANA **********/
const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

byte rowPins[ROWS] = {5, 4, A0, A1}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {A2, A3, A4, A5}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

/********** SENHA **********/
int tam_senha = 0;
char senha[10] = "";

//#########################################
//    DECLARAÇÃO DAS FUNÇÕES
//#########################################

int verifica_senha();
int alterar_senha();
uint8_t getFingerprintEnroll();
int getFingerprintIDez();
uint8_t getFingerprintID();

//#########################################
//    FUNÇÃO SETUP
//#########################################

void setup(){
  lcd.begin(16, 2);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("SO PRA NAO");
  lcd.setCursor(0, 1);
  lcd.print("FICAR EM BRANCO");

  pinMode(Sinal_ESP, OUTPUT);

  Serial.begin(9600);
  while (!Serial);  // For Yun/Leo/Micro/Zero/...
  delay(100);
  Serial.println("\n\nAdafruit Fingerprint sensor enrollment");

  // set the data rate for the sensor serial port
  finger.begin(57600);

  if (finger.verifyPassword()) {
    Serial.println("Found fingerprint sensor!");
  } else {
    Serial.println("Did not find fingerprint sensor :(");
    while (1) { delay(1); }
  }

  finger.getTemplateCount();
  Serial.print("Sensor contains ");
  Serial.print(finger.templateCount);
  Serial.println(" templates");
  Serial.println("Waiting for valid finger...");
}

//#########################################
//    FUNÇÃO LOOP
//#########################################

void loop() {
  char key = keypad.getKey();

  Serial.println("Esperando a escolha de alguma opcao:");
  while (!key) {
    key = keypad.getKey();
  }

  if (key == 'A'){
    if(verifica_senha()){
      if(aberto){
        digitalWrite(Sinal_ESP, LOW);
        aberto = false;
        Serial.println("FECHOU !");
      }else{
        digitalWrite(Sinal_ESP,HIGH);
        aberto = true;
        Serial.println("ABRIU !");
      }
    }
  }else if (key == 'B'){
    alterar_senha();
    Serial.println(senha);
    Serial.println(tam_senha);

  }else if (key == 'C'){
    if(verifica_senha()){
      Serial.println("Ready to enroll a fingerprint!");
      Serial.println("Please type in the ID # (from 1 to 127) you want to save this finger as...");
      id = finger.templateCount + 1;
      if (id == 0) {// ID #0 not allowed, try again!
         return;
      }
      Serial.print("Enrolling ID #");
      Serial.println(id);

      while (!getFingerprintEnroll());
    }

  }else if (key == 'D'){
    if(getFingerprintIDez()+1){
      if(aberto){
        digitalWrite(Sinal_ESP, LOW);
        aberto = false;
        Serial.println("FECHOU !");
      }else{
        digitalWrite(Sinal_ESP,HIGH);
        aberto = true;
        Serial.println("ABRIU !");
      }
    }
  }else if (key == '#'){
    //TO-DO
  }else if (key){
    Serial.println(key);
  }
  delay(100);
}

//#########################################
//    FUNÇÕES TECLADO MEMBRANA
//#########################################

int verifica_senha(){
  int i = 0;
  int correta = 1;
  char senha_teste[10] = "";

  Serial.println("Confirme a sua senha:");

  while (i < tam_senha) {
    senha_teste[i] = keypad.getKey();
    if(senha_teste[i]){
      if(senha_teste[i] != senha[i]){
        correta = 0;
      }
      i++;
    }
  }
  return correta;
}

int alterar_senha(){
  int i = 0;
  char key = "";

  if(verifica_senha()){
    //senha = "";
    Serial.println("Insira sua senha nova:");
    key = keypad.getKey();
    while((key != '*') && (i < 10)){
      if (key){
        if((key >= 48) && (key <= 57)){
          senha[i] = key;
          i++;
        }else{
          Serial.println("Digito invalido, valores devem ser entre 0 - 9");
        }
      }
      key = keypad.getKey();
    }
    tam_senha = i;
    Serial.println("Senha alterada com sucesso.");
  }else{
    Serial.println("Senha incorreta.");
  }
  return 1;
}

//#########################################
//    FUNÇÕES TELA LCD
//#########################################

//TO-DO

//#########################################
//    FUNÇÕES LEITOR BIOMETRICO
//#########################################
uint8_t getFingerprintEnroll() {
  int p = -1;
  Serial.print("Waiting for valid finger to enroll as #"); Serial.println(id);
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      Serial.println(".");
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      break;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
      break;
    default:
      Serial.println("Unknown error");
      break;
    }
  }

  // OK success!
  p = finger.image2Tz(1);
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }

  Serial.println("Remove finger");
  delay(2000);
  p = 0;
  while (p != FINGERPRINT_NOFINGER) {
    p = finger.getImage();
  }
  Serial.print("ID "); Serial.println(id);
  p = -1;
  Serial.println("Place same finger again");
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      Serial.print(".");
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      break;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
      break;
    default:
      Serial.println("Unknown error");
      break;
    }
  }

  // OK success!
  p = finger.image2Tz(2);
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }

  // OK converted!
  Serial.print("Creating model for #");  Serial.println(id);

  p = finger.createModel();
  if (p == FINGERPRINT_OK) {
    Serial.println("Prints matched!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_ENROLLMISMATCH) {
    Serial.println("Fingerprints did not match");
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }

  Serial.print("ID "); Serial.println(id);
  p = finger.storeModel(id);
  if (p == FINGERPRINT_OK) {
    Serial.println("Stored!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_BADLOCATION) {
    Serial.println("Could not store in that location");
    return p;
  } else if (p == FINGERPRINT_FLASHERR) {
    Serial.println("Error writing to flash");
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }
}

// returns -1 if failed, otherwise returns ID #
int getFingerprintIDez() {
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK)  return -1;

  // found a match!
  Serial.print("Found ID #");
  Serial.print(finger.fingerID);
  Serial.print(" with confidence of ");
  Serial.println(finger.confidence);
  return finger.fingerID;
}

uint8_t getFingerprintID() {
  uint8_t p = finger.getImage();
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      Serial.println("No finger detected");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }

  // OK success!

  p = finger.image2Tz();
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }

  // OK converted!
  p = finger.fingerFastSearch();
  if (p == FINGERPRINT_OK) {
    Serial.println("Found a print match!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_NOTFOUND) {
    Serial.println("Did not find a match");
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }

  // found a match!
  Serial.print("Found ID #");
  Serial.print(finger.fingerID);
  Serial.print(" with confidence of ");
  Serial.println(finger.confidence);

  return finger.fingerID;
}
