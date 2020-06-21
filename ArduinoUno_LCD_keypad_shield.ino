/*********************************************************************/
  #define autor       "Caio Elizio - caioelizio@gmail.com 62 98223-9516"
  #define programa    "Casa1014 iot Lab "
  #define versao      "v2.0"
  #define dataversao  "17/02/20"
  #define placa       "Arduino Uno LCD keypad shield"
/*********************************************************************/
//Definicoes Setup
//Definicoes programacao por serial
/*Lista de Peças: 
 * 1 Arduino UNO - ATmega328 
 * 1 LCD_keypad_shield
 * 1 buzzer
 * 1 carregador celular 5v (Alim. Arduino)
 * 1 carregador powerbank 5v (Alim. Arduino)
*/
/*Versoes
  v1.01 ...
  v1.02 ...
*/

//Pinagem--------------------------------------------------
  #define pinLedOnboard    13
  #define pinBuzzer        A1
//define values buttons
  #define pinBtnRIGHT  0
  #define pinBtnUP     1
  #define pinBtnDOWN   2
  #define pinBtnLEFT   3
  #define pinBtnSELECT 4
  #define pinBtnNONE   5
#include <LiquidCrystal.h>
  //Pin assignments for DFRobot LCD Keypad Shield
  LiquidCrystal lcd(8, 9, 4, 5, 6, 7);
  
//Variáveis e objetos globais--------------------------------------
  #define ON  1
  #define OFF 0
  boolean bsomteclas = false;
//f_Tempo
  int i_seg    = 0;
  int i_seg2   = 0;
  int i_min    = 0;
  int i_min2   = 1;
  int i_horas  = 0;
  int i_valor  = 4825;
  long lastMsg = 0;  
//EEPROM
  int iStatusLedOnboard = 0;
  int iNumReset         = 0;
//define values buttons
  int lcd_key     = -1;
  int adc_key_in  = 0;


#include <EEPROM.h>
  int  end_EEPROM = 0;  //inicia endereço 
  int  EEPROM_Read(int end_EEPROM);
  void EEPROM_Write(int end_EEPROM_w,int i_valor);



//Definicoes Setup
void setup(){
  initSerial();
  initOutput(); //Define a porta status de saida e entrada
  
  f_lcd_incio();
  f_AtivaSomTeclas(ON);
//  f_buzzbeep(); //1 bip
//  delay(10);
  delay(1000); //Espera 1 segundo 
}

//Funções----------------------------------------------------------- 
//Definicoes Serial
void initSerial(){
  Serial.begin(115200); //9600 da erro, caindo conexao...
  delay(10);
  Serial.println(); //Linha em branco
  Serial.println(F("Iniciando Serial..."));
  Serial.println(); //Linha em branco
  Serial.print(F("Autor:       ")); Serial.println(autor);
  Serial.print(F("Sistema:     ")); Serial.print(programa); Serial.println(versao);
  Serial.print(F("Data versao: ")); Serial.println(dataversao);

  Serial.println(); //Linha em branco
  Serial.println(F("-------------------"));
  iNumReset = EEPROM_Read(200);  //ler status salvo na memoria
  iNumReset++;
  Serial.print(F("Numero de Reset: ")); Serial.println(iNumReset);
  Serial.println(F("-------------------"));
  EEPROM_Write(200,iNumReset); //Numero de Reset iNumReset
}
void initOutput(void){ //Definicoes pinos INPUT OUTPUT
  Serial.println(); //Linha em branco  
  pinMode(pinLedOnboard, OUTPUT);  //initialize onboard LED as output
  pinMode(pinBuzzer,     OUTPUT);

  Serial.println(); //Linha em branco
  Serial.println(F("Definindo dispositivos como desligado ON ou OFF"));  
  iStatusLedOnboard    = EEPROM_Read(13);  //ler status salvo na memoria
  digitalWrite(pinLedOnboard,     iStatusLedOnboard); //led 13 começa com o status salvo
//  digitalWrite(pinLedOnboard,     OFF);
  digitalWrite(pinBuzzer,         ON);

  Serial.print(F("pinLedOnboard         "));  Serial.print(pinLedOnboard);
  if (iStatusLedOnboard == 1){
    Serial.println(F(" ON"));
  }else{
    Serial.println(F(" OFF"));
  }
  Serial.print(F("pinBuzzer             "));  Serial.print("A1");  Serial.println(F(" ON"));
}
void f_led_arduinoUno(boolean b_valor){ //acende e apaga o led do arduino
  if (b_valor == ON){
    EEPROM_Write(13,1);    //Status led onboard
    digitalWrite(pinLedOnboard, ON);  //LED Arduino
//    Serial.print(F("Led Arduino ")); Serial.println(F("ON"));
  } else {
    EEPROM_Write(13,0);    //Status led onboard
    digitalWrite(pinLedOnboard, OFF); //LED Arduino
//    Serial.print(F("Led Arduino ")); Serial.println(F("OFF"));  
  }
}

