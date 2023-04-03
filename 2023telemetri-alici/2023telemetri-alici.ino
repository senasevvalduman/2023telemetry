#include <TimeLib.h>
#include "LoRa_E32.h"
#define F_CPU 16000000UL   // CPU frekansı tanımlandı.
#define BAUD 9600          // USART baud oranı tanımlandı.
#define UBRR_VALUE F_CPU/16/BAUD-1
String command="";
String b2[30];
int b2i[30];
float b2f[30];
float b3f[30];
int where[100],j=0;
char temp;
int a=1;
#include <Arduino.h>
#include <stdlib.h>
#include <stdio.h>
#include <SoftwareSerial.h>
#include <SD.h>
#include <SPI.h>
SoftwareSerial portLora(2, 3);
LoRa_E32 e32ttl(&portLora);

File myFile;

int pinCS = 53;

char ayrac = ';';
char bitis = '/';

typedef struct {
byte sicaklik1[10];
byte hiz[10];
byte volt[12];
byte enerji[10];
} Signal;
Signal data;

typedef struct {
byte sicaklik1[10];
byte hiz[10];
byte volt[12];
byte enerji[10];
} Signal1;
Signal1 data1;

void setup(){
  Serial.begin(9600);//Serial 0 started
  e32ttl.begin();
  pinMode(pinCS, OUTPUT);
  sewal();
  UBRR1 = 103;//Serial 1 Started 9600 baud
   UCSR1C |= (1 << UCSZ11) | (1 << UCSZ10); 
   UCSR1B |= (1 << RXEN1) | (1 << TXEN1) | (1 << RXCIE1 );

}

void loop(){bedo();
  while(1){
    a++;
    Serial.println(*(float*)data.volt);
    sdkartdeneme();
    sewalLora();
  
    delay(100);
  }

}
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
      for(int i=0;i<j-1;i++){
        b2[i]=command.substring(where[i]+1,where[i+1]);
        b2i[i]=b2[i].toInt();
        b3f[i]=b2[i].toFloat();
        b2f[i] = b2i[i];
        Serial.println(b2f[i]);
        *(float*)data.hiz = b2f[0];
        *(float*)data.volt = b3f[1];
        *(float*)data.enerji = b2f[2];
        *(float*)data.sicaklik1 = b2f[3];
        *(float*)data1.hiz = b2f[0];
        *(float*)data1.volt = b3f[1];
        *(float*)data1.enerji = b2f[2];
        *(float*)data1.sicaklik1 = b2f[3];
      }
    }
    command="";
    j=0;
    
   // USART_TransmitString(command);
  //  USART_TransmitString("  ");
    //USART_TransmitString(String(a));
   // USART_TransmitString("  \n");
    //command="";
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

void sewalLora(){
ResponseStatus rs = e32ttl.sendFixedMessage(0, 16, 12, &data, sizeof(Signal));
  Serial.println(rs.getResponseDescription());
     
  delay(650);
  
  }
  
void bedo(){
  myFile = SD.open("test.txt", FILE_WRITE);
   if(myFile){
   myFile.println("zaman_ms;hiz_kmh;T_bat_C;V_bat_C;kalan_enerji_Wh");
   myFile.print(hour());
   myFile.print(":");
   myFile.print(minute());
   myFile.print(":");
   myFile.print(second());
   myFile.print("/ ");
   myFile.print(*(float*)(data.sicaklik1));
   myFile.print(" ;");
   myFile.print(*(float*)(data.hiz));
   myFile.print(" ;");
   myFile.print(*(float*)(data.volt));
   myFile.print(" ;");
   myFile.print(*(float*)(data.enerji));
   myFile.println(" / ");
   myFile.close(); // kapatmamak gerekebilir
    Serial.println("Done.");
   }
   else{
    Serial.println("test.txt açılmadı");
   }

     
  delay(650);
  }
void sewal(){
 
    if (SD.begin())
  {
    Serial.println("SD card is ready to use.");
  } 
  else{
    Serial.println("SD card initialization failed");
    return;
  }
  
  // Create/Open file 
  
  
  // if the file opened okay, write to it:

  // Reading the file
  /*myFile = SD.open("test.txt");
  if (myFile) {
    Serial.println("Read:");
    // Reading the whole file
    while (myFile.available()) {
      Serial.write(myFile.read());
   }
    myFile.close();
  }
  else {
    Serial.println("error opening test.txt");
  }*/
}

void sdkartdeneme(){
  myFile = SD.open("test.txt", FILE_WRITE);
  
    if (myFile) {
    Serial.println("Writing to file...");
    // Write to file
    myFile.println("zaman_ms;hiz_kmh;T_bat_C;V_bat_C;kalan_enerji_Wh");
    bedo();
    delay(300);
  
  }
  // if the file didn't open, print an error:
  else {
    Serial.println("error opening test.txt");
    delay(300);
  }
  }
