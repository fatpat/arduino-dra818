/*
  DRA818.h

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

#ifndef DRA818_h
#define DRA818_h

#include <Stream.h>
#include <HardwareSerial.h>
#include <SoftwareSerial.h>

#define DRA818_VHF        0x0
#define DRA818_UHF        0x1
#define DRA818_VHF_MIN    134.0
#define DRA818_VHF_MAX    174.0
#define DRA818_UHF_MIN    400.0
#define DRA818_UHF_MAX    470.0
#define DRA818_12K5       0x0
#define DRA818_25K        0x1
//#define DRA818_DEBUG

class DRA818 {
    public:
        // Constructors
        DRA818(HardwareSerial *serial, uint8_t type);
        DRA818(SoftwareSerial *serial, uint8_t type);
        void init(Stream *serial, uint8_t type);

        // low level DRA818 function
        int group(uint8_t bandwidth, float freq_tx, float freq_rx, uint8_t ctcss_tx, uint8_t squelch, uint8_t ctcss_rx);
        int handshake();
        int scan(float freq);
        int volume(uint8_t volume);
        int filters(bool pre, bool high, bool low);
        int rssi();


        // serial connection to DRA818
        Stream *serial;

#ifdef DRA818_DEBUG
        // log Serial interface
        void set_log(Stream *log);
        Stream *log;
#endif

        // all-in-one configuration functions
        static DRA818* configure(SoftwareSerial *stream, uint8_t type, float freq_rx, float freq_tx, uint8_t squelch, uint8_t volume, uint8_t ctcss_rx, uint8_t ctcss_tx, uint8_t bandwidth, bool pre, bool high, bool low, Stream *log = NULL);
        static DRA818* configure(HardwareSerial *stream, uint8_t type, float freq_rx, float freq_tx, uint8_t squelch, uint8_t volume, uint8_t ctcss_rx, uint8_t ctcss_tx, uint8_t bandwidth, bool pre, bool high, bool low, Stream *log = NULL);

    private:
        uint8_t type;

        int read_response();

        static DRA818* configure(DRA818 *dra, float freq_rx, float freq_tx, uint8_t squelch, uint8_t volume, uint8_t ctcss_rx, uint8_t ctcss_tx, uint8_t bandwidth, bool pre, bool high, bool low, Stream *log = NULL);
};
#endif
