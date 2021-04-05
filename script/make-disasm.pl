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
# imm = #$00
# zp = $00
# zpx = $00,X
# zpy = $00,Y
# izx = ($00,X)
# izy = ($00),Y
# abs = $0000
# abx = $0000,X
# aby = $0000,Y
# ind = ($0000)
# rel = $0000 (PC-relative)
# acc = A

my @opcode_table = qw(
	BRK:imp ORA:izx ---:--- ---:--- ---:--- ORA:zp  ASL:zp  ---:--- PHP:imp ORA:imm ASL:acc ---:--- ---:--- ORA:abs ASL:abs ---:---
	BPL:rel ORA:izy ---:--- ---:--- ---:--- ORA:zpx ASL:zpx ---:--- CLC:imp ORA:aby ---:--- ---:--- ---:--- ORA:abx ASL:abx ---:---
	JSR:abs AND:izx ---:--- ---:--- BIT:zp  AND:zp  ROL:zp  ---:--- PLP:imp AND:imm ROL:acc ---:--- BIT:abs AND:abs ROL:abs ---:---
	BMI:rel AND:izy ---:--- ---:--- ---:--- AND:zpx ROL:zpx ---:--- SEC:imp AND:aby ---:--- ---:--- ---:--- AND:abx ROL:abx ---:---
	RTI:imp EOR:izx ---:--- ---:--- ---:--- EOR:zp  LSR:zp  ---:--- PHA:imp EOR:imm LSR:acc ---:--- JMP:abs EOR:abs LSR:abs ---:---
	BVC:rel EOR:izy ---:--- ---:--- ---:--- EOR:zpx LSR:zpx ---:--- CLI:imp EOR:aby ---:--- ---:--- ---:--- EOR:abx LSR:abx ---:---
	RTS:imp ADC:izx ---:--- ---:--- ---:--- ADC:zp  ROR:zp  ---:--- PLA:imp ADC:imm ROR:acc ---:--- JMP:ind ADC:abs ROR:abs ---:---
	BVS:rel ADC:izy ---:--- ---:--- ---:--- ADC:zpx ROR:zpx ---:--- SEI:imp ADC:aby ---:--- ---:--- ---:--- ADC:abx ROR:abx ---:---
	---:--- STA:izx ---:--- ---:--- STY:zp  STA:zp  STX:zp  ---:--- DEY:imp ---:--- TXA:imp ---:--- STY:abs STA:abs STX:abs ---:---
	BCC:rel STA:izy ---:--- ---:--- STY:zpx STA:zpx STX:zpy ---:--- TYA:imp STA:aby TXS:imp ---:--- ---:--- STA:abx ---:--- ---:---
	LDY:imm LDA:izx LDX:imm ---:--- LDY:zp  LDA:zp  LDX:zp  ---:--- TAY:imp LDA:imm TAX:imp ---:--- LDY:abs LDA:abs LDX:abs ---:---
	BCS:rel LDA:izy ---:--- ---:--- LDY:zpx LDA:zpx LDX:zpy ---:--- CLV:imp LDA:aby TSX:imp ---:--- LDY:abx LDA:abx LDX:aby ---:---
	CPY:imm CMP:izx ---:--- ---:--- CPY:zp  CMP:zp  DEC:zp  ---:--- INY:imp CMP:imm DEX:imp ---:--- CPY:abs CMP:abs DEC:abs ---:---
	BNE:rel CMP:izy ---:--- ---:--- ---:--- CMP:zpx DEC:zpx ---:--- CLD:imp CMP:aby ---:--- ---:--- ---:--- CMP:abx DEC:abx ---:---
	CPX:imm SBC:izx ---:--- ---:--- CPX:zp  SBC:zp  INC:zp  ---:--- INX:imp SBC:imm NOP:imp ---:--- CPX:abs SBC:abs INC:abs ---:---
	BEQ:rel SBC:izy ---:--- ---:--- ---:--- SBC:zpx INC:zpx ---:--- SED:imp SBC:aby ---:--- ---:--- ---:--- SBC:abx INC:abx ---:---
);
my %mode_table = (
    '---' => '',
    imp => '',
    imm => '#L',
    zp  => 'L',
    zpx => 'L,x',
    zpy => 'L,y',
    izx => '(L,x)',
    izy => '(L),y',
    abs => 'HL',
    abx => 'HL,x',
    aby => 'HL,y',
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
#    say 'enum Instruction = {';
#    for my $name (@inames) {
#        say $indent, "INST_$name,";
#    }
#    say '};';
#    say '';
#
    my %mode_offset = offsets_of(name => \@modes);
#    say 'enum Mode = {';
#    for my $name (@mnames) {
#        say $indent, "MODE_$name,";
#    }
#    say '};';
#    say '';
#
    say 'static const char *c_instruction[] = {';
    for my $name (sort keys %inst_offset) {
        say $indent, qq("$name",);
    }
    say '};';
    say '';
#
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
