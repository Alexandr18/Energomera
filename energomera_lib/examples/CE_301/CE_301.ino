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

    energomera.openSession();//
      
    Energy_Struct energy = energomera.readEnergy();//
    Serial.println("A - " + String(energy.A_D ) + " | " + String(energy.A_N ));//
    Serial.println("B - " + String(energy.B_D ) + " | " + String(energy.B_N ));//
    Serial.println("C - " + String(energy.C_D ) + " | " + String(energy.C_N ));//
  
    VI_Struct voltage = energomera.readVoltage();//
    Serial.println("Voltage A " + String(voltage.A));//
    Serial.println("Voltage B " + String(voltage.B));//
    Serial.println("Voltage C " + String(voltage.C));//
  
    double power = energomera.readPower();//
    Serial.println("Current consumption = " + String(power) + " kW");//
  
    double freq = energomera.readFreq();//
    Serial.println("Mains freq: " + String(freq) + " Hz");//
    VI_Struct current = energomera.readCurrent();//
    Serial.println(String(current.A ) + "\t" + String(current.B ) + "\t" + String(current.C ));//
  }

