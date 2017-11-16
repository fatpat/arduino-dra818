/*
  debug.ino

  A simple sketch that configure a DRA818V and scan all the frequencies in the 
  range 134-174 MHz with a step of 12.5 kHz. 3 pins of the arduino are 
  used (PD, RX and TX) (CAUTION the DRA818 runs @3.3v). Configure the 3 macros
  and customize the DRA818::configure(...) call to your needs.

  Define the macro DRA818_DEBUG in DRA818.h and recompile to activate the
  debug functions of the library.
  
  Here is a sample output from the serial connection:

  Booting ...
  initializing I/O ... done
  initializing DRA818 ... DRA818: log serial connection active
  DRA818::handshake
  -> AT+DMOCONNECT
  <- +DMOCONNECT:0
  Returned value=1
  DRA818::group
  -> AT+DMOSETGROUP=0,145.5000,145.5000,0000,4,0000
  <- +DMOSETGROUP:0
  Returned value=1
  DRA818::volume
  -> AT+DMOSETVOLUME=8
  <- +DMOSETVOLUME:0
  Returned value=1
  DRA818::filters
  -> AT+SETFILTER=1,1,1
  <- +DMOSETFILTER:0
  Returned value=1
  done
  Starting ... 
  Scanning frequency 134.0000 kHz ...DRA818::scan
  -> S+134.0000
  <- S=1
  Returned value=0
 
  Scanning frequency 134.0125 kHz ...DRA818::scan
  -> S+134.0125
  <- S=1
  Returned value=0

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
#include "DRA818.h" // uncomment the following line in DRA818.h (#define DRA818_DEBUG)

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
   * We add the &Serial parameter to tell the DRA object where to log its debug information (to the builtin serial)
   *
   * Alternative call:
   *  dra = new DRA818(dra_serial, DRA818_VHF);
   *  dra->set_log(&Serial);
   *  dra->handshake();
   *  dra->group(DRA818_12K5, 145.500, 145.500, 0, 4, 0);
   *  dra->volume(8);
   *  dra->filters(true, true, true);
   */
  dra = DRA818::configure(dra_serial, DRA818_VHF, 145.500, 145.500, 4, 8, 0, 0, DRA818_12K5, true, true, true, &Serial);
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
