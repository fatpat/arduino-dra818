# arduino_dra818
Arduino library for DRA818 VHF/UHF Band Voice Transceiver.

Don't bother with the DRA818 API, just plug the RX/TX pin to the arduino, include <DRA818.h> and you're good to go. You can use either the builtin UART [HardwareSoftware](https://www.arduino.cc/en/Reference/HardwareSoftware) or two digital pins of your choice and the [SoftwareSerial](https://www.arduino.cc/en/Reference/SoftwareSerial) library.

## Installation
Download the last release and in arduino IDE, in the menu, select `Sketch` / `Include Library` / `Add .ZIP Library` and choose the downloaded file. You should see DRA sample files.

## Usage
:warning: The DRA818 module is powered with 3.3v and use 3.3v TTL. If a 5v arduino is used, you should convert the 5v TX pin of the arduino to the 3.3v RX pin of the DRA818. The arduino RX will be able to detect correct TTL sent as-is (3.3v) from the TX pin of the DRA818.

You can use this library in the *object style* or with a simple configuration call. Here is a sample code snippet with both usage:
```
#include <SoftwareSerial.h>
#include <DRA818.h>

dra_serial *SoftwareSerial;
dra *DRA818;

void setup() {
 dra_serial = new SoftwareSerial(3, 4); // use pin 3 and 4 for serial communication to DRA818
 
 // all in one configuration
 dra = DRA818::configure(dra_serial, DRA818_VHF, 145.500, 145.500, 4, 8, 0, 0, DRA818_12K5, true, true, true);
 
 // object-style configuration
 dra = new DRA818(dra_serial, DRA818_VHF);
 dra->handshake();
 dra->group(DRA818_12K5, 145.500, 145.500, 0, 4, 0);
 dra->volume(8);
 dra->filters(true, true, true);
}
void loop() {
}
```

## API
### Constructor
#### HardwareSerial
Must be called when a HardwareSerial connection to the DRA818 is used (aka when using the builtin arduino UART).

```DRA818(HardwareSerial *serial, uint8_t type);```
* **serial** must be a HardwareSerial object.
* **type** must be the DRA818 version used. Must be any of the following values:
  * ```DRA818_VHF```
  * ```DRA818_UHF```
#### SoftwareSerial
Must be called when a SoftwareSerial connection to the DRA818 is used (aka when not using the builtin arduino UART but using digital pins).

```DRA818(SoftwareSerial *serial, uint8_t type);```
* **serial** must be a SoftwareSerial object.
* **type** must be the DRA818 version used. Must be any of the following values:
  * ```DRA818_VHF```
  * ```DRA818_UHF```
### Low level API functions that interact directly with the DRA818 chip
The following functions are simple mappings from the DRA818 protocol.
#### Handshake
It is used to check if the module works normally. DRA818V module will send back response information when it receives this command from the host. It's sent 3 times with a timeout of 2s because the module needs time to start.

```int DRA818::handshake();```

**return value**: 1 on success, 0 otherwise
#### Group
This command is used to configure a group of module parameters. This is the main configuration routine.

```int DRA818::group(uint8_t bandwidth, float freq_tx, float freq_rx, uint8_t ctcss_tx, uint8_t squelch, uint8_t ctcss_rx);```
* **bandwidth**
* **freq_tx** is a float that represents the TX frequency. If the value is beyond the module limit (134-174 MHz for VHF and 400-470 MHz for UHF), the value is automaticaly bound to the correct range.
* **freq_rx** is a float that represents the RX frequency. Same rules apply with **freq_tx**
* **ctcss_tx** is the CTCSS TX value. Its value is bounded to the range [0000,0038]
* **squelch** is the squelch of the receiver and it is bounded to the range [0,8]
* **ctcss_rx** is the CTCSS RX value. Its value is bounded to the range [0000,0038]

**return value**: 1 on success, 0 otherwise
#### Volume
This command is used to adjust the volume of module.

** return value**: 1 on success, 0 otherwise

```int DRA818::volume(uint8_t volume);```
* **volume** is bounded to the range [1,8]

**return value**: 1 on success, 0 otherwise
#### Filters
This command is used to turn on/off Pre/de-emphasis, Highpass, Lowpass filter.

```int DRA818::filters(bool pre, bool high, bool low);```
* **pre**(true or false) to enable/disable the Pre/de-emphasis filter
* **high**(true or false) to enable/disable the Highpass filter
* **low**(true or false) to enable/disable the Lowpass filter

**return value**: 1 on success, 0 otherwise
#### Scan
It scans the wanted frequency.

```int DRA818::scan(float freq);```
* **freq** is the frequency to scan

**return value**: 1 if the scanned frequency is active, 0 otherwise
### All-in-one configuration routine
The static ```configuration``` function can be called to setup the DRA818 all in once without calling different low level API functions without passing with constructor and low level API calls.
#### HardwareSerial
Must be called when a HardwareSerial connection to the DRA818 is used (aka when using the builtin arduino UART).

```static DRA818* DRA818::configure(HardwareSerial *serial, uint8_t type, float freq_rx, float freq_tx, uint8_t squelch, uint8_t volume, uint8_t ctcss_rx, uint8_t ctcss_tx, uint8_t bandwidth, bool pre, bool high, bool low);```
* **serial** must be a HardwareSerial object.
* **type** must be the DRA818 version used. Must be any of the following values:
  * ```DRA818_VHF```
  * ```DRA818_UHF```
* **freq_rx** is a float that represents the RX frequency. Same rules apply with **freq_tx**
* **freq_tx** is a float that represents the TX frequency. If the value is beyond the module limit (134-174 MHz for VHF and 400-470 MHz for UHF), the value is automaticaly bound to the correct range.
* **squelch** is the squelch of the receiver and it is bounded to the range [0,8]
* **volume** is bounded to the range [1,8]
* **ctcss_rx** is the CTCSS RX value. Its value is bounded to the range [0000,0038]
* **ctcss_tx** is the CTCSS TX value. Its value is bounded to the range [0000,0038]
* **bandwidth**
* **pre**(true or false) to enable/disable the Pre/de-emphasis filter
* **high**(true or false) to enable/disable the Highpass filter
* **low**(true or false) to enable/disable the Lowpass filter

**return value**: a pointer to the generated DRA818 object.
#### SoftwareSerial
Must be called when a SoftwareSerial connection to the DRA818 is used (aka when not using the builtin arduino UART but using digital pins). It's the same call than the previous call with a different serial type (SoftwareSerial instead of HardwareSerial)

```static DRA818* DRA818::configure(SoftwareSerial *stream, uint8_t type, float freq_rx, float freq_tx, uint8_t squelch, uint8_t volume, uint8_t ctcss_rx, uint8_t ctcss_tx, uint8_t bandwidth, bool pre, bool high, bool low);```

## Debug
Debugging the communication with the DRA818 module is included in the code. It can be used to debug the communication between the arduino and the DRA818 module.

First, enable debuging in the library by defining `DRA818_DEBUG` in `DRA818.h`. Then the DRA818 class exposes a `set_log(Stream *)` function and the `configure()` function takes another argument (`Stream *log`). 
```
dra = DRA818::configure(dra_serial, DRA818_VHF, 145.500, 145.500, 4, 8, 0, 0, DRA818_12K5, true, true, true, &Serial);
```
`&Serial` is a pointer to the builtin UART. The DRA818 library will send debug messages. See examples/debug.

## Thanks
* [darksidelemm](https://github.com/darksidelemm) for its original module that gave me inspiration for the rewrite of the module: https://github.com/darksidelemm/dra818
* [f4goh](https://github.com/f4goh) for its project and ideas from its code: https://github.com/f4goh/DRAPRS

## License

Copyright (c) Arduino LLC. All right reserved.

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
