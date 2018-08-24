#include "energomera.h"
// открываем сессию
const byte cmd1[] = {0xaF, 0x3F, 0x21, 0x8D, 0x0A};
// читаем тип счетчика
const byte cmd2[] = {0x06, 0x30, 0x35, 0xb1, 0x8d, 0x0a};
// снимаем показания
const byte cmd3[] = {0x81, 0xd2, 0xb1, 0x82, 0xc5, 0xd4, 0x30, 0x50, 0xc5, 0x28, 0xa9, 0x03, 0xb7};
//напряжение на фазах
const byte cmd4[] = {0x81, 0xd2, 0xb1, 0x82, 0x56, 0xcf, 0xcc, 0xd4, 0x41, 0x28, 0xa9, 0x03, 0x5f};
// мощность
const byte cmd5[] = {0x81, 0xd2, 0xb1, 0x82, 0x50, 0xcf, 0xd7, 0xc5, 0x50, 0x28, 0xa9, 0x03, 0xe4};
// частота
const byte cmd6[] = {0x81, 0xd2, 0xb1, 0x82, 0xc6, 0xd2, 0xc5, 0xd1, 0x55, 0x28, 0xa9, 0x03, 0x5c};
//ток
const byte cmd7[] = {0x81, 0xd2, 0xb1, 0x82, 0xc3, 0x55, 0xd2, 0xd2, 0xc5, 0x28, 0xa9, 0x03, 0x5a};
//полный серийник
const byte cmd8[]={0x81,0xD2,0xB1,0x82,0x53,0x4E,0x55,0x4D,0x42,0x28,0xA9,0x03,0xDE};
//мощность по фазам
const byte cmd9[]={0x81 ,0xD2 ,0xB1 ,0x82 ,0x50 ,0xCF ,0xD7 ,0x50 ,0x50 ,0x28 ,0xA9 ,0x03 ,0x6F};

const byte cmd10[]={0x81,0xD2,0xB1,0x82,0xC3,0xCF,0xD2,0x55,0x55,0x28,0xA9,0x03,0xE7};

const byte cmd11[]={0x81,0xD2,0xB1,0x82,0xC3,0xCF,0xD2,0xC9,0x55,0x28,0xA9,0x03,0xDB};
char mybuf[512] = {0};


long readLongInBracet(char* buf) {
  uint16_t idx = 0;
  while (buf[idx++] != '(');

  char* smallbuf = buf + idx;
  while (buf[++idx] != ')');
  buf[idx] = 0;
  return atol(smallbuf);
}

double readDoubleInBracet(char* buf, uint16_t &idx) {

  while (buf[idx++] != '(');
  char* smallbuf = buf + idx;
  while (buf[++idx] != ')');
  buf[idx] = 0;

  //return String(smallbuf).toDouble();
  return atof(smallbuf);

}
void readDoubles(char* buf, double * output, uint16_t sz) {
  uint16_t idx = 0;
  for (uint16_t i = 0; i < sz; i++) {
    *(output++) = readDoubleInBracet(buf, idx);
  }
}


void Energomera::LL_writeToMeter(const byte * command, uint16_t sz) {
  digitalWrite(DIR, HIGH);
  rss->write(command, sz);
  digitalWrite(DIR, LOW);
}

char Energomera::LL_readFromMeter() {
  digitalWrite(DIR, LOW);
  while (!rss->available());
  return rss->read() & 0x7F;
}

// Blocking read
uint16_t Energomera::readIntoBuffer(char* buf, uint16_t sz) {
  uint16_t idx = 0;
  while (rss->available() == 0);
  long t = millis();
  for (;;) {
    if (idx == sz)
      break;
    if (millis() - t > 200) {
      break;
    }
    if (rss->available() > 0) {
      buf[idx++] = LL_readFromMeter();
      t = millis();
    }
  }
  buf[idx] = 0;
  return idx;
}
void Energomera::flushBuffer() {
  readIntoBuffer(mybuf, 512);
}

long Energomera::openSession() {
  LL_writeToMeter(cmd1, sizeof(cmd1));
  flushBuffer();

  LL_writeToMeter(cmd2, sizeof(cmd2));
  flushBuffer();
  //readIntoBuffer(mybuf, 512);
  return readLongInBracet(mybuf);
}

Energy_Struct Energomera::readEnergy() {
  LL_writeToMeter(cmd3, sizeof(cmd3));
  double energies[6] = {0};
  readIntoBuffer(mybuf, 512);
  readDoubles(mybuf, energies, 6);
  Energy_Struct output;
  output.A_D = energies[0];
  output.B_D = energies[1];
  output.C_D = energies[2];
  output.A_N = energies[3];
  output.B_N = energies[4];
  output.C_N = energies[5];
  return output;
}

VI_Struct Energomera::readVoltage() {
  LL_writeToMeter(cmd4, sizeof(cmd4));
  double vtgs[3] = {0};
  readIntoBuffer(mybuf, 512);
  readDoubles(mybuf, vtgs, 3);
  VI_Struct output;
  output.A = vtgs[0];
  output.B = vtgs[1];
  output.C = vtgs[2];
  return output;
}


VI_Struct Energomera::readCurrent() {
  LL_writeToMeter(cmd7, sizeof(cmd7));
  double vtgs[3] = {0};
  readIntoBuffer(mybuf, 512);
  readDoubles(mybuf, vtgs, 3);
  VI_Struct output;
  output.A = vtgs[0];
  output.B = vtgs[1];
  output.C = vtgs[2];
  return output;
}
VI_Struct Energomera::readPowerOnPhase() {
  LL_writeToMeter(cmd9, sizeof(cmd9));
  double vtgs[3] = {0};
  readIntoBuffer(mybuf, 512);
  readDoubles(mybuf, vtgs, 3);
  VI_Struct output;
  output.A = vtgs[0];
  output.B = vtgs[1];
  output.C = vtgs[2];
  return output;
}
VI_Struct Energomera::CORUU() {
  LL_writeToMeter(cmd10, sizeof(cmd10));
  double vtgs[3] = {0};
  readIntoBuffer(mybuf, 512);
  readDoubles(mybuf, vtgs, 3);
  VI_Struct output;
  output.A = vtgs[0];
  output.B = vtgs[1];
  output.C = vtgs[2];
  return output;
}
VI_Struct Energomera::CORIU() {
  LL_writeToMeter(cmd11, sizeof(cmd11));
  double vtgs[3] = {0};
  readIntoBuffer(mybuf, 512);
  readDoubles(mybuf, vtgs, 3);
  VI_Struct output;
  output.A = vtgs[0];
  output.B = vtgs[1];
  output.C = vtgs[2];
  return output;
}

double Energomera::readPower() {
  LL_writeToMeter(cmd5, sizeof(cmd5));
  readIntoBuffer(mybuf, 512);
  double out = 0;
  readDoubles( mybuf, &out, 1);
  return out;
}
//------
long Energomera::readFullSerial(){
  LL_writeToMeter(cmd8, sizeof(cmd8));
  flushBuffer();
  //readIntoBuffer(mybuf, 512);
  return readLongInBracet(mybuf);
}
//------

double Energomera::readFreq() {
  LL_writeToMeter(cmd6, sizeof(cmd6));
  readIntoBuffer(mybuf, 512);
  double out = 0;
  readDoubles( mybuf, &out, 1);
  return out;
}
