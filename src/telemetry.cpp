#include "telemetry.h"
#include "configuration.h"

extern Configuration  Config;

void setup() {
  if (Config.telemetry.active) {
    bool status;
    status = bme.begin(0x77);  // Don't forget to join pins for righ direction on BME280!
    if (status) {
      Serial.println("Could not find a valid BME280 sensor, check wiring!");
    //telemetryPacket = "OK2UFO-1>APRS,TCPIP*,qAC,T2CZECH:T#144,163,078,013,080,,1111,Solar Power iGate";
    //Serial.println(telemetryPacket);
    //espClient.write((telemetryPacket + "\n").c_str());
    //telemetryPacket = "OK2UFO-1>APRS,TCPIP*,qAC,T2CZECH::OK2UFO-1 :PARM.Battery";
    //Serial.println(telemetryPacket);
    //espClient.write((telemetryPacket + "\n").c_str());
    //telemetryPacket = "OK2UFO-1>APRS,TCPIP*,qAC,T2CZECH::OK2UFO-1 :UNIT.Vdc";
    //Serial.println(telemetryPacket);
    //espClient.write((telemetryPacket + "\n").c_str());
    //telemetryPacket = "OK2UFO-1>APRS,TCPIP*,qAC,T2CZECH::OK2UFO-1 :EQNS.0,0.01,2.5";
    //Serial.println(telemetryPacket);
    //espClient.write((telemetryPacket + "\n").c_str());
    //telemetryPacket = "OK2UFO-1>APRS,TCPIP*,qAC,T2CZECH::OK2UFO-1 :BITS.11111111,Solar Power iGate";
    //Serial.println(telemetryPacket);
    //espClient.write((telemetryPacket + "\n").c_str());

    } else {
      Serial.println("init : BME280 Module  ...     done!");
    }
  } else {
    Serial.println("(TELEMETRY not 'active' in 'igate_conf.json')");
  }
}


void updateTelemetry() {
    String telemetryPacket;

      //beaconPacket = "OK2UFO-1>APRS,TCPIP*,qAC,T2CZECH::OK2UFO-1 :BITS.11111111,Solar Power iGate";
      //telemetryPacket = "OK2UFO-1>APRS,TCPIP*,qAC,T2CZECH:T#" + String(lastTelemetryPacket) + ",";

   // UPDATE
                lastTelemetryPacket++;
                //telemetryPacket = iGateBeaconPacket;
                telemetryPacket = "OK2UFO-1>APRS,TCPIP*,qAC,T2CZECH:T#" + String(lastTelemetryPacket) + ",";
                telemetryPacket += String(float((BATTERY_Utils::checkVoltages()-2.5)/0.01),0);
                telemetryPacket += ",0,0,0,0,00000000";
                Serial.println(telemetryPacket);
                espClient.write((telemetryPacket + "\n").c_str());
                // ---------------------------------------------------------

  }
