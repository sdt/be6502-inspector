#ifndef INCLUDE_DISASM_H
#define INCLUDE_DISASM_H

#include <stdint.h>

class Disassembler {
public:
    Disassembler();

    bool isOk()         { return m_ok; }
    bool isReady()      { return m_text[0] != 0; }
    const char* text()  { return m_text; }
    void clear()        { m_text[0] = 0; }
    bool isDecoding()   { return m_bytes > 0; }

    void decodeByte(uint8_t byte);
    void reset();

private:
    char    m_text[16];
    uint8_t m_buffer[3];
    uint8_t m_bytes;
    bool    m_ok;
};

#endif // INCLUDE_DISASM_H
