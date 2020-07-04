/*********************************************************************/
  #define autor       "Caio Elizio - caioelizio@gmail.com 62 98223-9516"
  #define programa    "LCD keypad shield "
  #define versao      "v2.0"
  #define dataversao  "17/02/20"
  #define placa       "Arduino Uno LCD keypad shield"
  #define TempoAtualizacao 10
  boolean b_debug    = true;
/*********************************************************************/
//Sensor DHT11
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
  #define pinDHT11     A2 //2 ou 3 sensor de temperatura.
  #define pinSensorPIR 11
//  #define pin 11 porta livre
//  #define pin 12 porta livre
#include <LiquidCrystal.h>
  LiquidCrystal lcd(8, 9, 4, 5, 6, 7); //Pin for DFRobot LCD Keypad Shield


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


//Sensor DHT11
#include <DHT.h>
void iniDHT11();
void f_SensorDHT11();
  #define DHTTYPE DHT11
  DHT dht(pinDHT11, DHTTYPE);
  float f_temp     = 0;
  float f_humid    = 0;



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
  pinMode(pinDHT11,      INPUT);
  pinMode(pinSensorPIR,  INPUT);
    
  Serial.println(); //Linha em branco
  Serial.println(F("Definindo dispositivos como desligado ON ou OFF"));  
  iStatusLedOnboard    = EEPROM_Read(13);  //ler status salvo na memoria
  digitalWrite(pinLedOnboard,     iStatusLedOnboard); //led 13 começa com o status salvo
//  digitalWrite(pinLedOnboard,     OFF);
  digitalWrite(pinBuzzer,         ON);
  digitalWrite(pinDHT11,          OFF);
  digitalWrite(pinSensorPIR,      ON);

  Serial.print(F("pinLedOnboard         "));  Serial.print(pinLedOnboard);
  if (iStatusLedOnboard == 1){
    Serial.println(F(" ON"));
  }else{
    Serial.println(F(" OFF"));
  }
  Serial.print(F("pinBuzzer             "));  Serial.print("A1");         Serial.println(F("  ON"));
  Serial.print(F("pinDHT11              "));  Serial.print(pinDHT11);     Serial.println(F("  OFF"));
  Serial.print(F("pinSensorPIR          "));  Serial.print(pinSensorPIR); Serial.println(F("  ON"));
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
void EEPROM_Clear(){
  // write a 0 to all 512 bytes of the EEPROM
  for (int i = 0; i < 512; i++)
    EEPROM.write(i, 0);
  // turn the LED on when we're done
  digitalWrite(13, HIGH);
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
void f_teclado_shied() {
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
      f_lcd_incio();
      break;
    }
    case pinBtnNONE: {
//      lcd.print("NONE  ");
//      lcd.print("      ");
      break;
    }
  }
}




//Sensor DHT11
void iniDHT11() {
  Serial.println(); //Linha em branco
  Serial.println(F("Iniciando DHT11..."));
  dht.begin();
  f_SensorDHT11();
}
void f_SensorDHT11() {
  float f_temp2  = 0;
  float f_humid2 = 0;
  int i_temp  = 0;
  int i_humid = 0;
  if (dht.readTemperature()) f_temp2  = dht.readTemperature();
  if (dht.readHumidity())    f_humid2 = dht.readHumidity();

  if (isnan(f_humid2) || isnan(f_temp2)) {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Falha DHT");
    Serial.println(F("Falha na leitura do sensor DHT!"));
    i_temp  = 0;
    i_humid = 0;
    return;
  }
  else { //se ok...
    f_temp  = f_temp2;
    f_humid = f_humid2;
    i_temp  = f_temp2;
    i_humid = f_humid2;
  }
  Serial.print(F("Temperatura: ")); Serial.print(f_temp, 0);  Serial.print(F("oC"));
  Serial.print(F(" - Humidade: ")); Serial.print(f_humid, 0); Serial.println(F("%"));
  lcd.clear();
  lcd.setCursor(0,0);
//  lcd.print("Temp.:    "); lcd.print(f_temp);
  lcd.print("Temp.:    "); lcd.print(i_temp);
  lcd.setCursor(0,1);
//  lcd.print("Humidade: "); lcd.print(f_humid);
  lcd.print("Humidade: "); lcd.print(i_humid);
  delay(1000);
}



