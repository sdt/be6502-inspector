#!/usr/bin/env perl

use 5.14.0;
use warnings;
use Function::Parameters qw( :strict );

# http://www.emulator101.com/6502-addressing-modes.html
# ACCUMULATOR           lsr a       0
# IMMEDIATE             lda #$12    1
# IMPLIED               clc         0
# RELATIVE              beq offs    1 signed
# ABSOLUTE              jmp $9000   2
# ZEROPAGE              lda $35     1
# INDIRECT              jmp ($1000) 2
# ABSOLUTE_INDEXED      sta $1000,y 2
# ZEROPAGE_INDEXED_X    sta $c0,x   1
# ZEROPAGE_INDEXED_Y    sta $c0,y   1
# INDEXED_INDIRECT      lda ($20,x) 1   x only
# INDIRECT_INDEXED      lda ($86),y 1   y only

# Addressing modes:
# http://www.oxyron.de/html/opcodes02.html
# http://www.oxyron.de/html/opcodesc02.html
# imm = #$00
# zp = $00
# zpx = $00,X
# zpy = $00,Y
# izp => ($00)
# izx = ($00,X)
# izy = ($00),Y
# abs = $0000
# abx = $0000,X
# aby = $0000,Y
# iax = ($0000,X)
# ind = ($0000)
# rel = $0000 (PC-relative)
# acc = A

my @opcode_table = qw(
         0x_0    0x_1    0x_2    0x_3    0x_4    0x_5    0x_6    0x_7    0x_8    0x_9    0x_a    0x_b    0x_c    0x_d    0x_e    0x_f
	0x0_ BRK:imp ORA:izx ---:--- ---:--- TSB:zp  ORA:zp  ASL:zp  RMB0:zp PHP:imp ORA:imm ASL:acc ---:--- TSB:abs ORA:abs ASL:abs BBR0:rel
	0x1_ BPL:rel ORA:izy ORA:izp ---:--- TRB:zp  ORA:zpx ASL:zpx RMB1:zp CLC:imp ORA:aby INC:imp ---:--- TRB:abs ORA:abx ASL:abx BBR1:rel
	0x2_ JSR:abs AND:izx ---:--- ---:--- BIT:zp  AND:zp  ROL:zp  RMB2:zp PLP:imp AND:imm ROL:acc ---:--- BIT:abs AND:abs ROL:abs BBR2:rel
	0x3_ BMI:rel AND:izy AND:izp ---:--- BIT:zpx AND:zpx ROL:zpx RMB3:zp SEC:imp AND:aby DEC:imp ---:--- BIT:abx AND:abx ROL:abx BBR3:rel
	0x4_ RTI:imp EOR:izx ---:--- ---:--- ---:--- EOR:zp  LSR:zp  RMB4:zp PHA:imp EOR:imm LSR:acc ---:--- JMP:abs EOR:abs LSR:abs BBR4:rel
	0x5_ BVC:rel EOR:izy EOR:izp ---:--- ---:--- EOR:zpx LSR:zpx RMB5:zp CLI:imp EOR:aby PHY:imp ---:--- ---:--- EOR:abx LSR:abx BBR5:rel
	0x6_ RTS:imp ADC:izx ---:--- ---:--- STZ:zp  ADC:zp  ROR:zp  RMB6:zp PLA:imp ADC:imm ROR:acc ---:--- JMP:ind ADC:abs ROR:abs BBR6:rel
	0x7_ BVS:rel ADC:izy ADC:izp ---:--- STZ:zpx ADC:zpx ROR:zpx RMB7:zp SEI:imp ADC:aby PLY:imp ---:--- JMP:iax ADC:abx ROR:abx BBR7:rel
	0x8_ BRA:rel STA:izx ---:--- ---:--- STY:zp  STA:zp  STX:zp  SMB0:zp DEY:imp BIT:imm TXA:imp ---:--- STY:abs STA:abs STX:abs BBS0:rel
	0x9_ BCC:rel STA:izy STA:izp ---:--- STY:zpx STA:zpx STX:zpy SMB1:zp TYA:imp STA:aby TXS:imp ---:--- STZ:abs STA:abx STZ:abx BBS1:rel
	0xa_ LDY:imm LDA:izx LDX:imm ---:--- LDY:zp  LDA:zp  LDX:zp  SMB2:zp TAY:imp LDA:imm TAX:imp ---:--- LDY:abs LDA:abs LDX:abs BBS2:rel
	0xb_ BCS:rel LDA:izy LDA:izp ---:--- LDY:zpx LDA:zpx LDX:zpy SMB3:zp CLV:imp LDA:aby TSX:imp ---:--- LDY:abx LDA:abx LDX:aby BBS3:rel
	0xc_ CPY:imm CMP:izx ---:--- ---:--- CPY:zp  CMP:zp  DEC:zp  SMB4:zp INY:imp CMP:imm DEX:imp WAI:imp CPY:abs CMP:abs DEC:abs BBS4:rel
	0xd_ BNE:rel CMP:izy CMP:izp ---:--- ---:--- CMP:zpx DEC:zpx SMB5:zp CLD:imp CMP:aby PHX:imp STP:imp ---:--- CMP:abx DEC:abx BBS5:rel
	0xe_ CPX:imm SBC:izx ---:--- ---:--- CPX:zp  SBC:zp  INC:zp  SMB6:zp INX:imp SBC:imm NOP:imp ---:--- CPX:abs SBC:abs INC:abs BBS6:rel
	0xf_ BEQ:rel SBC:izy SBC:izp ---:--- ---:--- SBC:zpx INC:zpx SMB7:zp SED:imp SBC:aby PLX:imp ---:--- ---:--- SBC:abx INC:abx BBS7:rel
);

