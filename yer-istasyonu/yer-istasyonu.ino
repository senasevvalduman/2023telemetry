//Kütüphaneler
#include <SoftwareSerial.h>

//Kütüphane Tanımlamaları
SoftwareSerial lora(10, 11);

//Değişkenler
String gelenVeri;
String veriler[6];
int toplam;
int kontrol;
 
void setup() {
  //Haberleşme Başlangıcı
  Serial.begin(9600);
  lora.begin(9600);
}
 
void loop() {
  //Veri alımının kontrolü
  while (lora.available()) {
    gelenVeri = lora.readString();
    //Paket başlangıç kontrolüne göre işleme devam edilir
    if(gelenVeri[0] = 'S')
    {
      //Gelen veriler dönüşümden sonra 41 byte olmalıdır, eğer değilse uygulamaya bastırılmaz
      if(kontrol == 0)
      {
        Serial.print(gelenVeri); //Gelen paket
        //Serial.println(kontrol); //Kontrol değeri
        //Serial.println(toplam); //Dönüşümden sonra toplam byte değeri
        int i = 0;
        //Gelen verileri ;'den itibaren ayırıp "veriler" paketinin içine depolar
        while (gelenVeri.length() > 0) 
        {
          int index = gelenVeri.indexOf(';');
          if (index >= 0) {
            veriler[i] = gelenVeri.substring(0, index);
            gelenVeri = gelenVeri.substring(index + 1);
            i++;
          }
          else {
            veriler[i] = gelenVeri;
            gelenVeri = "";
          }
          //Dönüşümden sonra toplamda yeni paketin kaç byte olduğu hesaplanır (5 değeri 5 tane ; için)
          toplam = sizeof(veriler[0]) + sizeof(veriler[1]) + sizeof(veriler[2]) + 
                   sizeof(veriler[3]) + sizeof(veriler[4]) + sizeof(veriler[5]) + 5;
           //Toplam byte değerinin modu alınarak modun sıfır çıkması halinde paketin doğruluğu kabul edilir
          kontrol = toplam % 41;
        }
      }
    }
  }
}
