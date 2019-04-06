#include <SimpleDHT.h>

#define ag_ismi "Tilgin-HD58hUutBupa"
#define ag_sifresi "6rFYzDt76SP6X"
#define IP "192.168.1.88"    //thingspeak.com IP adresi

int pinDHT22 = 2;
SimpleDHT22 dht22(pinDHT22);



void setup()
{
  Serial.begin(115200); //Seriport'u açıyoruz. Güncellediğimiz
  //ESP modülünün baudRate değeri 115200 olduğu için bizde Seriport'u 115200 şeklinde seçiyoruz

  Serial.println("AT"); //ESP modülümüz ile bağlantı kurulup kurulmadığını kontrol ediyoruz.

  delay(3000); //EPSP ile iletişim için 3 saniye bekliyoruz.

  if (Serial.find("OK")) {       //esp modülü ile bağlantıyı kurabilmişsek modül "AT" komutuna "OK" komutu ile geri dönüş yapıyor.
    Serial.println("AT+CWMODE=1"); //esp modülümüzün WiFi modunu STA şekline getiriyoruz. Bu mod ile modülümüz başka ağlara bağlanabilecek.
    delay(2000);
    String baglantiKomutu = String("AT+CWJAP=\"") + ag_ismi + "\",\"" + ag_sifresi + "\"";
    Serial.println(baglantiKomutu);
  }
}

void loop() {
  sicaklik_yolla();
  delay(60000);
}
void sicaklik_yolla() {
  float temperature = 0;
  float humidity = 0;
  //http://192.168.1.88/maxrest/rest/os/mxwo?_lid=wilson&_lpwd=wilson&_action=AddChange&LOCATION=OFF102&JPNUM=JP11220&siteid=BEDFORD
  Serial.println(String("AT+CIPSTART=\"TCP\",\"") + IP + "\",80");  //thingspeak sunucusuna bağlanmak için bu kodu kullanıyoruz.
  //AT+CIPSTART komutu ile sunucuya bağlanmak için sunucudan izin istiyoruz.
  //TCP burada yapacağımız bağlantı çeşidini gösteriyor. 80 ise bağlanacağımız portu gösteriyor
  delay(1500);
  
  int err = SimpleDHTErrSuccess;
  if ((err = dht22.read2(&temperature, &humidity, NULL)) != SimpleDHTErrSuccess) {
    Serial.print("Read DHT22 failed, err="); Serial.println(err);delay(2000);
    return;
  }
  
  String tweet = "POST /maxrest/rest/os/mxarduino?_lid=maxadmin&_lpwd=maxadmin&_action=AddChange&TEMP="+String(temperature)+"&HUM="+String(humidity)+" HTTP/1.1\n";
  tweet += "Host: 192.168.1.88\n";
  tweet += "Accept: */*\n";
  tweet += "User-Agent: Mozilla/5.0 (compatible; Rigor/1.0.0; http://rigor.com)\n";
  tweet += "Content-Length: 0\n";
  tweet += "Content-Type: application/x-www-form-urlencoded\n";
  tweet+="\r\n\r\n\r\n\r\n";

   
  Serial.print("AT+CIPSEND=");                    //veri yollayacağımız zaman bu komutu kullanıyoruz. Bu komut ile önce kaç tane karakter yollayacağımızı söylememiz gerekiyor.
  delay(100);
  Serial.println(tweet.length());

  if (Serial.find(">")) {                         //eğer sunucu ile iletişim sağlayıp komut uzunluğunu gönderebilmişsek ESP modülü bize ">" işareti ile geri dönüyor.
    // arduino da ">" işaretini gördüğü anda sıcaklık verisini esp modülü ile thingspeak sunucusuna yolluyor.
    Serial.println(tweet);
    Serial.println("AT+CIPCLOSE=0");
    delay(100);

  }
  else {
    Serial.println("AT+CIPCLOSE=0");
  }
}