void iniTempo(){
  i_valor = millis()/1000;
//  Serial.print(F("t_valor"); Serial.println(t_valor);
  i_seg   = i_valor % 60;
//  Serial.print(F("t_seg")); Serial.println(t_seg);
  i_valor = i_valor / 60;
//  Serial.print(F("t_valor")); Serial.println(t_valor);
  i_min   = i_valor % 60;
//  Serial.print(F("t_min")); Serial.println(t_min);
  i_valor = i_valor / 60;
//  Serial.print(F("t_valor")); Serial.println(t_valor);
  i_horas = i_valor;
//  Serial.print(F("t_horas")); Serial.println(t_horas);
}
void f_printTempoSerial(){
//  Serial.println(); //Linha em branco
  Serial.print(F("Tempo desde ultimo reset: "));
  if (i_horas < 10){ Serial.print(F("0")); } Serial.print(i_horas); Serial.print(F(":"));
  if (i_min   < 10){ Serial.print(F("0")); } Serial.print(i_min);   Serial.print(F(":"));
  if (i_seg   < 10){ Serial.print(F("0")); } Serial.println(i_seg);
}
void f_printTempoLCD(){
  lcd.setCursor(8,1);           // move cursor to second line "1" and 9 spaces over
  if (i_horas < 10){ lcd.print(F("0")); } lcd.print(i_horas); lcd.print(F(":"));
  if (i_min   < 10){ lcd.print(F("0")); } lcd.print(i_min);   lcd.print(F(":"));
  if (i_seg   < 10){ lcd.print(F("0")); } lcd.println(i_seg);
}

void f_buzz(int targetPin, long frequency, long length){
  long delayValue = 1000000 / frequency / 2;  // calculate the delay value between transitions
  long numCycles = frequency * length / 1000; // calculate the number of cycles for proper timing
  for (long i = 0; i < numCycles; i++){       // for the calculated length of time...
    digitalWrite(targetPin, HIGH); // write the buzzer pin high to push out the diaphram
    delayMicroseconds(delayValue); // wait for the calculated delay value
    digitalWrite(targetPin, LOW);  // write the buzzer pin low to pull back the diaphram
    delayMicroseconds(delayValue); // wait again or the calculated delay value
  }
}
void f_buzzbeep(){
  Serial.println(F("1 bip"));
  f_buzz(pinBuzzer, 300, 50);    //1 bip inicial (pin,frequencia,Duração)  //baixo
//  f_buzz(pinBuzzer, 1800, 50); //1 bip inicial (pin,frequencia,Duração)  //alto
//  f_buzz(pinBuzzer, 0, 0);      //stop the tone playing:
//  f_buzz(pinBuzzer, 0, 20);     //Min
//  f_buzz(pinBuzzer, 0, 20000);  //Max
}
void f_somteclas(){ //som teclas
  if (bsomteclas == true){
    f_buzzbeep();
  }
}
void f_AtivaSomTeclas(boolean b_valor) {
  if (b_valor == ON) {  bsomteclas = true;
    Serial.println(); //Linha em branco
    Serial.println(F("Som teclas on"));
  } else {
    int cont = 0;  
  //  cont++;
    bsomteclas = false;
    Serial.println(); //Linha em branco
    Serial.println(F("Som teclas off"));
  }
}

void EEPROM_Clear(){
  // write a 0 to all 512 bytes of the EEPROM
  for (int i = 0; i < 512; i++)
    EEPROM.write(i, 0);
  // turn the LED on when we're done
  digitalWrite(13, HIGH);
}
int EEPROM_Read(int end_EEPROM){ //EEPROM_Read(0-511)
  byte value;
  value = EEPROM.read(end_EEPROM);  //read a byte from the current address 0-511
/*  Serial.println(); //Linha em branco
  Serial.print(F("Endereco memoria EEPROM: "));  Serial.print(end_EEPROM);
  Serial.print(F("\t")); //espaço
  Serial.print(F(" Valor read: "));  Serial.print(value, DEC);
//*/
  delay(500);
  return value;
}
void EEPROM_Write(int end_EEPROM_w,int i_valor){ //EEPROM_Write(0-511,0-511)
  //the EEPROM, so go back to 0 when we hit 512.
  if (end_EEPROM_w > 512)  end_EEPROM_w = 0;
  if (end_EEPROM_w == 512) end_EEPROM_w = 0;
  EEPROM.write(end_EEPROM_w, i_valor);
  //write the value to the appropriate byte of the EEPROM.
  //these values will remain there when the board is turned off.
//*
/*  Serial.println(); //Linha em branco
  Serial.print(F("Endereco memoria: ")); Serial.print(end_EEPROM_w);
  Serial.print(F("\t")); //espaço
  Serial.print(F("Valor write: "));      Serial.println(i_valor, DEC);
//*/
  delay(100);
}

