#include <Arduino.h>
#include <LoRa.h>
#include <WiFi.h>
#include <vector>
#include "configuration.h"
#include "aprs_is_utils.h"
#include "station_utils.h"
#include "pins_config.h"
#include "query_utils.h"
#include "lora_utils.h"
#include "wifi_utils.h"
#include "digi_utils.h"
#include "gps_utils.h"
#include "display.h"
#include "utils.h"
/*#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <AsyncElegantOTA.h>*/

Configuration   Config;
WiFiClient      espClient;
//AsyncWebServer  server(80);

String          versionDate         = "2023.06.08";
int             myWiFiAPIndex       = 0;
int             myWiFiAPSize        = Config.wifiAPs.size();
WiFi_AP         *currentWiFi        = &Config.wifiAPs[myWiFiAPIndex];
bool            statusAfterBoot     = true;
int             stationMode         = Config.stationMode;

bool            beacon_update       = true;
uint32_t        lastBeaconTx        = 0;
uint32_t        previousWiFiMillis  = 0;
uint32_t        lastScreenOn        = millis();

std::vector<String> lastHeardStation;
std::vector<String> lastHeardStation_temp;

String firstLine, secondLine, thirdLine, fourthLine, iGateBeaconPacket;

void setup() {
  Serial.begin(115200);
  delay(1000);
  utils::setupDiplay();
  WIFI_Utils::setup();
  LoRa_Utils::setup();
  utils::validateDigiFreqs();
  iGateBeaconPacket = GPS_Utils::generateBeacon();
  /*server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "text/plain", "Hi! I am ESP32.");
  });
  AsyncElegantOTA.begin(&server);    // Start ElegantOTA
  server.begin();
  Serial.println("HTTP server started");*/  
}

void loop() {
  if (stationMode==3 || stationMode==4) {           // DigiRepeater (3 RxFreq=TxFreq / 4 RxFreq!=TxFreq)
    utils::checkDisplayInterval();
    utils::checkBeaconInterval();
    show_display(firstLine, secondLine, thirdLine, fourthLine, 0);
    DIGI_Utils::processPacket(LoRa_Utils::receivePacket());
  } else if (stationMode==1 || stationMode==2 ) {   // iGate (1 Only Rx / 2 Rx+Tx)
    WIFI_Utils::checkWiFi();
    if (!espClient.connected()) {
      APRS_IS_Utils::connect();
    }
    secondLine  = APRS_IS_Utils::checkStatus();
    show_display(firstLine, secondLine, thirdLine, fourthLine, 0);

    while (espClient.connected()) {
      utils::checkDisplayInterval();
      utils::checkBeaconInterval();
      APRS_IS_Utils::processLoRaPacket(LoRa_Utils::receivePacket());            
      if (espClient.available()) {
        String aprsisPacket;
        aprsisPacket.concat(espClient.readStringUntil('\r'));
        APRS_IS_Utils::processAPRSISPacket(aprsisPacket);
      }
    }
  }
}