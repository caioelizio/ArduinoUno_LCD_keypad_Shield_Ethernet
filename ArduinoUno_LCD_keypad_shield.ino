/*********************************************************************/
  #define autor       "Caio Elizio - caioelizio@gmail.com 62 98223-9516"
  #define programa    "Teste shield "
  #define versao      "v1.0"
  #define dataversao  "17/02/20"
  #define placa       "Arduino Uno + LCD keypad shield"
/*********************************************************************/
//Definicoes Setup
//Definicoes programacao por serial
/*Lista de Peças: 
 * 1 Arduino Uno
 * 1 LCD_keypad_shield
 * 1 buzzer
 * 1 carregador celular 5v (Alim. Arduino)
 * 1 carregador powerbank 5v (Alim. Arduino)
*/
/*Versoes
  v1.0 ...
*/

#include <LiquidCrystal.h>
  //Pin assignments for DFRobot LCD Keypad Shield
  LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

// define some values used by the panel and buttons
int lcd_key     = -1;
int adc_key_in  = 0;

#define btnRIGHT  0
#define btnUP     1
#define btnDOWN   2
#define btnLEFT   3
#define btnSELECT 4
#define btnNONE   5

// read the buttons
int read_LCD_buttons() {
 adc_key_in = analogRead(0);      // read the value from the sensor
 // my buttons when read are centered at these valies: 0, 144, 329, 504, 741
 // we add approx 50 to those values and check to see if we are close
 if (adc_key_in > 1000) return btnNONE; // We make this the 1st option for speed reasons since it will be the most likely result

 // For V1.1 us this threshold
 if (adc_key_in < 50)   return btnRIGHT;
 if (adc_key_in < 250)  return btnUP;
 if (adc_key_in < 450)  return btnDOWN;
 if (adc_key_in < 650)  return btnLEFT;
 if (adc_key_in < 850)  return btnSELECT;

 // For V1.0 comment the other threshold and use the one below:
/*
 if (adc_key_in < 50)   return btnRIGHT;
 if (adc_key_in < 195)  return btnUP;
 if (adc_key_in < 380)  return btnDOWN;
 if (adc_key_in < 555)  return btnLEFT;
 if (adc_key_in < 790)  return btnSELECT;
*/

 return btnNONE;  // when all others fail, return this...
}

void f_teclado() {
  lcd.setCursor(0,1);             // move to the begining of the second line
  switch (lcd_key) {              // depending on which button was pushed, we perform an action
    case btnRIGHT: {
      lcd.print("RIGHT ");
      break;
    }
    case btnLEFT: {
      lcd.print("LEFT   ");
      break;
    }
    case btnUP: {
      lcd.print("UP    ");
      break;
    }
    case btnDOWN: {
      lcd.print("DOWN  ");
      break;
    }
    case btnSELECT: {
      lcd.print("SELECT");
      break;
    }
    case btnNONE: {
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
  delay(2000);
}
void f_lcd_teste_botoes() {
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Click em 1 botao"); // print a simple message
}



//Definicoes Setup
void setup() {
  f_lcd_incio();
}

void loop() {
  f_lcd_teste_botoes();
  f_teclado();

  lcd_key = read_LCD_buttons();  // read the buttons
  delay(400);
}
