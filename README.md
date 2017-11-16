# arduino_dra818
Arduino library for DRA818 VHF/UHF Band Voice Transceiver
## API
### Constructor
#### HardwareSerial
Must be called when a HardwareSerial connection to the DRA818 is used (aka when using the builtin arduino UART).

```DRA818(HardwareSerial *serial, uint8_t type);```
* **serial** must be a SoftwareSerial object.
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
