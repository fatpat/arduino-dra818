/*
  DRA818.cpp - DRA818U/V Comms Library.
*/

#include "Arduino.h"
#include "DRA818.h"
#include <stdio.h>
#include <Stream.h>
#include <HardwareSerial.h>
#include <SoftwareSerial.h>

#define CTCSS_MAX           38
#define SQUELCH_MAX         8
#define VOLUME_MAX          8
#define VOLUME_MIN          1
#define TIMEOUT             2000
#define HANDSHAKE_REPEAT    3
#define SERIAL_SPEED        9600
#define SERIAL_CONFIG       SERIAL_8N1

#define CHECK(a, op, b) if (a op b) a = b

#ifdef DRA818_DEBUG
  #define LOG(action, msg) if (this->log) this->log->action(msg)
  #define SEND(msg) do {  \
    this->serial->write(msg); \
    if (this->log) this->log->write(msg);  \
    } while(0)
#else
  #define LOG(action, msg)
  #define SEND(msg) this->serial->write(msg)
#endif

DRA818::DRA818(HardwareSerial *serial, uint8_t type) {
  serial->begin(SERIAL_SPEED, SERIAL_CONFIG);
  this->init((Stream *)serial, type);
}
DRA818::DRA818(SoftwareSerial *serial, uint8_t type) {
  serial->begin(SERIAL_SPEED); // We can't configure bits/parity for SoftwareSerial, it's 8N1 as the DRA818
  this->init((Stream *)serial, type);
}

void DRA818::init(Stream *serial, uint8_t type) {
  this->serial = serial;
#ifdef DRA818_DEBUG
  this->log = NULL;
#endif
  this->type = type;
}

#ifdef DRA818_DEBUG
void DRA818::set_log(Stream *log) {
  this->log = log;
  LOG(println, F("DRA818: log serial connection active"));
}
#endif

int DRA818::read_response() {
  char ack[3];
  LOG(print, F("<- "));

  ack[2]=0;
  long start = millis();
  do {
    if (this->serial->available()) {
      ack[0] = ack[1];
      ack[1] = ack[2];
      ack[2] = this->serial->read();
      LOG(write, ack[2]);
    }
  } while (ack[2] != 0xa && (millis() - start) < TIMEOUT);
  LOG(write, '\n');

  LOG(print, F("Returned value="));
  LOG(println, ack[0] == '0' );

  return (ack[0] == '0');
}


int DRA818::group(uint8_t bw, float freq_tx, float freq_rx, uint8_t ctcss_tx, uint8_t squelch, uint8_t ctcss_rx) {
  char buffer[49];
  char buf_rx[9];
  char buf_tx[9];

  CHECK(bw, <, DRA818_12K5);
  CHECK(bw, >, DRA818_25K);

  CHECK(freq_rx, <, (this->type == DRA818_VHF ? DRA818_VHF_MIN : DRA818_UHF_MIN));
  CHECK(freq_tx, <, (this->type == DRA818_VHF ? DRA818_VHF_MIN : DRA818_UHF_MIN));

  CHECK(freq_rx, >, (this->type == DRA818_VHF ? DRA818_VHF_MAX : DRA818_UHF_MAX));
  CHECK(freq_tx, >, (this->type == DRA818_VHF ? DRA818_VHF_MAX : DRA818_UHF_MAX));

  CHECK(ctcss_rx, >, CTCSS_MAX);
  CHECK(ctcss_tx, >, CTCSS_MAX);

  CHECK(squelch, >, SQUELCH_MAX);

  dtostrf(freq_tx, 8, 4, buf_rx);
  dtostrf(freq_rx, 8, 4, buf_tx);
  sprintf(buffer, "AT+DMOSETGROUP=%01d,%s,%s,%04d,%c,%04d\r\n", bw, buf_tx, buf_rx, ctcss_tx, squelch + '0', ctcss_rx);
  LOG(print, F("-> "));
  SEND(buffer);

  return this->read_response();
}

int DRA818::handshake() {
  char i = HANDSHAKE_REPEAT;

  while (i-- > 0) {
    LOG(print, F("-> "));
    SEND("AT+DMOCONNECT\r\n");
    if (this->read_response()) {
      return true;
    }
  }

  return false;
}

int DRA818::scan(float freq) {
  char buf[10];
  dtostrf(freq, 8, 4, buf);

  LOG(print, F("-> "));
  SEND("S+");
  SEND(buf);
  SEND("\r\n");
  return read_response();
}

int DRA818::volume(uint8_t volume) {
  CHECK(volume, >, VOLUME_MAX);
  CHECK(volume, <, VOLUME_MIN);

  LOG(print, F("-> "));
  SEND("AT+DMOSETVOLUME=");
  SEND(volume + '0');
  SEND("\r\n");

  return read_response();
}

int DRA818::filters(bool pre, bool high, bool low) {
  LOG(print, F("-> "));
  SEND("AT+SETFILTER=");
  SEND('0' + (char)pre);
  SEND(",");
  SEND('0' + (char)high);
  SEND(",");
  SEND('0' + (char)low);
  SEND("\r\n");

  return read_response(); // SCAN function return 0 if there is a signal, 1 otherwise
}

static DRA818* DRA818::configure(SoftwareSerial *stream, uint8_t type, float freq_rx, float freq_tx, uint8_t squelch, uint8_t volume, uint8_t ctcss_rx, uint8_t ctcss_tx, uint8_t bandwidth, bool pre, bool high, bool low, Stream *log = NULL) {
  DRA818 *dra = new DRA818(stream, type);
  return DRA818::configure(dra, freq_rx, freq_tx, squelch, volume, ctcss_rx, ctcss_tx, bandwidth, pre, high, low, log);
}

static DRA818* DRA818::configure(HardwareSerial *stream, uint8_t type, float freq_rx, float freq_tx, uint8_t squelch, uint8_t volume, uint8_t ctcss_rx, uint8_t ctcss_tx, uint8_t bandwidth, bool pre, bool high, bool low, Stream *log = NULL) {
  DRA818 *dra = new DRA818(stream, type);
  return DRA818::configure(dra, freq_rx, freq_tx, squelch, volume, ctcss_rx, ctcss_tx, bandwidth, pre, high, low, log);
}

static DRA818* DRA818::configure(DRA818 *dra, float freq_rx, float freq_tx, uint8_t squelch, uint8_t volume, uint8_t ctcss_rx, uint8_t ctcss_tx, uint8_t bandwidth, bool pre, bool high, bool low, Stream *log = NULL) {
  int ret;
#ifdef DRA818_DEBUG
  dra->set_log(log);
#endif
  delay(TIMEOUT);
  dra->handshake();
  ret = dra->group(bandwidth, freq_tx, freq_rx, ctcss_tx, squelch, ctcss_tx);
  if (ret) ret = dra->volume(volume);
  if (ret) ret = dra->filters(pre, high, low);

  return ret ? dra : NULL;
}
