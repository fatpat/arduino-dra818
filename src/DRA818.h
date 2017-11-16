/*
    DRA818.h - DRA818U/V Comms Library.
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
#define DRA818_DEBUG

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

        // serial connection to DRA818
        Stream *serial;

#ifdef DRA818_DEBUG
        // log Serial interface
        void set_log(Stream *log);
        Stream *log;

        // all-in-one configuration functions
        static DRA818* configure(SoftwareSerial *stream, uint8_t type, float freq_rx, float freq_tx, uint8_t squelch, uint8_t volume, uint8_t ctcss_rx, uint8_t ctcss_tx, uint8_t bandwidth, bool pre, bool high, bool low, Stream *log = NULL);
        static DRA818* configure(HardwareSerial *stream, uint8_t type, float freq_rx, float freq_tx, uint8_t squelch, uint8_t volume, uint8_t ctcss_rx, uint8_t ctcss_tx, uint8_t bandwidth, bool pre, bool high, bool low, Stream *log = NULL);
#else
        static DRA818* configure(SoftwareSerial *stream, uint8_t type, float freq_rx, float freq_tx, uint8_t squelch, uint8_t volume, uint8_t ctcss_rx, uint8_t ctcss_tx, uint8_t bandwidth, bool pre, bool high, bool low);
        static DRA818* configure(HardwareSerial *stream, uint8_t type, float freq_rx, float freq_tx, uint8_t squelch, uint8_t volume, uint8_t ctcss_rx, uint8_t ctcss_tx, uint8_t bandwidth, bool pre, bool high, bool low);
#endif

    private:
        uint8_t type;

        int read_response();

        static DRA818* configure(DRA818 *dra, float freq_rx, float freq_tx, uint8_t squelch, uint8_t volume, uint8_t ctcss_rx, uint8_t ctcss_tx, uint8_t bandwidth, bool pre, bool high, bool low
#ifdef DRA818_DEBUG
        , Stream *log = NULL
#endif
        );
};
#endif