my %mode_table = (
    '---' => '',
    imp => '',
    imm => '#L',
    zp  => 'L',
    zpx => 'L,x',
    zpy => 'L,y',
    izp => '(L)',
    izx => '(L,x)',
    izy => '(L),y',
    abs => 'HL',
    abx => 'HL,x',
    aby => 'HL,y',
    iax => '(HL,x)',
    ind => '(HL)',
    rel => 'L',
    acc => 'a',
);

exit(main() ? 0 : 1);

fun main() {
    my @modes = make_mode_table();
    my @opcodes = make_opcode_table();
    my $indent = '    ';

    my %inst_offset = offsets_of(name => \@opcodes);
    my %mode_offset = offsets_of(name => \@modes);

    say 'static const char *c_instruction[] = {';
    for my $name (sort keys %inst_offset) {
        say $indent, qq("$name",);
    }
    say '};';
    say '';

    say 'static const Mode c_mode[] = {';
    for my $mode (@modes) {
        say sprintf('%s{ %d, %-16s }, // %s',
            $indent, $mode->{size}, qq("$mode->{pattern}"), $mode->{name});
    }
    say '};';
    say '';

    say 'static const Opcode c_opcode[] = {';
    for my $op (@opcodes) {
        say sprintf('%s{ %3d, %3d }, // %s %-3s %s',
            $indent,
            $inst_offset{ $op->{name} },
            $mode_offset{ $op->{mode} },
            $op->{opcode}, $op->{name}, $op->{mode});
    }
    say '};';

    return 1;
}

fun make_mode_table() {
    my @modes;
    for my $name (sort keys %mode_table) {
        my $pattern = $mode_table{$name};
        my $size = 1;
        if ($pattern =~ s/HL/\$%02x%02x/) {
            $size = 3;
        }
        elsif ($pattern =~ s/L/\$%02x/) {
            $size = 2;
        }
        if (length($pattern) > 0) {
            $pattern = '%s ' . $pattern;
        }
        else {
            $pattern = '%s';
        }
        push(@modes, {
            name    => $name,
            size    => $size,
            pattern => $pattern,
        });
    }
    return @modes;
}

fun make_opcode_table() {
    my @opcodes;
    my $opcode = 0;
    for (@opcode_table) {
        next if /^0x/;
        my ($name, $mode) = split(/:/);
        push(@opcodes, make_opcode($opcode, $name, $mode));
        $opcode++;
    }
    return @opcodes;
}

fun make_opcode($opcode, $name, $mode) {
    return {
        opcode => sprintf('$%02x', $opcode),
        name => $name,
        mode => $mode,
    };
}

fun offsets_of($key, $array) {
    my %hash;
    for my $i (@$array) {
        $hash{$i->{$key}} = 1;
    }
    my $offset = 0;
    return map { $_ => $offset++ } (sort keys %hash);
}
