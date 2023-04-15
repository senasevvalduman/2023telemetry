//Autors: sewal, b1d0
//kütüphaneler
#include <Arduino.h>
#include <stdlib.h>
#include <stdio.h>
#include <SD.h>
#include <SPI.h>

//tanımlamalar
#define F_CPU 16000000UL   // CPU frekansı tanımlandı.
#define BAUD 9600          // USART baud oranı tanımlandı.
#define UBRR_VALUE F_CPU/16/BAUD-1

//değişkenler
String command="";
String dosyaAdi;
String b2[30];
int b2i[30];
float b2f[30], b3f[30];
int where[100],j=0;
char temp;
int a=1;
int pinCS = 53;
char ayrac = ';';
char bitis = '/';
String message;

//kütüphane tanımlamaları
File myFile;

//sd kart veri yapısı
typedef struct {
byte sicaklik1[5];
byte hiz[5];
byte volt[12];
byte enerji[5];
} Signal1;
Signal1 data;

void setup(){
  //Seri Haberleşme Başlangıcı
  Serial.begin(9600);//Serial 0 started
  Serial2.begin(9600); //LoRa port
  UBRR1 = 103;//Serial 1 Started 9600 baud
  UCSR1C |= (1 << UCSZ11) | (1 << UCSZ10); 
  UCSR1B |= (1 << RXEN1) | (1 << TXEN1) | (1 << RXCIE1 );

  //Pin tanımları
  pinMode(pinCS, OUTPUT);

  sewal(); //SD kartı başlatma
}

void loop()
{
  while(1){
    a++;
    sewalLora(); //LoRa haberleşmesi
    bedo();
    delay(100);
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
        Serial.println(b2f[i]);
        //AKS verilerini sd kart verilerine eşitleme
        *(float*)data.hiz = b2f[0];
        *(float*)data.volt = b3f[1];
        *(float*)data.enerji = b2f[2];
        *(float*)data.sicaklik1 = b2f[3];
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

//Lora ile veri gönderme
void sewalLora(){
  Serial2.write((byte)0x00);
  Serial2.write(19);
  Serial2.write(12);
  Serial2.print(b2f[0]);
  Serial2.print(",");
  Serial2.print(b3f[1]);
  Serial2.print(",");
  Serial2.print(b2f[2]);
  Serial2.print(",");
  Serial2.print(b2f[3]);
  Serial2.println("");    
  delay(650); 
}

//SD kartı başlatma
void sewal(){
  if (!SD.begin(pinCS)) {
    Serial.println("SD kart başlatılamadı.");
    return;
  }
  
  Serial.println("SD kart başarılı bir şekilde başlatıldı.");
  
  dosyaAdi = "veri_" + String(millis()) + ".txt";
 
}

//sd kartın içine veri yazdırma
void bedo(){
   //if(myFile){
   // Dosya açılır ve yazma işlemi yapılır
     // Kayıt dosyası adı belirlenir
   File myFile = SD.open(dosyaAdi, FILE_WRITE);
   myFile.println("zaman_ms;hiz_kmh;T_bat_C;V_bat_C;kalan_enerji_Wh");
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
   myFile.close();
  // }
   //else{
    //Serial.println("veri.txt açılmadı");
  // }
  delay(650);
}
