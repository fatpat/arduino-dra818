#include <stdio.h>
#include <SoftwareSerial.h>
#include "DRA818.h"

#define PD      10  // Power Down pin. This need to start low, then be set high before programming.
#define RX      4
#define TX      3

#define RX_FREQ     146.050
#define TX_FREQ     146.050
#define VOLUME      8 // we wan't max output to ensure DTMF encore will be able to decode tones
#define SQUELCH     4
#define RX_CTCSS    0
#define TX_CTCSS    0
#define BANDWIDTH   DRA818_12K5

#define LOGw(a)     (Serial.write(a))
#define LOG(a)      (Serial.print(a))
#define LOGs(a)     (Serial.print(F(a)))
#define LOGln(a)    (Serial.println(a))


SoftwareSerial *dra_serial;
DRA818 *dra;

float freq;

void setup(){
  Serial.begin(9600);

  LOGln("Booting ...");

  LOGs("initializing I/O ... ");
  
  dra_serial = new SoftwareSerial(TX, RX); // Instantiate the Software Serial Object.
  pinMode(PD, OUTPUT);
  digitalWrite(PD,LOW);
  LOGln("done");

  // Now we configure the DRA818
  LOGs("initializing DRA818 ... ");
  digitalWrite(PD, HIGH); // we need high power for programming
  dra = DRA818::configure(dra_serial, DRA818_VHF, RX_FREQ, TX_FREQ, SQUELCH, VOLUME, RX_CTCSS, TX_CTCSS, BANDWIDTH, true, true, true);
  if (!dra) {
    LOGln("\nError while configuring DRA818");
  }
  freq = DRA818_VHF_MIN;

  LOGln("done");

  LOGln("Starting ... ");
}

int vol = 1;
void loop(){
  if (!dra) {
    return;
  }
  char buf[9];
  dtostrf(freq, 8, 4, buf);
  LOG(String("Scanning frequency ") +  String(buf) + String(" kHz ..."));
  if (dra->scan(freq)) LOG("Found");
  LOGln("");
  
  freq += 0.0125; //12.5kHz step
  if (freq > DRA818_VHF_MAX) freq = DRA818_VHF_MIN;
}
