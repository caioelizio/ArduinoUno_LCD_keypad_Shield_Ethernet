/*********************************************************************/
#define autor       "Caio Elizio - caioelizio@gmail.com 62 98223-9516"
#define programa    "Arduino Uno Shield Ethernet "
#define versao      "v1.2"
#define dataversao  "21/02/2020"
#define placa       "Arduino Uno Shield Ethernet"
#define hostnameOTA "Shield_Ethernet"
#define cRele1      "0 "
#define cRele2      "0 "
#define cSensores   "Sensores "
/*********************************************************************/
//Definicoes Rede Ethernet
//Definicoes websever
//----------------------------------------------------------------------------------------------------------//
/*Lista de Peças:
  1 Arduino Uno
  1 Shield Ethernet
  1 Fonte 5v (carregador de celular)
  2 Rele
  Fonte 12v 1a 11,50
  1 Case ABS Impressão 3d
*/
/*Versoes
  v1.1 add webserver
*/
#include <SPI.h>
#include <String.h>
#include <Ethernet.h>

//-------------------------------------------------------------------
byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };//MAC padrão;
IPAddress ip(192, 168, 15, 50);//Define o endereco IPv4(trocar final);
IPAddress gateway(192, 168, 1, 1);      //Define o gateway
IPAddress subnet(255, 255, 255, 0); //Define a máscara de rede
EthernetServer server(80); // Porta de serviço

//Pinagem--------------------------------------------------
int AA0 = A0;//Arduino analogica A0;
int AA1 = A1;//Arduino analogica A1;
int AA2 = A2;//Arduino analogica A2;
int AA3 = A3;//Arduino analogica A3;
int AA4 = A4;//Arduino analogica A4;
//----------------------------------
int D2 = 2;//Arduino digital D2;
int D3 = 3;//Arduino digital D3;
int D4 = 4;//Arduino digital D4;
int D5 = 5;//Arduino digital D5;
int D6 = 6;//Arduino digital D6;

//Variáveis e objetos globais--------------------------------------
String readString = String(30); // string para buscar dados de endereço
boolean statusA0 = false; // Variável para o status do led
boolean statusA1 = false; // Variável para o status do led
boolean statusA2 = false; // Variável para o status do led
boolean statusA3 = false; // Variável para o status do led
boolean statusA4 = false; // Variável para o status do led
boolean statusD2 = false; // Variável para o status do led
boolean statusD3 = false; // Variável para o status do led
boolean statusD4 = false; // Variável para o status do led
boolean statusD5 = false; // Variável para o status do led
boolean statusD6 = false; // Variável para o status do led

void setup() {
  initSerial();
  iniWebsever();
  
  //-----------------------Define pino como saída-----------------------
  pinMode(AA0, OUTPUT);
  pinMode(AA1, OUTPUT);
  pinMode(AA2, OUTPUT);
  pinMode(AA3, OUTPUT);
  pinMode(AA4, OUTPUT);
  pinMode(D2, OUTPUT);
  pinMode(D3, OUTPUT);
  pinMode(D4, OUTPUT);
  pinMode(D5, OUTPUT);
  pinMode(D6, OUTPUT);
  
  delay(1000);         //Espera 1 segundo
}



//Função: inicializa comunicação serial
void initSerial() {
  //Inicia a comunicação Serial
  Serial.begin(9600); //baudrate 9600 = arduino e 115200 = esp
  delay(10);
  Serial.println(); //Linha em branco
  Serial.println(F("Iniciando Serial..."));

  Serial.println(); //Linha em branco
  Serial.print(F("Autor:   "));      Serial.println(autor);
  Serial.print(F("Sistema: "));      Serial.print(programa); Serial.print(F(" - ")); Serial.println(versao);
  Serial.print(F("Data versao: ")); Serial.println(dataversao);
  Serial.println(); //Linha em branco
}