void f_lcd_incio() {
  lcd.begin(16, 2);  //start the library
  lcd.clear();
  lcd.setCursor(0,0); //move to the begining of the 1º line
  lcd.print("Cora Hacker");
  f_printNumReset();
  delay(2000);
  lcd.clear();
}
void f_printNumReset(){
  lcd.setCursor(0,1); //move to the begining of the second line
  lcd.print("N. Reset: ");  lcd.print(iNumReset);
}
void f_lcd_teste_botoes() {
//  Serial.println(); //Linha em branco
  Serial.println(F("Click em 1 botao"));
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Click em 1 botao");
  f_printNumReset();
}
void f_printTempoLCD(){
  f_printTempoSerial(); //tempo desde o ultimo reset
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Tempo reset ");
  lcd.setCursor(8,1);           // move cursor to second line "1" and 9 spaces over
  lcd.print(millis()/1000);
/*  if (i_horas < 10){ lcd.print(F("0")); } lcd.print(i_horas); lcd.print(F(":"));
  if (i_min   < 10){ lcd.print(F("0")); } lcd.print(i_min);   lcd.print(F(":"));
  if (i_seg   < 10){ lcd.print(F("0")); } lcd.println(i_seg);
  //*/
}

void f_atualiza_sensores(){
  long now = millis()/1000;
  if (now - lastMsg > TempoAtualizacao){
    lastMsg = now;
    Serial.println(); //Linha em branco
    Serial.println(F("<---------------Atualizando sensores-------------->"));
    f_printTempoSerial(); //tempo desde o ultimo reset
    f_SensorDHT11();

//    Serial.print(F("i_seg: ")); Serial.println(i_seg);
//    Serial.println(); //Linha em branco
    Serial.println(F("<------------------------------------------------->"));
  } else{
    f_lcd_teste_botoes();
    delay(3000);
    f_printTempoLCD();
    delay(3000);
  }
}



class Thread{
  private:
    long _delay;
    unsigned long tempo;
    long count;
    
  public:
    Thread(long delay);
    boolean Timer();
    boolean Refresh();
    boolean posDelay();
    int getCount();
    void setCount();
};


Thread::Thread(long delay){
  _delay = delay;
  tempo = 0;
  count = 0;
}

boolean Thread::Timer (){
  return (tempo < millis())? true: false;
}

boolean Thread::Refresh(){
  tempo = millis() + _delay;
  return true;
}

boolean Thread::posDelay(){
  return (Timer()== true)? Refresh(): false;
}

int Thread::getCount(){
  return count;
}


void Thread::setCount(){
  count++;
}

Thread *a, *b, *c, *d;
long contador;






//Definicoes Setup
void setup(){
  initSerial();
  initOutput(); //Define a porta status de saida e entrada
  
  f_lcd_incio();
  f_AtivaSomTeclas(ON);
  f_buzzbeep(); //1 bip

  a = new Thread(1000);
  b = new Thread(3000);
  c = new Thread(5000);
  d = new Thread(8000);
}
void loop() {
//  iniTempo();       //atualiza variaveis time do reset
  
  //put your main code here, to run repeatedly //coloque seu código principal aqui, para executar repetidamente
  if(a->posDelay()){
    a->setCount();
    f_printTempoLCD();
  }
  if(b->posDelay()){
    b->setCount();
    f_lcd_teste_botoes();
  }
  if(c->posDelay()){
    f_SensorDHT11();
    c->setCount();
  }

  if(a->posDelay()){
    a->setCount();
    Serial.print(" A: ");
    Serial.print(a->getCount());
    Serial.print(" B: ");
    Serial.print(b->getCount());
    Serial.print(" C: ");
    Serial.print(c->getCount());
    Serial.print(" D: ");
    Serial.println(d->getCount());
  }
  if(a->posDelay()){
    a->setCount();
  }

//  f_ProgControleSerial();
  f_teclado_shied();
  lcd_key = read_LCD_buttons();  // read the buttons

}
