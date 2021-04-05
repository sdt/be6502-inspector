#include "disasm.h"

#include <stdio.h>
#include <string.h>

struct Mode {
    uint8_t byteCount;
    const char* format;
};

struct Opcode {
    uint8_t instOffset;
    uint8_t modeOffset;
};

#include "disasm-defs.h"


Disassembler::Disassembler()
    : m_bytes(0)
    , m_ok(true)
{
    m_text[0] = 0;
}

void Disassembler::reset() {
    m_bytes = 0;
    m_text[0] = 0;
    m_ok = true;
}

void Disassembler::decodeByte(uint8_t byte) {
    if (!m_ok) {
        strcpy(m_text, "<ERR>");
        return;
    }

    if (m_bytes == 0) {
        // We are at the start of an instruction, look up the opcode byte.
        const Opcode* opcode = &c_opcode[byte];
        if (opcode->instOffset == 0) {
            strcpy(m_text, "<ERR>");
            m_ok = false;
            return;
        }

        // If this is a one-byte instruction, we can decode it immediately
        const Mode* mode = &c_mode[opcode->modeOffset];
        if (mode->byteCount == 1) {
            sprintf(m_text, mode->format, c_instruction[opcode->instOffset]);
            return;
        }

        // If it's a multi-byte instruction, stash the byte for later
        m_buffer[m_bytes++] = byte;
    }
    else {
        // Stash the next byte
        m_buffer[m_bytes++] = byte;

        // Lookup the opcode and mode using the first byte, and see if we
        // have all the bytes for this instruction.
        const Opcode* opcode = &c_opcode[m_buffer[0]];
        const Mode* mode = &c_mode[opcode->modeOffset];
        if (m_bytes < mode->byteCount) {
            return;
        }

        // We have all the bytes, decode the instruction
        if (m_bytes == 2) {
            sprintf(m_text, mode->format, c_instruction[opcode->instOffset],
                    m_buffer[1]);
        }
        else {
            sprintf(m_text, mode->format, c_instruction[opcode->instOffset],
                    m_buffer[2], m_buffer[1]); // byte swap for little endian
        }
        m_bytes = 0; // clear the byte buffer
    }
}
