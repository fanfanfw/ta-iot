//include library untuk WiFi
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>

//konfigurasi WiFi
const char* ssid = "pampam";
const char* password = "pampam1234";

String url;
 
//siapkan variabel untuk sensor api dan lampu LED
#define pinSensor 5 //D1 = GPI05
#define pinLED 4 //D1 = GPI04

WiFiClient client;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode (pinSensor, INPUT);
  pinMode(pinLED, OUTPUT);

  //koneksi ke wifi
  WiFi.hostname("NodeMCU");
  WiFi.begin(ssid, password);

  //uji koneksi wifi
  while(WiFi.status() != WL_CONNECTED)
  {
    digitalWrite(pinLED, LOW);
    delay(500);
  }
  digitalWrite(pinLED, HIGH);

}

void loop() {
  // put your main code here, to run repeatedly:
  int api = digitalRead(pinSensor);
  if(api == 0) 
  {
    Serial.println("Tidak Ada Api");
  }else
  {
    Serial.println("Ada Titik Api");
    kirim_wa("Api Terdeteksi Di Rumah Anda!!!!");
  }

}

void kirim_wa(String pesan)
{
  url = "http://api.callmebot.com/whatsapp.php?phone=6289525521887&text="+ 
  urlencode(pesan) +"&apikey=2458124";
  postData();
}

void postData()
{
  int httpCode;
  HTTPClient http;
  http.begin(client, url);
  httpCode = http.POST(url);
  if(httpCode==200)
  {
    Serial.println("Notifikasi WhatsApp Berhasil Terkirim");
  }else
  {
    Serial.println("Notifikasi WhatsApp Gagal");
  }
  http.end();
}

String urlencode(String str)
{
  String encodedString="";
  char c;
  char code0, code1, code2;
  for(int i=0; i<str.length(); i++)
  {
    c = str.charAt(i);
    if (c == ' ')
    {
      encodedString += '+';
    }else if(isalnum(c)){
      encodedString += c;
    }else
    {
      code1 = (c & 0xf) + '0';
      if((c & 0xf) > 9) {
        code1 = (c & 0xf ) - 10 + 'A' ;
      }
      c = (c>>4) &0xf;
      code0 = c+'0';
      if(c > 9) {
        code0 = c - 10 + 'A';
      }
      code2 = '\0';
      encodedString += '%';
      encodedString += code0;
      encodedString += code1;
    }
    yield();
  }



  Serial.println(encodedString);
  return encodedString;
}