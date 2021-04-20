// Connection map for Arduino Mega2560:
//
// Clock pin:    6502(PHI2)   -> MEGA(2)
//
// Address pins: 6502(A0-A15) -> MEGA(A0-A15) Port F & K
// Data pins:    6502(D0-D7)  -> MEGA(37-30)  Port C
// Misc pins:    6502(VPB)    -> MEGA(49)     Port L0
//               6502(MLB)    -> MEGA(48)     Port L1   don't need this one
//               6502(SYNC)   -> MEGA(47)     Port L2
//               6502(RWB)    -> MEGA(46)     Port L3
//
// External serial board connected to Serial2 TXD2/TXR2 MEGA(16,17)

#include <Arduino.h>
#include "disasm.h"

#define CLOCK_PIN   2
#define TRACE       Serial

static void initClockInterrupt();
static void clockInterruptHandler();
static bool isClockReady();

static void initBusRead();
static void updateBusRead();
static volatile bool clockReady = false;

char outbuf[128];
static Disassembler disasm;

void setup() {
    TRACE.begin(115200);
    TRACE.println("-------------------------------------");

    initBusRead();
    initClockInterrupt();
}

void loop() {
    if (!isClockReady()) {
        return;
    }

    updateBusRead();
}

static void initClockInterrupt() {
    pinMode(CLOCK_PIN, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(CLOCK_PIN),
                    clockInterruptHandler, RISING);
}

static void clockInterruptHandler() {
    clockReady = true;
}

static bool isClockReady() {
    noInterrupts();
    bool ready = clockReady;
    clockReady = false;
    interrupts();
    return ready;
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

    bool read   = BOOL_PIN(misc, 3, true, false);
    bool sync   = BOOL_PIN(misc, 2, true, false);
    bool vector = BOOL_PIN(misc, 0, false, true);

    char r = BOOL_PIN(misc, 3, 'r', 'W');
    char s = BOOL_PIN(misc, 2, 'S', '-');
    char v = BOOL_PIN(misc, 0, '-', 'V');

    // r-V = vector
    // rS- = start instruction

    static uint16_t pc = 0;

    bool showOpcode = false;
    if (vector) {
        disasm.reset();
    }
    else if (sync) {
        disasm.reset();
        disasm.decodeByte(data);
        showOpcode = disasm.isReady();
        pc = addr;
    }
    else if (read && disasm.isDecoding() && (addr == pc + 1)) {
        disasm.decodeByte(data);
        showOpcode = disasm.isReady();
        pc = addr;
    }

    if (showOpcode) {
        sprintf(outbuf, "A:%04x D:%02x %c%c%c %s",
            addr, data, r, s, v, disasm.text());
        disasm.clear();
    }
    else {
        sprintf(outbuf, "A:%04x D:%02x %c%c%c", addr, data, r, s, v);
    }

    TRACE.println(outbuf);
}
