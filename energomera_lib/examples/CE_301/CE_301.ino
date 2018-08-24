#include <SoftwareSerial.h>//
#include "energomera.h"//
SoftwareSerial RS485 (7, 6);//
Energomera energomera(&RS485, 8);//

void setup()
{
  Serial.begin(115200);//
  RS485.begin(9600);//
}

void loop() {

    long ser = energomera.openSession();//
      Serial.println(ser);
    Energy_Struct energy = energomera.readEnergy();//
    Serial.println("energy A - " + String(energy.A_D ) + " | " + String(energy.A_N ));//
    Serial.println("energy B - " + String(energy.B_D ) + " | " + String(energy.B_N ));//
    Serial.println("energy C - " + String(energy.C_D ) + " | " + String(energy.C_N ));//
  
    VI_Struct voltage = energomera.readVoltage();//
    Serial.println("Voltage A " + String(voltage.A));//
    Serial.println("Voltage B " + String(voltage.B));//
    Serial.println("Voltage C " + String(voltage.C));//
    
    VI_Struct current = energomera.readCurrent();//
    Serial.println("current A " + String(current.A));//
    Serial.println("current B " + String(current.B));//
    Serial.println("current C " + String(current.C));//
    
    VI_Struct poweron = energomera.readPowerOnPhase();//
    Serial.println("Power A " + String(poweron.A));//
    Serial.println("Power B " + String(poweron.B));//
    Serial.println("Power C " + String(poweron.C));//
    
    VI_Struct coruu = energomera.CORUU();
    Serial.println("CORUU " + String(coruu.A));//
    Serial.println("CORUU " + String(coruu.B));//
    Serial.println("CORUU " + String(coruu.C));//
    
    VI_Struct coriu = energomera.CORIU();
     Serial.println("CORIU " + String(coriu.A));//
    Serial.println("CORIU " + String(coriu.B));//
    Serial.println("CORIU " + String(coriu.C));//
    
    double power = energomera.readPower();//
    Serial.println("Current consumption = " + String(power) + " kW");//
  
    double freq = energomera.readFreq();//
    Serial.println("Mains freq: " + String(freq) + " Hz");//
   
    long serialnum = energomera.readFullSerial();
    Serial.println("serial:"+ String(serialnum));
   
    for(;;){}
  }
