//Author: b1d0

#include <GSM.h>
#include <stdio.h>
#include <string.h>
#include <ArduinoJson.h>
#include <Arduino.h>

#define PINNUMBER ""

#define GPRS_APN       "internet"
#define GPRS_LOGIN     "eva"
#define GPRS_PASSWORD  "123"

GSMClient client;
GPRS gprs;
GSM gsmAccess;

String message = "25,19,12,22";

char server[] = "37.148.213.99";
// char path[] = "/api/send/dagilmak_istiyorum";
int port = 5000;

//char server[] = "webhook.site";
//int port = 80;

long result;
boolean notConnected = true;
boolean serverConnection = false;
int i = 0;
String b2[30];
int b2i[30];
float b2f[30], b3f[30];
char temp;
String command="";
int where[100],j=0;
float VoltDonusum;

//tanımlamalar
#define F_CPU 16000000UL   // CPU frekansı tanımlandı.
#define BAUD 9600          // USART baud oranı tanımlandı.
#define UBRR_VALUE F_CPU/16/BAUD-1

StaticJsonBuffer<3000> jsonBuffer;
JsonObject& object = jsonBuffer.createObject();

void setup() {
    Serial.begin(9600);
    UBRR1 = 103;//Serial 1 Started 9600 baud
    UCSR1C |= (1 << UCSZ11) | (1 << UCSZ10); 
    UCSR1B |= (1 << RXEN1) | (1 << TXEN1) | (1 << RXCIE1 );
    while (!Serial) {
        ;
    }

    Serial.println("bedirhan naber bebegim");

    while (notConnected) {
        if ((gsmAccess.begin(PINNUMBER) == GSM_READY) & (gprs.attachGPRS(GPRS_APN, GPRS_LOGIN, GPRS_PASSWORD) == GPRS_READY)) {
            notConnected = false;
        } else {
            Serial.println("GSM'e baglanilamadi :(");
            delay(1000);
        }
    }

    Serial.println("servera baglaniliyor");
    Serial.println(" ");
}

void loop() {
    if (!client.connected()) {
        client.connect(server, port);
    }

    if (client.connected()) {
        delay(100);

        Serial.print(b2f[0]);
        Serial.print(",");
        Serial.print(VoltDonusum);
        Serial.print(",");
        Serial.print(b2f[2]);
        Serial.print(",");
        Serial.println(b2f[3]);

        client.print("GET /update?value=");
        client.print(b2f[0]);
        client.print(",");
        client.print(VoltDonusum);
        client.print(",");
        client.print(b2f[2]);
        client.print(",");
        client.print(b2f[3]);
        client.println(" HTTP/1.1");

        client.print("Host: ");
        client.println(server);

        client.println("Accept: /");
        client.println("Accept-Encoding: gzip, deflate");
        client.println("User-Agent: Arduino/1.0");
        client.print("Content-Length: ");
        client.println(object.measureLength());
        client.println();
        object.printTo(client);

        delay(2000);
        i++;
        if(i == 30){
          client.flush();
        }
    }
}

//AKS'den veri alma
ISR(USART1_RX_vect){ 
  temp=UDR1;
  if(temp!='\n'){
    command+=temp;
  }
  else{
    if(command.substring(0,2)=="T,"){
      for(int i=0;i<command.length();i++){
        if(command.charAt(i)==','){
          where[j]=i;
          j++;
        }
      }
      //Gelen verileri ayırma ve ilgili tiplere dönüştürme
      for(int i=0;i<j-1;i++){
        b2[i]=command.substring(where[i]+1,where[i+1]);
        b2i[i]=b2[i].toInt();
        b3f[i]=b2[i].toFloat();
        b2f[i] = b2i[i];
        message = b2[i];
        VoltDonusum = (b3f[1]/1000);
        //AKS verilerini sd kart verilerine eşitleme
      }
    }
    command="";
    j=0;
    }
  }    
      
  void USART_TransmitPolling(char DataByte){
  while (( UCSR1A & (1<<UDRE1)) == 0) {};
  UDR0 = DataByte;
}
void USART_TransmitString(String DataByte){
  for(int i=0;i<DataByte.length();i++){
    USART_TransmitPolling(DataByte.charAt(i));
  }  
}
