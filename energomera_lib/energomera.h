#ifndef energomera_h
#define energomera_h

#if defined(ARDUINO) && ARDUINO >= 100
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif 

typedef struct {
  double A_D = 0;
  double B_D = 0;
  double C_D = 0;
  double A_N = 0;
  double B_N = 0;
  double C_N = 0;
} Energy_Struct;

typedef struct {
  double A = 0;
  double B = 0;
  double C = 0;
} VI_Struct;
class Energomera {
    Stream* rss; // RX, TX
    uint8_t DIR;
  public:
    Energomera(Stream* strm, uint8_t DIR) {
      rss = strm;
      this->DIR = DIR;
      pinMode(DIR, OUTPUT);
    }

    long openSession() ;
    Energy_Struct readEnergy() ;
    VI_Struct readVoltage();
    VI_Struct readCurrent() ;
    double readPower();
    double readFreq();

  private:
    void LL_writeToMeter(const byte * command, uint16_t sz);
    char LL_readFromMeter() ;
    uint16_t readIntoBuffer(char* buf, uint16_t sz) ;
    void flushBuffer() ;

};
#endif // #ifndef energomera_h