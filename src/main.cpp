// Connection map for Arduino Mega2560:
//
// Clock pin:    6502(PHI2)   -> MEGA(2)
//
// Address pins: 6502(A0-A15) -> MEGA(A0-A15) Port F & K
// Data pins:    6502(D0-D7)  -> MEGA(37-30)  Port C
// Misc pins:    6502(VPB)    -> MEGA(49)     Port L0
//               6502(MLB)    -> MEGA(48)     Port L1
//               6502(SYNC)   -> MEGA(47)     Port L2
//               6502(RWB)    -> MEGA(46)     Port L3
//
// External serial board connected to Serial2 TXD2/TXR2 MEGA(16,17)

#include <Arduino.h>

#define CLOCK_PIN   2
#define TRACE       Serial2

static void initClockInterrupt();
static void clockInterruptHandler();

static void initBusRead();
static void updateBusRead();
static volatile bool clockReady = false;

char outbuf[128];

void setup() {
    TRACE.begin(115200);
    TRACE.println("-------------------------------------");

    initBusRead();
    initClockInterrupt();
}

void loop() {
    while (1) {
        noInterrupts();
        bool ready = clockReady;
        clockReady = false;
        interrupts();

        if (!ready) {
            break;
        }

        updateBusRead();
    }
}

static void initClockInterrupt() {
    pinMode(CLOCK_PIN, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(CLOCK_PIN),
                    clockInterruptHandler, RISING);
}

static void clockInterruptHandler() {
    clockReady = true;
}

static void initBusRead() {
    DDRF = 0;
    DDRK = 0;    // address lines are input
    DDRC = 0;    // data lines are input
    DDRL = 0;    // misc input lines
}

#define BIT_IS_SET(byteValue, bitNumber) \
    (((byteValue) & (1<<(bitNumber))) != 0)

#define BOOL_PIN(byteValue, bitNumber, onValue, offValue) \
    (BIT_IS_SET(byteValue, bitNumber) ? (onValue) : (offValue))

static void updateBusRead() {
    uint16_t loAddr = PINF;
    uint16_t hiAddr = PINK;
    uint16_t addr = loAddr | (hiAddr << 8);

    uint8_t data = PINC;
    uint8_t misc = PINL;

    if (BIT_IS_SET(misc, 2)) {
        TRACE.println();
    }

    char vpb  = BOOL_PIN(misc, 0, '-', 'V');
    char mlb  = BOOL_PIN(misc, 1, '-', 'M');
    char sync = BOOL_PIN(misc, 2, 'S', '-');
    char rwb  = BOOL_PIN(misc, 3, 'R', 'W');
    sprintf(outbuf, "A:%04x D:%02x %c%c%c%c", addr, data, vpb, mlb, sync, rwb);

    TRACE.println(outbuf);
}
