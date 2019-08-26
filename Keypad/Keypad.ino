/* @file HelloKeypad.pde
|| @version 1.0
|| @author Alexander Brevig
|| @contact alexanderbrevig@gmail.com
||
|| @description
|| | Demonstrates the simplest use of the matrix Keypad library.
|| #
*/
#include <Keypad.h>

const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

/*
byte rowPins[ROWS] = {12, 11, 10, 9}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {8, 7, 6, 5}; //connect to the column pinouts of the keypad
*/

byte rowPins[ROWS] = {12, 11, A0, A1}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {A2, A3, A4, A5}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

void setup(){
   Serial.begin(9600);
}

/*
  VARIAVEIS GLOBAIS
*/
int tam_senha = 0;
char senha[10] = "";

/*
  FUNÇOES
*/

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


/*
  LOOP
*/
void loop(){
  char key = keypad.getKey();

  if (key == 'A'){
    alterar_senha();
  }
  if (key == 'B'){
    Serial.println(senha);
    Serial.println(tam_senha);
  }
  if (key){
    Serial.println(key);
  }
}
