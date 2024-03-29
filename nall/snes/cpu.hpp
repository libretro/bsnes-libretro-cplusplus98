#ifndef NALL_SNES_CPU_HPP
#define NALL_SNES_CPU_HPP

namespace nall {

struct SNESCPU {
  enum : unsigned {
    Implied,        //
    Constant,       //#$00
    AccumConstant,  //#$00
    IndexConstant,  //#$00
    Direct,         //$00
    DirectX,        //$00,x
    DirectY,        //$00,y
    IDirect,        //($00)
    IDirectX,       //($00,x)
    IDirectY,       //($00),y
    ILDirect,       //[$00]
    ILDirectY,      //[$00],y
    Address,        //$0000
    AddressX,       //$0000,x
    AddressY,       //$0000,y
    IAddressX,      //($0000,x)
    ILAddress,      //[$0000]
    PAddress,       //PBR:$0000
    PIAddress,      //PBR:($0000)
    Long,           //$000000
    LongX,          //$000000,x
    Stack,          //$00,s
    IStackY,        //($00,s),y
    BlockMove,      //$00,$00
    RelativeShort,  //+/- $00
    RelativeLong,   //+/- $0000
  };

  struct OpcodeInfo {
    char name[4];
    unsigned mode;
  };

  static const OpcodeInfo opcodeInfo[256];
};

const SNESCPU::OpcodeInfo SNESCPU::opcodeInfo[256] = {
  //0x00 - 0x0f
  { "brk", Constant },
  { "ora", IDirectX },
  { "cop", Constant },
  { "ora", Stack },

  { "tsb", Direct },
  { "ora", Direct },
  { "asl", Direct },
  { "ora", ILDirect },

  { "php", Implied },
  { "ora", AccumConstant },
  { "asl", Implied },
  { "phd", Implied },

  { "tsb", Address },
  { "ora", Address },
  { "asl", Address },
  { "ora", Long },

  //0x10 - 0x1f
  { "bpl", RelativeShort },
  { "ora", IDirectY },
  { "ora", IDirect },
  { "ora", IStackY },

  { "trb", Direct },
  { "ora", DirectX },
  { "asl", DirectX },
  { "ora", ILDirectY },

  { "clc", Implied },
  { "ora", AddressY },
  { "inc", Implied },
  { "tcs", Implied },

  { "trb", Address },
  { "ora", AddressX },
  { "asl", AddressX },
  { "ora", LongX },

  //0x20 - 0x2f
  { "jsr", Address },
  { "and", IDirectX },
  { "jsl", Long },
  { "and", Stack },

  { "bit", Direct },
  { "and", Direct },
  { "rol", Direct },
  { "and", ILDirect },

  { "plp", Implied },
  { "and", AccumConstant },
  { "rol", Implied },
  { "pld", Implied },

  { "bit", Address },
  { "and", Address },
  { "rol", Address },
  { "and", Long },

  //0x30 - 0x3f
  { "bmi", RelativeShort },
  { "and", IDirectY },
  { "and", IDirect },
  { "and", IStackY },

  { "bit", DirectX },
  { "and", DirectX },
  { "rol", DirectX },
  { "and", ILDirectY },

  { "sec", Implied },
  { "and", AddressY },
  { "dec", Implied },
  { "tsc", Implied },

  { "bit", AddressX },
  { "and", AddressX },
  { "rol", AddressX },
  { "and", LongX },

  //0x40 - 0x4f
  { "rti", Implied },
  { "eor", IDirectX },
  { "wdm", Constant },
  { "eor", Stack },

  { "mvp", BlockMove },
  { "eor", Direct },
  { "lsr", Direct },
  { "eor", ILDirect },

  { "pha", Implied },
  { "eor", AccumConstant },
  { "lsr", Implied },
  { "phk", Implied },

  { "jmp", PAddress },
  { "eor", Address },
  { "lsr", Address },
  { "eor", Long },

  //0x50 - 0x5f
  { "bvc", RelativeShort },
  { "eor", IDirectY },
  { "eor", IDirect },
  { "eor", IStackY },

  { "mvn", BlockMove },
  { "eor", DirectX },
  { "lsr", DirectX },
  { "eor", ILDirectY },

  { "cli", Implied },
  { "eor", AddressY },
  { "phy", Implied },
  { "tcd", Implied },

  { "jml", Long },
  { "eor", AddressX },
  { "lsr", AddressX },
  { "eor", LongX },

  //0x60 - 0x6f
  { "rts", Implied },
  { "adc", IDirectX },
  { "per", Address },
  { "adc", Stack },

  { "stz", Direct },
  { "adc", Direct },
  { "ror", Direct },
  { "adc", ILDirect },

  { "pla", Implied },
  { "adc", AccumConstant },
  { "ror", Implied },
  { "rtl", Implied },

  { "jmp", PIAddress },
  { "adc", Address },
  { "ror", Address },
  { "adc", Long },

  //0x70 - 0x7f
  { "bvs", RelativeShort },
  { "adc", IDirectY },
  { "adc", IDirect },
  { "adc", IStackY },

  { "stz", DirectX },
  { "adc", DirectX },
  { "ror", DirectX },
  { "adc", ILDirectY },

  { "sei", Implied },
  { "adc", AddressY },
  { "ply", Implied },
  { "tdc", Implied },

  { "jmp", IAddressX },
  { "adc", AddressX },
  { "ror", AddressX },
  { "adc", LongX },

  //0x80 - 0x8f
  { "bra", RelativeShort },
  { "sta", IDirectX },
  { "brl", RelativeLong },
  { "sta", Stack },

  { "sty", Direct },
  { "sta", Direct },
  { "stx", Direct },
  { "sta", ILDirect },

  { "dey", Implied },
  { "bit", AccumConstant },
  { "txa", Implied },
  { "phb", Implied },

  { "sty", Address },
  { "sta", Address },
  { "stx", Address },
  { "sta", Long },

  //0x90 - 0x9f
  { "bcc", RelativeShort },
  { "sta", IDirectY },
  { "sta", IDirect },
  { "sta", IStackY },

  { "sty", DirectX },
  { "sta", DirectX },
  { "stx", DirectY },
  { "sta", ILDirectY },

  { "tya", Implied },
  { "sta", AddressY },
  { "txs", Implied },
  { "txy", Implied },

  { "stz", Address },
  { "sta", AddressX },
  { "stz", AddressX },
  { "sta", LongX },

  //0xa0 - 0xaf
  { "ldy", IndexConstant },
  { "lda", IDirectX },
  { "ldx", IndexConstant },
  { "lda", Stack },

  { "ldy", Direct },
  { "lda", Direct },
  { "ldx", Direct },
  { "lda", ILDirect },

  { "tay", Implied },
  { "lda", AccumConstant },
  { "tax", Implied },
  { "plb", Implied },

  { "ldy", Address },
  { "lda", Address },
  { "ldx", Address },
  { "lda", Long },

  //0xb0 - 0xbf
  { "bcs", RelativeShort },
  { "lda", IDirectY },
  { "lda", IDirect },
  { "lda", IStackY },

  { "ldy", DirectX },
  { "lda", DirectX },
  { "ldx", DirectY },
  { "lda", ILDirectY },

  { "clv", Implied },
  { "lda", AddressY },
  { "tsx", Implied },
  { "tyx", Implied },

  { "ldy", AddressX },
  { "lda", AddressX },
  { "ldx", AddressY },
  { "lda", LongX },

  //0xc0 - 0xcf
  { "cpy", IndexConstant },
  { "cmp", IDirectX },
  { "rep", Constant },
  { "cmp", Stack },

  { "cpy", Direct },
  { "cmp", Direct },
  { "dec", Direct },
  { "cmp", ILDirect },

  { "iny", Implied },
  { "cmp", AccumConstant },
  { "dex", Implied },
  { "wai", Implied },

  { "cpy", Address },
  { "cmp", Address },
  { "dec", Address },
  { "cmp", Long },

  //0xd0 - 0xdf
  { "bne", RelativeShort },
  { "cmp", IDirectY },
  { "cmp", IDirect },
  { "cmp", IStackY },

  { "pei", IDirect },
  { "cmp", DirectX },
  { "dec", DirectX },
  { "cmp", ILDirectY },

  { "cld", Implied },
  { "cmp", AddressY },
  { "phx", Implied },
  { "stp", Implied },

  { "jmp", ILAddress },
  { "cmp", AddressX },
  { "dec", AddressX },
  { "cmp", LongX },

  //0xe0 - 0xef
  { "cpx", IndexConstant },
  { "sbc", IDirectX },
  { "sep", Constant },
  { "sbc", Stack },

  { "cpx", Direct },
  { "sbc", Direct },
  { "inc", Direct },
  { "sbc", ILDirect },

  { "inx", Implied },
  { "sbc", AccumConstant },
  { "nop", Implied },
  { "xba", Implied },

  { "cpx", Address },
  { "sbc", Address },
  { "inc", Address },
  { "sbc", Long },

  //0xf0 - 0xff
  { "beq", RelativeShort },
  { "sbc", IDirectY },
  { "sbc", IDirect },
  { "sbc", IStackY },

  { "pea", Address },
  { "sbc", DirectX },
  { "inc", DirectX },
  { "sbc", ILDirectY },

  { "sed", Implied },
  { "sbc", AddressY },
  { "plx", Implied },
  { "xce", Implied },

  { "jsr", IAddressX },
  { "sbc", AddressX },
  { "inc", AddressX },
  { "sbc", LongX },
};

}

#endif
