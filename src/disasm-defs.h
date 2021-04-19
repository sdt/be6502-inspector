static const char *c_instruction[] = {
    "---",
    "ADC",
    "AND",
    "ASL",
    "BBR0",
    "BBR1",
    "BBR2",
    "BBR3",
    "BBR4",
    "BBR5",
    "BBR6",
    "BBR7",
    "BBS0",
    "BBS1",
    "BBS2",
    "BBS3",
    "BBS4",
    "BBS5",
    "BBS6",
    "BBS7",
    "BCC",
    "BCS",
    "BEQ",
    "BIT",
    "BMI",
    "BNE",
    "BPL",
    "BRA",
    "BRK",
    "BVC",
    "BVS",
    "CLC",
    "CLD",
    "CLI",
    "CLV",
    "CMP",
    "CPX",
    "CPY",
    "DEC",
    "DEX",
    "DEY",
    "EOR",
    "INC",
    "INX",
    "INY",
    "JMP",
    "JSR",
    "LDA",
    "LDX",
    "LDY",
    "LSR",
    "NOP",
    "ORA",
    "PHA",
    "PHP",
    "PHX",
    "PHY",
    "PLA",
    "PLP",
    "PLX",
    "PLY",
    "RMB0",
    "RMB1",
    "RMB2",
    "RMB3",
    "RMB4",
    "RMB5",
    "RMB6",
    "RMB7",
    "ROL",
    "ROR",
    "RTI",
    "RTS",
    "SBC",
    "SEC",
    "SED",
    "SEI",
    "SMB0",
    "SMB1",
    "SMB2",
    "SMB3",
    "SMB4",
    "SMB5",
    "SMB6",
    "SMB7",
    "STA",
    "STP",
    "STX",
    "STY",
    "STZ",
    "TAX",
    "TAY",
    "TRB",
    "TSB",
    "TSX",
    "TXA",
    "TXS",
    "TYA",
    "WAI",
};

static const Mode c_mode[] = {
    { 1, "%s"             }, // ---
    { 3, "%s $%02x%02x"   }, // abs
    { 3, "%s $%02x%02x,x" }, // abx
    { 3, "%s $%02x%02x,y" }, // aby
    { 1, "%s a"           }, // acc
    { 3, "%s ($%02x%02x,x)" }, // iax
    { 2, "%s #$%02x"      }, // imm
    { 1, "%s"             }, // imp
    { 3, "%s ($%02x%02x)" }, // ind
    { 2, "%s ($%02x)"     }, // izp
    { 2, "%s ($%02x,x)"   }, // izx
    { 2, "%s ($%02x),y"   }, // izy
    { 2, "%s $%02x"       }, // rel
    { 2, "%s $%02x"       }, // zp
    { 2, "%s $%02x,x"     }, // zpx
    { 2, "%s $%02x,y"     }, // zpy
};

