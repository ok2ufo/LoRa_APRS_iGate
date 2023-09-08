#include "telemetry.h"
#include "configuration.h"

extern Configuration  Config;

void setup() {
  if (Config.telemetry.active) {
    bool status;
    status = true;
    if (status) {
      Serial.println("Sending Telemetry init packet!");

      // 4x packet for setings aprs.fi for telemetery - now only Voltage Li-POL 2,5-4,2V
      // INIT
      telemetryPacket = Config.callsign + ">APRS,TCPIP*,qAC,T2CZECH:T#100,100,100,100,100,,1111,Solar Power iGate";
      Serial.println(telemetryPacket);
      espClient.write((telemetryPacket + "\n").c_str());
      sleep(1000);
      // PARM  
      telemetryPacket = Config.callsign + ">APRS,TCPIP*,qAC,T2CZECH::OK2UFO-1 :PARM.Battery";
      Serial.println(telemetryPacket);
      espClient.write((telemetryPacket + "\n").c_str());
      sleep(1000);
      // UNITS
      telemetryPacket = Config.callsign + ">APRS,TCPIP*,qAC,T2CZECH::OK2UFO-1 :UNIT.Vdc";
      Serial.println(telemetryPacket);
      espClient.write((telemetryPacket + "\n").c_str());
      sleep(1000);
      //EQNS
      telemetryPacket = Config.callsign + ">APRS,TCPIP*,qAC,T2CZECH::OK2UFO-1 :EQNS.0,0.01,2.5";
      Serial.println(telemetryPacket);
      espClient.write((telemetryPacket + "\n").c_str());
      sleep(1000);
      //BITS
      telemetryPacket = Config.callsign + ">APRS,TCPIP*,qAC,T2CZECH::OK2UFO-1 :BITS.11111111,Solar Power iGate";
      Serial.println(telemetryPacket);
      espClient.write((telemetryPacket + "\n").c_str());
      sleep(1000);
    } else {
      Serial.println("init : Telemetry  ...     failed!");
    }
  } else {
    Serial.println("(TELEMETRY not 'active' in 'igate_conf.json')");
  }
}


void updateTelemetry() {
    String telemetryPacket;

    lastTelemetryPacket++;
    telemetryPacket = Config.callsign + ">APRS,TCPIP*,qAC,T2CZECH:T#" + String(lastTelemetryPacket) + ",";
    telemetryPacket += String(float((BATTERY_Utils::checkVoltages()-2.5)/0.01),0);
    telemetryPacket += ",0,0,0,0,00000000";
    Serial.println(telemetryPacket);
    espClient.write((telemetryPacket + "\n").c_str());
}
