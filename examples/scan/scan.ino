/*
  scan.ino

  A simple sketch that configure a DRA818V and scan all the frequencies in the 
  range 134-174 MHz with a step of 12.5 kHz. 3 pins of the arduino are 
  used (PD, RX and TX) (CAUTION the DRA818 runs @3.3v). Configure the 3 macros
  and customize the DRA818::configure(...) call to your needs.

  Here is a sample output from the serial connection:

  Booting ...
  initializing I/O ... done
  initializing DRA818 ... done
  Starting ... 
  Scanning frequency 134.0000 kHz ...
  Scanning frequency 134.0125 kHz ...
  Scanning frequency 134.0250 kHz ...
  Scanning frequency 134.0375 kHz ...
  Scanning frequency 134.0500 kHz ...
  Scanning frequency 134.0625 kHz ...
  Scanning frequency 134.0750 kHz ...

  I'm using a digital pin (D10) to crontrol the PD pin of the DRA818. 
  It's not mandatory, you can just let the PD pin of the DRA818 
  to HIGH and free an arduino digital pin.


  Copyright (c) 2017, Jerome LOYET

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include <stdio.h>
#include <SoftwareSerial.h>
#include "DRA818.h"

/* Used Pins */
#define PD      10  // to the DRA818 PD pin
#define RX      3   // arduino serial RX pin to the DRA818 TX pin
#define TX      4   // arduino serial TX pin to the DRA818 RX pin

SoftwareSerial *dra_serial; // Serial connection to DRA818
DRA818 *dra;                // the DRA object once instanciated
float freq;                 // the next frequency to scan

void setup(){
  Serial.begin(9600); // for logging

  Serial.println("Booting ...");

  Serial.print("initializing I/O ... ");  
  dra_serial = new SoftwareSerial(RX, TX); // Instantiate the Software Serial Object.
  pinMode(PD, OUTPUT);                     // Power control of the DRA818
  digitalWrite(PD,HIGH);                    // start at low power
  Serial.println("done");

  Serial.print("initializing DRA818 ... ");
  /*
   * Configure DRA818V using 145.500 MHz, squelch 4, volume 8, no ctcss, 12.5 kHz bandwidth, all filters activated
   *
   * Alternative call:
   *  dra = new DRA818(dra_serial, DRA818_VHF);
   *  dra->handshake();
   *  dra->group(DRA818_12K5, 145.500, 145.500, 0, 4, 0);
   *  dra->volume(8);
   *  dra->filters(true, true, true);
   */
  dra = DRA818::configure(dra_serial, DRA818_VHF, 145.500, 145.500, 4, 8, 0, 0, DRA818_12K5, true, true, true);
  if (!dra) {
    Serial.println("\nError while configuring DRA818");
  }
  freq = DRA818_VHF_MIN;

  Serial.println("done");

  Serial.println("Starting ... ");
}

void loop(){
  char buf[9];

  if (!dra) return; // do nothing if DRA configuration failed

  dtostrf(freq, 8, 4, buf);  // convert frequency to string with right precision
  Serial.print(String("Scanning frequency ") +  String(buf) + String(" kHz ..."));
  /* scan the frequency */
  if (dra->scan(freq)) Serial.print("Found");
  Serial.println("");
  
  freq += 0.0125; //12.5kHz step
  if (freq > DRA818_VHF_MAX) freq = DRA818_VHF_MIN; // when DRA818_VHF_MAX (174.0) is reached, start over at DRA818_VHF_MIN (134.0)
}
