extern "C" {
#include <user_interface.h>
}

#include <Arduino.h>
#include <string.h>
#include <stdio.h>

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

const char* ssid = "ESPScanServer";
const char* password = "instarea";
 
ESP8266WebServer server(80); //Server on port 80


int test=0;
struct FrameControl {
  uint8_t version:2;
  uint8_t type:2;
  uint8_t subtype:4;
  uint8_t toDS:1;
  uint8_t fromDS:1;
  uint8_t MF:1;
  uint8_t retry:1;
  uint8_t pwr:1;
  uint8_t more:1;
  uint8_t w:1;
  uint8_t o:1;
};

/*
  Following structs taken literally from Espressif SDK docs
*/
struct RxControl {
  signed rssi:8; // signal intensity of packet
  unsigned rate:4;
  unsigned is_group:1;
  unsigned:1;
  unsigned sig_mode:2; // 0:is 11n packet; 1:is not 11n packet;
  unsigned legacy_length:12; // if not 11n packet, shows length of packet.
  unsigned damatch0:1;
  unsigned damatch1:1;
  unsigned bssidmatch0:1;
  unsigned bssidmatch1:1;
  unsigned MCS:7; // if is 11n packet, shows the modulation and code used (range from 0 to 76)
  unsigned CWB:1; // if is 11n packet, shows if is HT40 packet or not
  unsigned HT_length:16;// if is 11n packet, shows length of packet.
  unsigned Smoothing:1;
  unsigned Not_Sounding:1;
  unsigned:1;
  unsigned Aggregation:1;
  unsigned STBC:2;
  unsigned FEC_CODING:1; // if is 11n packet, shows if is LDPC packet or not.
  unsigned SGI:1;
  unsigned rxend_state:8;
  unsigned ampdu_cnt:8;
  unsigned channel:4; //which channel this packet in.
  unsigned:12;
};

struct LenSeq{
  u16 len; // length of packet
  u16 seq; // serial number of packet, the high 12bits are serial number, low 14 bits are Fragment number (usually be 0)
  u8 addr3[6]; // the third address in packet
};

struct sniffer_buf{
  struct RxControl rx_ctrl;
  u8 buf[36]; // head of ieee80211 packet
  u16 cnt; // number count of packet
  struct LenSeq lenseq[1]; //length of packet
};

struct sniffer_buf2 {
  struct RxControl rx_ctrl;
  u8 buf[112];
  u16 cnt;
  u16 len; //length of packet
};

static const char MAC_FMT[] = "%02X:%02X:%02X:%02X:%02X:%02X";
static const uint8_t MAC1_BEGIN = 4;
static const uint8_t MAC2_BEGIN = 10;
static const uint8_t MAC3_BEGIN = 16;
static const uint8_t MAC_SIZE = 6;

/*
  POC packet sniffer and parser, we process only 100% valid frames that have all
  required data (incomplete frames can be parsed if verified valid)
*/
      #define POCET_ULOZENYCH_MAC 100
      int known_count=0;
      String macadresy[POCET_ULOZENYCH_MAC];
      String strankavypis;
      