void f_ProgControleSerial(){ 
  char leitura_serial = ' ';
  String leitura = "";
  //if ( Serial.available() == 1){ //Se serial recer dados... se nao inicia o loop novamente
  while( Serial.available() > 0 ){ //enquanto serial nao receber dados
  leitura_serial = Serial.read();
  Serial.print(F("leitura_serial: "));
  Serial.println(leitura_serial);   //Imprimir dados da serial
    if (leitura_serial == 'D' || leitura_serial == 'd'){ //desligado
      f_led_arduinoUno(OFF);
  //    f_AtivaSomTeclas(OFF);
      while(1); //para o loop
    };
    if (leitura_serial == 'L' || leitura_serial == 'l'){ //ligado
      f_led_arduinoUno(ON);
 //     f_AtivaSomTeclas(ON);
      while(1); //para o loop
    }else{
      Serial.println(leitura_serial + " Tecla errada digite L ou D.");
      while(0); //inicia o loop
    };
  };
}

int read_LCD_buttons() {
 adc_key_in = analogRead(0);      // read the value from the sensor
 // my buttons when read are centered at these valies: 0, 144, 329, 504, 741
 // we add approx 50 to those values and check to see if we are close
 if (adc_key_in > 1000) return pinBtnNONE; // We make this the 1st option for speed reasons since it will be the most likely result
 // For V1.1 us this threshold
 if (adc_key_in < 50)   return pinBtnRIGHT;
 if (adc_key_in < 250)  return pinBtnUP;
 if (adc_key_in < 450)  return pinBtnDOWN;
 if (adc_key_in < 650)  return pinBtnLEFT;
 if (adc_key_in < 850)  return pinBtnSELECT;

 // For V1.0 comment the other threshold and use the one below:
/*
 if (adc_key_in < 50)   return pinBtnRIGHT;
 if (adc_key_in < 195)  return pinBtnUP;
 if (adc_key_in < 380)  return pinBtnDOWN;
 if (adc_key_in < 555)  return pinBtnLEFT;
 if (adc_key_in < 790)  return pinBtnSELECT;
*/

 return pinBtnNONE;  // when all others fail, return this...
}
void f_teclado() {
  lcd.setCursor(0,1);             // move to the begining of the second line
  switch (lcd_key) {              // depending on which button was pushed, we perform an action
    case pinBtnRIGHT: {
//      if (i_seg > 2){
        lcd.print("RIGHT ");
        Serial.println(); //Linha em branco
        Serial.println(F("RIGHT "));
        f_somteclas();
//      }
      break;
    }
    case pinBtnLEFT: {
      lcd.print("LEFT   ");
      f_somteclas();
      break;
    }
    case pinBtnUP: {
      lcd.print("UP    ");
      f_somteclas();
      break;
    }
    case pinBtnDOWN: {
      lcd.print("DOWN  ");
      f_somteclas();
      break;
    }
    case pinBtnSELECT: {
      lcd.print("SELECT");
      f_somteclas();
      break;
    }
    case pinBtnNONE: {
//      lcd.print("NONE  ");
      lcd.print("      ");
      break;
    }
  }
}

void f_lcd_incio() {
  lcd.begin(16, 2);              // start the library
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Cora Hacker"); // print a simple message
  lcd.setCursor(0,1);             // move to the begining of the second line
  lcd.print("N. Reset: ");  lcd.print(iNumReset);
  delay(2000);
  lcd.clear();
}
void f_lcd_teste_botoes() {
  lcd.clear();
//  lcd.setCursor(0,0);
//  lcd.print("Click buttons"); // print a simple message
  lcd.print("Click em 1 botao"); // print a simple message
}

void f_atualiza_sensores(){
  long now = millis()/1000;
  if (now - lastMsg > 5){ //se maior q 10segundos
    lastMsg = now;
    Serial.println(); //Linha em branco
    Serial.println(F("<---------------Atualizando sensores-------------->"));
    f_printTempoSerial(); //tempo desde o ultimo reset

    f_lcd_teste_botoes();
    f_printTempoLCD();

//
//    Serial.print(F("i_seg: ")); Serial.println(i_seg);
//    Serial.println(); //Linha em branco
    Serial.println(F("<------------------------------------------------->"));
  }
}

void loop() {
  iniTempo();
  f_ProgControleSerial();

  f_atualiza_sensores();
  
  f_teclado();
  lcd_key = read_LCD_buttons();  // read the buttons
  
  delay(100);
}