void iniWebsever() {
  // Inicia o Ethernet
  //Ethernet.begin(mac, ip);
  Ethernet.begin(mac, ip, gateway, subnet);
  server.begin();
}  
void f_websever() {
  // Criar uma conexão de cliente
  EthernetClient client = server.available();

  if (client) {
//  if (!client) {
    Serial.println(F("nova conexao no websever"));

    while (client.connected()) {
    //Aguarde até que o cliente envie alguns dados    
//    while (!client.available()) {
      delay(1);
//    }
  
//     if (client.available()) {
        char c = client.read();
        // ler caractere por caractere vindo do HTTP
        if (readString.length() < 30) {
          // armazena os caracteres para string
          readString += (c);
        }

        //se o pedido HTTP terminou
        if (c == '\n') {
          if (readString.indexOf("a0high") >= 0) { //Recebido do Android;
            // O Led vai ser ligado
            digitalWrite(AA0, HIGH);//Arduino porta digital D2=5V;
            statusA0 = true;
          }

/*          // Se a string possui o texto L=Desligar
          if (readString.indexOf("a0low") >= 0)  { //Recebido do Android;
            // O Led vai ser desligado
            digitalWrite(AA0, LOW);//Arduino porta digital D2=0V;
            statusA0 = false;
          }
          if (readString.indexOf("a1high") >= 0)  { //Recebido do Android;
            
          }

          // Se a string possui o texto L=Desligar
          if (readString.indexOf("a1low") >= 0)  { //Recebido do Android;
            
          }
          if (readString.indexOf("a2high") >= 0)  { //Recebido do Android;
            // O Led vai ser ligado
            digitalWrite(AA2, HIGH); //Arduino porta digital D2=5V;
            statusA2 = true;
          }
          // Se a string possui o texto L=Desligar
          if (readString.indexOf("a2low") >= 0)  { //Recebido do Android;
            // O Led vai ser desligado
            digitalWrite(AA2, LOW);//Arduino porta digital D2=0V;
            statusA2 = false;
          }
          if (readString.indexOf("a3high") >= 0)  { //Recebido do Android;
            // O Led vai ser ligado
            digitalWrite(AA3, HIGH);//Arduino porta digital D2=5V;
            statusA3 = true;
          }
          // Se a string possui o texto L=Desligar
          if (readString.indexOf("a3low") >= 0)  { //Recebido do Android;
            // O Led vai ser desligado
            digitalWrite(AA3, LOW);//Arduino porta digital D2=0V;
            statusA3 = false;
          }
          if (readString.indexOf("a4high") >= 0)  { //Recebido do Android;
            // O Led vai ser ligado
            digitalWrite(AA4, HIGH);//Arduino porta digital D2=5V;
            statusA4 = true;
          }
          // Se a string possui o texto L=Desligar
          if (readString.indexOf("a4low") >= 0)  { //Recebido do Android;
            // O Led vai ser desligado
            digitalWrite(AA4, LOW);//Arduino porta digital D2=0V;
            statusA4 = false;
          }
          if (readString.indexOf("d2high") >= 0)  { //Recebido do Android;
            // O Led vai ser ligado
            digitalWrite(D2, HIGH);//Arduino porta digital D2=5V;
            statusD2 = true;
          }
          // Se a string possui o texto L=Desligar
          if (readString.indexOf("d2low") >= 0)  { //Recebido do Android;
            // O Led vai ser desligado
            digitalWrite(D2, LOW);//Arduino porta digital D2=0V;
            statusD2 = false;
          }
          if (readString.indexOf("d3high") >= 0)  { //Recebido do Android;
            // O Led vai ser ligado
            digitalWrite(D3, HIGH);//Arduino porta digital D2=5V;
            statusD3 = true;
          }
          // Se a string possui o texto L=Desligar
          if (readString.indexOf("d3low") >= 0)  { //Recebido do Android;
            // O Led vai ser desligado
            digitalWrite(D3, LOW);//Arduino porta digital D2=0V;
            statusD3 = false;
          }
          if (readString.indexOf("d4high") >= 0)  { //Recebido do Android;
            // O Led vai ser ligado
            digitalWrite(D4, HIGH);//Arduino porta digital D2=5V;
            statusD4 = true;
          }
          // Se a string possui o texto L=Desligar
          if (readString.indexOf("d4low") >= 0)  { //Recebido do Android;
            // O Led vai ser desligado
            digitalWrite(D4, LOW);//Arduino porta digital D2=0V;
            statusD4 = false;
          }
          if (readString.indexOf("d5high") >= 0)  { //Recebido do Android;
            // O Led vai ser ligado
            digitalWrite(D5, HIGH);//Arduino porta digital D2=5V;
            statusD5 = true;
          }
          // Se a string possui o texto L=Desligar
          if (readString.indexOf("d5low") >= 0)  { //Recebido do Android;
            // O Led vai ser desligado
            digitalWrite(D5, LOW);//Arduino porta digital D2=0V;
            statusD5 = false;
          }
          if (readString.indexOf("d6high") >= 0)  { //Recebido do Android;
            // O Led vai ser ligado
            digitalWrite(D6, HIGH);//Arduino porta digital D2=5V;
            statusD6 = true;
          }
          // Se a string possui o texto L=Desligar
          if (readString.indexOf("d6low") >= 0)  { //Recebido do Android;
            // O Led vai ser desligado
            digitalWrite(D6, LOW);//Arduino porta digital D2=0V;
            statusD6 = false;
          }//*/

            // Read the first line of the request
            String request = client.readStringUntil('\r');
            // Serial.print("request ") Serial.println(request);
            client.flush();
    
            // Match the request
            String buf = "";
                  
            buf += "<HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML><html>";
            buf += "<meta charset ='utf-8'/>";
            buf += "<meta name = 'viewport' content = 'width=device-width, initial-scale=1'>";
            buf += "<link href='https://maxcdn.bootstrapcdn.com/bootstrap/3.3.1/css/bootstrap.min.css' rel='stylesheet'></link>";

            buf += "<div class='col-md-5'>";

            buf += "  <div class='jumbotron'>";
            buf += "   <h1 class='text-center'>Automação Casa</h1>";
            buf += "   <h3 class='text-center'>Marcao iot</h3>";
            buf += "  </div>";
            //  buf += "<br>";
          
            //Botoes
            buf += "<div class='alert alert-info text-center' role='alert'>";
            buf += "  <h3 class='panel-title'>";
            buf += "  Quarto";
            buf += "  </h3>";
            buf += "  <br>";

///*            
            buf += "    Luz";
//            buf += "  <p>";
            buf += "    <a href=\"/rele1=ON\">";
            buf += "      <button class='btn btn-large btn-success' type='button'>";
            buf += "      Ligar";
            buf += "      </button>";
            buf += "    </a>";//*/

//            buf += "  <br>";
            
/*            buf += "    <a href=\"/rele1=OFF\">";
            buf += "      <button class='btn btn-large btn-danger' type='button'>";
            buf += "        Desligar";
            buf += "      </button>";
            buf += "    </a>";//*/
            //  buf +=      cStatusR1;
//            buf += "  </p>";//*/

            buf += "</div>";
                      
            //buf += "</div>";
            buf += "</div>";
            //FIM Botoes
        
            buf += "</div>";
            buf += "</html>";
            buf += "";
          
          //  Serial.println(F("atualizando http..."));
            client.print(buf);
//            client.flush();
            delay(1);
            Serial.println();
//*/


/*          // dados HTML de saída começando com cabeçalho padrão
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println();
          client.println("<font size='20'>");
          
          if (statusA0) {
            client.println("statusA0: azeroon");//Ethernet envia para Android;
            client.println();        //Linha em branco 
          } else {
            client.println("statusA0: azerooff");//Ethernet envia string p/ Android apenas letras
            client.println();        //Linha em branco
          }

/*          if (statusA1) {
            client.println("statusA1: aoneon");//Ethernet envia para Android;
            client.println();        //Linha em branco 
          } else {
            client.println("statusA1: aoneoff");//Ethernet envia string para Android;
            client.println();        //Linha em branco 
          }
          if (statusA2) {
            client.println("statusA0: atwoon");//Ethernet envia para Android;
            client.println();        //Linha em branco 
          } else {
            client.println("statusA0: atwooff");//Ethernet envia string para Android;
            client.println();        //Linha em branco 
          }
          if (statusA3) {
            client.println("statusA0: athreeon");//Ethernet envia para Android;
            client.println();        //Linha em branco 
          } else {
            client.println("statusA0: athreeoff");//Ethernet envia string para Android;
            client.println();        //Linha em branco 
          }
          if (statusA4) {
            client.println("statusA0: afouron");//Ethernet envia para Android;
            client.println();        //Linha em branco 
          } else {
            client.println("statusA0: afouroff");//Ethernet envia string para Android;
            client.println();        //Linha em branco 
          }
          if (statusD2) {
            client.println("statusA0: dtwoon");//Ethernet envia para Android;
            client.println();        //Linha em branco 
          } else {
            client.println("statusA0: dtwooff");//Ethernet envia string para Android;
            client.println();        //Linha em branco 
          }
          if (statusD3) {
            client.println("statusA0: dthreeon");//Ethernet envia para Android;
            client.println();        //Linha em branco 
          } else {
            client.println("statusA0: dthreeoff");//Ethernet envia string para Android;
            client.println();        //Linha em branco 
          }
          if (statusD4) {
            client.println("statusA0: dfouron");//Ethernet envia para Android;
            client.println();        //Linha em branco 
          } else {
            client.println("statusA0: dfouroff");//Ethernet envia string para Android;
            client.println();        //Linha em branco 
          }
          if (statusD5) {
            client.println("statusA0: dfiveon");//Ethernet envia para Android;
            client.println();        //Linha em branco 
          } else {
            client.println("statusA0: dfiveoff");//Ethernet envia string para Android;
            client.println();        //Linha em branco 
          }
          if (statusD6) {
            client.println("statusA0: dsixon");//Ethernet envia para Android;
            client.println();        //Linha em branco 
          } else {
            client.println("statusA0: dsixoff");//Ethernet envia string para Android;
            client.println();        //Linha em branco 
          }//*/

//          readString = ""; //limpa string para a próxima leitura
          client.stop(); // parar cliente//*/
        }

      } //while (client.connected()) {

    } //if (client) {
//  }
} //void f_websever() {


void loop() {
  f_websever();
}