static void ICACHE_FLASH_ATTR handle_pkt(uint8_t* buf, uint16_t len) {
  if (len <= 12) {
    return; // packet is invalid
  }
  if (len == 128) {
    char mStr[20];
    char macadd[20];
    sniffer_buf2 *data = (sniffer_buf2 *)buf;

    //Serial.print(F("RSSI: "));
    //Serial.println(data->rx_ctrl.rssi);
    FrameControl *fc = (FrameControl *)data->buf;
    //printf(mStr, "t: %d; s: %d", fc->type, fc->subtype);
    //Serial.println(mStr);

    
    if (fc->type == 0 && fc->subtype == 4) {
      /*
        type 0 - management frame
        subtype 4 - probe request
      */ 
      uint8_t mac2[6];
      for (size_t i = MAC2_BEGIN; i < MAC2_BEGIN + MAC_SIZE; i++) {
        mac2[i - MAC2_BEGIN] = data->buf[i];
      }
      //sprintf(mStr, MAC_FMT, mac2[0], mac2[1], mac2[2], mac2[3], mac2[4], mac2[5]);
      sprintf(macadd, MAC_FMT, mac2[0], mac2[1], mac2[2], mac2[3], mac2[4], mac2[5]);
      Serial.print(F("MAC2: "));
      //macadd==mStr;  

      Serial.println(macadd);


  Serial.print("\n"); 
int i;
i=0;
while(i<=known_count) {
  i++;
/*
    Serial.print("i:"); 
    Serial.print(i); 
    Serial.print("   known count:"); 
    Serial.print(known_count);
  */
  Serial.print("\n");
  if (i>known_count)
  {
    macadresy[i] = macstring;
    known_count++;
    strankavypis+=macstring;
    strankavypis+="\n";
    Serial.print("\n=================nova MAC adresa==============\n");
    break;
    Serial.print(macadresy[i]);
    Serial.print("\n");
  }
  else if(macstring == macadresy[i]) {
    Serial.print("rovnake\n");
    break;
  }
}
    Serial.print("\n");
for (i=0; i<=known_count;i++){
    Serial.print(macadresy[i]);
    Serial.print("\n");
}


    }
  }
}

void setup() {
  /*

  delay(10);
  wifi_set_opmode(STATION_MODE);
  wifi_set_channel(1);
  wifi_set_promiscuous_rx_cb(handle_pkt);
  delay(10);
  wifi_promiscuous_enable(1);
  */
}

void handleRoot() {


    server.send(200, "text/plain",strankavypis);
}

unsigned long timeNow = 0;
unsigned long timeLast = 0;
int x=1;
int seconds = 0;
void loop() {
  
  Serial.begin(115200);
  wifi_set_opmode(STATION_MODE);
  wifi_set_channel(1);
  wifi_set_promiscuous_rx_cb(handle_pkt);
  delay(10);
  wifi_promiscuous_enable(1);
timeLast = millis()/1000;
    Serial.print("\n");
        Serial.print("\/\//\\/\/\/\/\\/\/\/\/\/\zaciatok SNIFF/\/\/\/\/\/\/\/\/\\/\/\/\\/\/\n");
    x=1; 
  while(x==1){
      yield();
timeNow = millis()/1000; // the number of milliseconds that have passed since boot
seconds = timeNow - timeLast;//the number of seconds that have passed since the last time 60 seconds was reached
  /* Serial.print("\n");
Serial.print(seconds);
    Serial.print("\n");
    */
      if (seconds>30)x=0;
  }
   wifi_promiscuous_enable(0);
      Serial.print("\n/\/\/\/\/\/\\/\/\/\/\/\/\\//\/\/\/\/\/\/\/\\/koniec SNIFF/\/\/\/\/\/\/\/\\//\\/\/\/\/\/\/\\/\/\/\/\/\/\/\/\/\/\\n");
      
Serial.println("SERVER START\nSERVER START\nSERVER START\nSERVER START\nSERVER START\nSERVER START\nSERVER START\nSERVER START\nSERVER START\nSERVER START\n");
  WiFi.mode(WIFI_AP);           //Only Access point
  WiFi.softAP(ssid, password);  //Start HOTspot removing password will disable security
 
  IPAddress myIP = WiFi.softAPIP(); //Get IP address
  Serial.print("HotSpt IP:");
  Serial.println(myIP);
 
  server.on("/", handleRoot);      //Which routine to handle at root location
 
  server.begin();                  //Start server
  Serial.println("HTTP server started");

  timeLast = millis()/1000;
  x=1; 
  while(x==1){
server.handleClient();          //Handle client requests

timeNow = millis()/1000; // the number of milliseconds that have passed since boot
seconds = timeNow - timeLast;//the number of seconds that have passed since the last time 60 seconds was reached
/*
   Serial.print("\n");
Serial.print(seconds);
    Serial.print("\n");
 */   
      if (seconds>30){
        x=0;
      }
  }

} //koniec loop
