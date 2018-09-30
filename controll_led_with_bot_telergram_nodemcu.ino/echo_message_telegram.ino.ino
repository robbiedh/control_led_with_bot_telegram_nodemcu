
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
 
// Inicializamos la conexion WIFI con el Router
char ssid[] = "open_wrt";   // SSID Name
char password[] = "pg"; // password of SSID
 
// Inicializar Telegram BOT
#define BOTtoken "your Token"  // Your tokne  of bor telegram, you can get it  from bot father
 
WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);
 
int Bot_mtbs = 50; //Tiempo medio entre el escaneo de mensajes
long Bot_lasttime;   //exploracion de el ultimo mensaje
bool Start = false;
String chat_id2;
bool flag = false;
 
void handleNewMessages(int numNewMessages) {
  Serial.println("handleNewMessages");
  Serial.println(String(numNewMessages));
  
  for (int i = 0; i < numNewMessages; i++) {
    String chat_id = String(bot.messages[i].chat_id);
    chat_id2 = chat_id;
    String text = bot.messages[i].text;
 
    String from_name = bot.messages[i].from_name;
    if (from_name == "") from_name = "Guest";
 
    if (text == "/start") {
      String welcome = "Selamat Datang , " + from_name + ".\n";
    
    
      bot.sendMessage(chat_id, welcome);
    }
 
    if (text == "/options") {
      String keyboardJson = "[[\"/ledOn\", \"/ledOff\"],[\"/sensor\"]]";
      bot.sendMessageWithReplyKeyboard(chat_id, "Options :", "", keyboardJson, true);
    }
 
    if (text == "/ledOn") {
      digitalWrite(13, HIGH);
      bot.sendMessage(chat_id, "Led is ON");
    }
 
    if (text == "/ledOff") {
      digitalWrite(13, LOW);
      bot.sendMessage(chat_id, "Led is OFF");
    }
 
    if (text == "/sensor") {
      if (digitalRead(4) == 0) {
        bot.sendMessage(chat_id, "Sensor ");
      } else {
        bot.sendMessage(chat_id, "Sensor ");
      }
    }
  }
 
}
 
 
void setup() {
  Serial.begin(9600);
 
  
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);
 

  Serial.print("Connecting  a Wifi: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
 
  Serial.println("");
  Serial.println("WiFi conectada");
  pinMode(13, OUTPUT);
  pinMode(4, INPUT_PULLUP);
}
 
void loop() {
  if (millis() > Bot_lasttime + Bot_mtbs)  {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);
 
    while (numNewMessages) {
      Serial.println("get respons");
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }
 
    if (digitalRead(4) == 1) {
      if (flag == false) {
        bot.sendMessage(chat_id2, "sensor");
        flag = true;
      }
    } else {
      flag = false;
    }
 
 
    Bot_lasttime = millis();
  }
}
