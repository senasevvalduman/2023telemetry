//Kütüphaneler
#include <SoftwareSerial.h>

//Kütüphane Tanımlamaları
SoftwareSerial lora(10, 11);

//Değişkenler
String gelenVeri;
 
void setup() {
  //Haberleşme Başlangıcı
  Serial.begin(9600);
  lora.begin(9600);
}
 
void loop() {
  //Gelen veriyi ekrana yazdırma
  while (lora.available()) {
    gelenVeri = lora.readString();
    if(gelenVeri[0] = 'S')
    {
      Serial.print(gelenVeri);    
    }
  }
}