static const Opcode c_opcode[] = {
    {  28,   7 }, // $00 BRK imp
    {  52,  10 }, // $01 ORA izx
    {   0,   0 }, // $02 --- ---
    {   0,   0 }, // $03 --- ---
    {  93,  13 }, // $04 TSB zp
    {  52,  13 }, // $05 ORA zp
    {   3,  13 }, // $06 ASL zp
    {  61,  13 }, // $07 RMB0 zp
    {  54,   7 }, // $08 PHP imp
    {  52,   6 }, // $09 ORA imm
    {   3,   4 }, // $0a ASL acc
    {   0,   0 }, // $0b --- ---
    {  93,   1 }, // $0c TSB abs
    {  52,   1 }, // $0d ORA abs
    {   3,   1 }, // $0e ASL abs
    {   4,  12 }, // $0f BBR0 rel
    {  26,  12 }, // $10 BPL rel
    {  52,  11 }, // $11 ORA izy
    {  52,   9 }, // $12 ORA izp
    {   0,   0 }, // $13 --- ---
    {  92,  13 }, // $14 TRB zp
    {  52,  14 }, // $15 ORA zpx
    {   3,  14 }, // $16 ASL zpx
    {  62,  13 }, // $17 RMB1 zp
    {  31,   7 }, // $18 CLC imp
    {  52,   3 }, // $19 ORA aby
    {  42,   7 }, // $1a INC imp
    {   0,   0 }, // $1b --- ---
    {  92,   1 }, // $1c TRB abs
    {  52,   2 }, // $1d ORA abx
    {   3,   2 }, // $1e ASL abx
    {   5,  12 }, // $1f BBR1 rel
    {  46,   1 }, // $20 JSR abs
    {   2,  10 }, // $21 AND izx
    {   0,   0 }, // $22 --- ---
    {   0,   0 }, // $23 --- ---
    {  23,  13 }, // $24 BIT zp
    {   2,  13 }, // $25 AND zp
    {  69,  13 }, // $26 ROL zp
    {  63,  13 }, // $27 RMB2 zp
    {  58,   7 }, // $28 PLP imp
    {   2,   6 }, // $29 AND imm
    {  69,   4 }, // $2a ROL acc
    {   0,   0 }, // $2b --- ---
    {  23,   1 }, // $2c BIT abs
    {   2,   1 }, // $2d AND abs
    {  69,   1 }, // $2e ROL abs
    {   6,  12 }, // $2f BBR2 rel
    {  24,  12 }, // $30 BMI rel
    {   2,  11 }, // $31 AND izy
    {   2,   9 }, // $32 AND izp
    {   0,   0 }, // $33 --- ---
    {  23,  14 }, // $34 BIT zpx
    {   2,  14 }, // $35 AND zpx
    {  69,  14 }, // $36 ROL zpx
    {  64,  13 }, // $37 RMB3 zp
    {  74,   7 }, // $38 SEC imp
    {   2,   3 }, // $39 AND aby
    {  38,   7 }, // $3a DEC imp
    {   0,   0 }, // $3b --- ---
    {  23,   2 }, // $3c BIT abx
    {   2,   2 }, // $3d AND abx
    {  69,   2 }, // $3e ROL abx
    {   7,  12 }, // $3f BBR3 rel
    {  71,   7 }, // $40 RTI imp
    {  41,  10 }, // $41 EOR izx
    {   0,   0 }, // $42 --- ---
    {   0,   0 }, // $43 --- ---
    {   0,   0 }, // $44 --- ---
    {  41,  13 }, // $45 EOR zp
    {  50,  13 }, // $46 LSR zp
    {  65,  13 }, // $47 RMB4 zp
    {  53,   7 }, // $48 PHA imp
    {  41,   6 }, // $49 EOR imm
    {  50,   4 }, // $4a LSR acc
    {   0,   0 }, // $4b --- ---
    {  45,   1 }, // $4c JMP abs
    {  41,   1 }, // $4d EOR abs
    {  50,   1 }, // $4e LSR abs
    {   8,  12 }, // $4f BBR4 rel
    {  29,  12 }, // $50 BVC rel
    {  41,  11 }, // $51 EOR izy
    {  41,   9 }, // $52 EOR izp
    {   0,   0 }, // $53 --- ---
    {   0,   0 }, // $54 --- ---
    {  41,  14 }, // $55 EOR zpx
    {  50,  14 }, // $56 LSR zpx
    {  66,  13 }, // $57 RMB5 zp
    {  33,   7 }, // $58 CLI imp
    {  41,   3 }, // $59 EOR aby
    {  56,   7 }, // $5a PHY imp
    {   0,   0 }, // $5b --- ---
    {   0,   0 }, // $5c --- ---
    {  41,   2 }, // $5d EOR abx
    {  50,   2 }, // $5e LSR abx
    {   9,  12 }, // $5f BBR5 rel
    {  72,   7 }, // $60 RTS imp
    {   1,  10 }, // $61 ADC izx
    {   0,   0 }, // $62 --- ---
    {   0,   0 }, // $63 --- ---
    {  89,  13 }, // $64 STZ zp
    {   1,  13 }, // $65 ADC zp
    {  70,  13 }, // $66 ROR zp
    {  67,  13 }, // $67 RMB6 zp
    {  57,   7 }, // $68 PLA imp
    {   1,   6 }, // $69 ADC imm
    {  70,   4 }, // $6a ROR acc
    {   0,   0 }, // $6b --- ---
    {  45,   8 }, // $6c JMP ind
    {   1,   1 }, // $6d ADC abs
    {  70,   1 }, // $6e ROR abs
    {  10,  12 }, // $6f BBR6 rel
    {  30,  12 }, // $70 BVS rel
    {   1,  11 }, // $71 ADC izy
    {   1,   9 }, // $72 ADC izp
    {   0,   0 }, // $73 --- ---
    {  89,  14 }, // $74 STZ zpx
    {   1,  14 }, // $75 ADC zpx
    {  70,  14 }, // $76 ROR zpx
    {  68,  13 }, // $77 RMB7 zp
    {  76,   7 }, // $78 SEI imp
    {   1,   3 }, // $79 ADC aby
    {  60,   7 }, // $7a PLY imp
    {   0,   0 }, // $7b --- ---
    {  45,   5 }, // $7c JMP iax
    {   1,   2 }, // $7d ADC abx
    {  70,   2 }, // $7e ROR abx
    {  11,  12 }, // $7f BBR7 rel
    {  27,  12 }, // $80 BRA rel
    {  85,  10 }, // $81 STA izx
    {   0,   0 }, // $82 --- ---
    {   0,   0 }, // $83 --- ---
    {  88,  13 }, // $84 STY zp
    {  85,  13 }, // $85 STA zp
    {  87,  13 }, // $86 STX zp
    {  77,  13 }, // $87 SMB0 zp
    {  40,   7 }, // $88 DEY imp
    {  23,   6 }, // $89 BIT imm
    {  95,   7 }, // $8a TXA imp
    {   0,   0 }, // $8b --- ---
    {  88,   1 }, // $8c STY abs
    {  85,   1 }, // $8d STA abs
    {  87,   1 }, // $8e STX abs
    {  12,  12 }, // $8f BBS0 rel
    {  20,  12 }, // $90 BCC rel
    {  85,  11 }, // $91 STA izy
    {  85,   9 }, // $92 STA izp
    {   0,   0 }, // $93 --- ---
    {  88,  14 }, // $94 STY zpx
    {  85,  14 }, // $95 STA zpx
    {  87,  15 }, // $96 STX zpy
    {  78,  13 }, // $97 SMB1 zp
    {  97,   7 }, // $98 TYA imp
    {  85,   3 }, // $99 STA aby
    {  96,   7 }, // $9a TXS imp
    {   0,   0 }, // $9b --- ---
    {  89,   1 }, // $9c STZ abs
    {  85,   2 }, // $9d STA abx
    {  89,   2 }, // $9e STZ abx
    {  13,  12 }, // $9f BBS1 rel
    {  49,   6 }, // $a0 LDY imm
    {  47,  10 }, // $a1 LDA izx
    {  48,   6 }, // $a2 LDX imm
    {   0,   0 }, // $a3 --- ---
    {  49,  13 }, // $a4 LDY zp
    {  47,  13 }, // $a5 LDA zp
    {  48,  13 }, // $a6 LDX zp
    {  79,  13 }, // $a7 SMB2 zp
    {  91,   7 }, // $a8 TAY imp
    {  47,   6 }, // $a9 LDA imm
    {  90,   7 }, // $aa TAX imp
    {   0,   0 }, // $ab --- ---
    {  49,   1 }, // $ac LDY abs
    {  47,   1 }, // $ad LDA abs
    {  48,   1 }, // $ae LDX abs
    {  14,  12 }, // $af BBS2 rel
    {  21,  12 }, // $b0 BCS rel
    {  47,  11 }, // $b1 LDA izy
    {  47,   9 }, // $b2 LDA izp
    {   0,   0 }, // $b3 --- ---
    {  49,  14 }, // $b4 LDY zpx
    {  47,  14 }, // $b5 LDA zpx
    {  48,  15 }, // $b6 LDX zpy
    {  80,  13 }, // $b7 SMB3 zp
    {  34,   7 }, // $b8 CLV imp
    {  47,   3 }, // $b9 LDA aby
    {  94,   7 }, // $ba TSX imp
    {   0,   0 }, // $bb --- ---
    {  49,   2 }, // $bc LDY abx
    {  47,   2 }, // $bd LDA abx
    {  48,   3 }, // $be LDX aby
    {  15,  12 }, // $bf BBS3 rel
    {  37,   6 }, // $c0 CPY imm
    {  35,  10 }, // $c1 CMP izx
    {   0,   0 }, // $c2 --- ---
    {   0,   0 }, // $c3 --- ---
    {  37,  13 }, // $c4 CPY zp
    {  35,  13 }, // $c5 CMP zp
    {  38,  13 }, // $c6 DEC zp
    {  81,  13 }, // $c7 SMB4 zp
    {  44,   7 }, // $c8 INY imp
    {  35,   6 }, // $c9 CMP imm
    {  39,   7 }, // $ca DEX imp
    {  98,   7 }, // $cb WAI imp
    {  37,   1 }, // $cc CPY abs
    {  35,   1 }, // $cd CMP abs
    {  38,   1 }, // $ce DEC abs
    {  16,  12 }, // $cf BBS4 rel
    {  25,  12 }, // $d0 BNE rel
    {  35,  11 }, // $d1 CMP izy
    {  35,   9 }, // $d2 CMP izp
    {   0,   0 }, // $d3 --- ---
    {   0,   0 }, // $d4 --- ---
    {  35,  14 }, // $d5 CMP zpx
    {  38,  14 }, // $d6 DEC zpx
    {  82,  13 }, // $d7 SMB5 zp
    {  32,   7 }, // $d8 CLD imp
    {  35,   3 }, // $d9 CMP aby
    {  55,   7 }, // $da PHX imp
    {  86,   7 }, // $db STP imp
    {   0,   0 }, // $dc --- ---
    {  35,   2 }, // $dd CMP abx
    {  38,   2 }, // $de DEC abx
    {  17,  12 }, // $df BBS5 rel
    {  36,   6 }, // $e0 CPX imm
    {  73,  10 }, // $e1 SBC izx
    {   0,   0 }, // $e2 --- ---
    {   0,   0 }, // $e3 --- ---
    {  36,  13 }, // $e4 CPX zp
    {  73,  13 }, // $e5 SBC zp
    {  42,  13 }, // $e6 INC zp
    {  83,  13 }, // $e7 SMB6 zp
    {  43,   7 }, // $e8 INX imp
    {  73,   6 }, // $e9 SBC imm
    {  51,   7 }, // $ea NOP imp
    {   0,   0 }, // $eb --- ---
    {  36,   1 }, // $ec CPX abs
    {  73,   1 }, // $ed SBC abs
    {  42,   1 }, // $ee INC abs
    {  18,  12 }, // $ef BBS6 rel
    {  22,  12 }, // $f0 BEQ rel
    {  73,  11 }, // $f1 SBC izy
    {  73,   9 }, // $f2 SBC izp
    {   0,   0 }, // $f3 --- ---
    {   0,   0 }, // $f4 --- ---
    {  73,  14 }, // $f5 SBC zpx
    {  42,  14 }, // $f6 INC zpx
    {  84,  13 }, // $f7 SMB7 zp
    {  75,   7 }, // $f8 SED imp
    {  73,   3 }, // $f9 SBC aby
    {  59,   7 }, // $fa PLX imp
    {   0,   0 }, // $fb --- ---
    {   0,   0 }, // $fc --- ---
    {  73,   2 }, // $fd SBC abx
    {  42,   2 }, // $fe INC abx
    {  19,  12 }, // $ff BBS7 rel
};
