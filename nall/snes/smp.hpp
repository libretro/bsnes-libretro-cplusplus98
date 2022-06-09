#ifndef NALL_SNES_SMP_HPP
#define NALL_SNES_SMP_HPP

namespace nall {

struct SNESSMP {
  enum : unsigned {
    Implied,          //
    TVector,          //0
    Direct,           //$00
    DirectRelative,   //$00,+/-$00
    ADirect,          //a,$00
    AAbsolute,        //a,$0000
    AIX,              //a,(x)
    AIDirectX,        //a,($00+x)
    AConstant,        //a,#$00
    DirectDirect,     //$00,$00
    CAbsoluteBit,     //c,$0000:0
    Absolute,         //$0000
    P,                //p
    AbsoluteA,        //$0000,a
    Relative,         //+/-$00
    ADirectX,         //a,$00+x
    AAbsoluteX,       //a,$0000+x
    AAbsoluteY,       //a,$0000+y
    AIDirectY,        //a,($00)+y
    DirectConstant,   //$00,#$00
    IXIY,             //(x),(y)
    DirectX,          //$00+x
    A,                //a
    X,                //x
    XAbsolute,        //x,$0000
    IAbsoluteX,       //($0000+x)
    CNAbsoluteBit,    //c,!$0000:0
    XDirect,          //x,$00
    PVector,          //$ff00
    YaDirect,         //ya,$00
    XA,               //x,a
    YAbsolute,        //y,$0000
    Y,                //y
    AX,               //a,x
    YDirect,          //y,$00
    YConstant,        //y,#$00
    XSp,              //x,sp
    YaX,              //ya,x
    IXPA,             //(x)+,a
    SpX,              //sp,x
    AIXP,             //a,(x)+
    DirectA,          //$00,a
    IXA,              //(x),a
    IDirectXA,        //($00+x),a
    XConstant,        //x,#$00
    AbsoluteX,        //$0000,x
    AbsoluteBitC,     //$0000:0,c
    DirectY,          //$00,y
    AbsoluteY,        //$0000,y
    Ya,               //ya
    DirectXA,         //$00+x,a
    AbsoluteXA,       //$0000+x,a
    AbsoluteYA,       //$0000+y,a
    IDirectYA,        //($00)+y,a
    DirectYX,         //$00+y,x
    DirectYa,         //$00,ya
    DirectXY,         //$00+x,y
    AY,               //a,y
    DirectXRelative,  //$00+x,+/-$00
    XDirectY,         //x,$00+y
    YDirectX,         //y,$00+x
    YA,               //y,a
    YRelative,        //y,+/-$00
  };

  struct OpcodeInfo {
    char name[6];
    unsigned mode;
  };

  static const OpcodeInfo opcodeInfo[256];
};

const SNESSMP::OpcodeInfo SNESSMP::opcodeInfo[256] = {
  //0x00 - 0x0f
  { "nop  ", Implied },
  { "tcall", TVector },
  { "set0 ", Direct },
  { "bbs0 ", DirectRelative },

  { "or   ", ADirect },
  { "or   ", AAbsolute },
  { "or   ", AIX },
  { "or   ", AIDirectX },

  { "or   ", AConstant },
  { "or   ", DirectDirect },
  { "or1  ", CAbsoluteBit },
  { "asl  ", Direct },

  { "asl  ", Absolute },
  { "push ", P },
  { "tset ", AbsoluteA },
  { "brk  ", Implied },

  //0x10 - 0x1f
  { "bpl  ", Relative },
  { "tcall", TVector },
  { "clr0 ", Direct },
  { "bbc0 ", DirectRelative },

  { "or   ", ADirectX },
  { "or   ", AAbsoluteX },
  { "or   ", AAbsoluteY },
  { "or   ", AIDirectY },

  { "or   ", DirectConstant },
  { "or   ", IXIY },
  { "decw ", Direct },
  { "asl  ", DirectX },

  { "asl  ", A },
  { "dec  ", X },
  { "cmp  ", XAbsolute },
  { "jmp  ", IAbsoluteX },

  //0x20 - 0x2f
  { "clrp ", Implied },
  { "tcall", TVector },
  { "set1 ", Direct },
  { "bbs1 ", DirectRelative },

  { "and  ", ADirect },
  { "and  ", AAbsolute },
  { "and  ", AIX },
  { "and  ", AIDirectX },

  { "and  ", AConstant },
  { "and  ", DirectDirect },
  { "or1  ", CNAbsoluteBit },
  { "rol  ", Direct },

  { "rol  ", Absolute },
  { "push ", A },
  { "cbne ", DirectRelative },
  { "bra  ", Relative },

  //0x30 - 0x3f
  { "bmi  ", Relative },
  { "tcall", TVector },
  { "clr1 ", Direct },
  { "bbc1 ", DirectRelative },

  { "and  ", ADirectX },
  { "and  ", AAbsoluteX },
  { "and  ", AAbsoluteY },
  { "and  ", AIDirectY },

  { "and  ", DirectConstant },
  { "and  ", IXIY },
  { "incw ", Direct },
  { "rol  ", DirectX },

  { "rol  ", A },
  { "inc  ", X },
  { "cmp  ", XDirect },
  { "call ", Absolute },

  //0x40 - 0x4f
  { "setp ", Implied },
  { "tcall", TVector },
  { "set2 ", Direct },
  { "bbs2 ", DirectRelative },

  { "eor  ", ADirect },
  { "eor  ", AAbsolute },
  { "eor  ", AIX },
  { "eor  ", AIDirectX },

  { "eor  ", AConstant },
  { "eor  ", DirectDirect },
  { "and1 ", CAbsoluteBit },
  { "lsr  ", Direct },

  { "lsr  ", Absolute },
  { "push ", X },
  { "tclr ", AbsoluteA },
  { "pcall", PVector },

  //0x50 - 0x5f
  { "bvc  ", Relative },
  { "tcall", TVector },
  { "clr2 ", Direct },
  { "bbc2 ", DirectRelative },

  { "eor  ", ADirectX },
  { "eor  ", AAbsoluteX },
  { "eor  ", AAbsoluteY },
  { "eor  ", AIDirectY },

  { "eor  ", DirectConstant },
  { "eor  ", IXIY },
  { "cmpw ", YaDirect },
  { "lsr  ", DirectX },

  { "lsr  ", A },
  { "mov  ", XA },
  { "cmp  ", YAbsolute },
  { "jmp  ", Absolute },

  //0x60 - 0x6f
  { "clrc ", Implied },
  { "tcall", TVector },
  { "set3 ", Direct },
  { "bbs3 ", DirectRelative },

  { "cmp  ", ADirect },
  { "cmp  ", AAbsolute },
  { "cmp  ", AIX },
  { "cmp  ", AIDirectX },

  { "cmp  ", AConstant },
  { "cmp  ", DirectDirect },
  { "and1 ", CNAbsoluteBit },
  { "ror  ", Direct },

  { "ror  ", Absolute },
  { "push ", Y },
  { "dbnz ", DirectRelative },
  { "ret  ", Implied },

  //0x70 - 0x7f
  { "bvs  ", Relative },
  { "tcall", TVector },
  { "clr3 ", Direct },
  { "bbc3 ", DirectRelative },

  { "cmp  ", ADirectX },
  { "cmp  ", AAbsoluteX },
  { "cmp  ", AAbsoluteY },
  { "cmp  ", AIDirectY },

  { "cmp  ", DirectConstant },
  { "cmp  ", IXIY },
  { "addw ", YaDirect },
  { "ror  ", DirectX },

  { "ror  ", A },
  { "mov  ", AX },
  { "cmp  ", YDirect },
  { "reti ", Implied },

  //0x80 - 0x8f
  { "setc ", Implied },
  { "tcall", TVector },
  { "set4 ", Direct },
  { "bbs4 ", DirectRelative },

  { "adc  ", ADirect },
  { "adc  ", AAbsolute },
  { "adc  ", AIX },
  { "adc  ", AIDirectX },

  { "adc  ", AConstant },
  { "adc  ", DirectDirect },
  { "eor1 ", CAbsoluteBit },
  { "dec  ", Direct },

  { "dec  ", Absolute },
  { "mov  ", YConstant },
  { "pop  ", P },
  { "mov  ", DirectConstant },

  //0x90 - 0x9f
  { "bcc  ", Relative },
  { "tcall", TVector },
  { "clr4 ", Direct },
  { "bbc4 ", DirectRelative },

  { "adc  ", ADirectX },
  { "adc  ", AAbsoluteX },
  { "adc  ", AAbsoluteY },
  { "adc  ", AIDirectY },

  { "adc  ", DirectRelative },
  { "adc  ", IXIY },
  { "subw ", YaDirect },
  { "dec  ", DirectX },

  { "dec  ", A },
  { "mov  ", XSp },
  { "div  ", YaX },
  { "xcn  ", A },

  //0xa0 - 0xaf
  { "ei   ", Implied },
  { "tcall", TVector },
  { "set5 ", Direct },
  { "bbs5 ", DirectRelative },

  { "sbc  ", ADirect },
  { "sbc  ", AAbsolute },
  { "sbc  ", AIX },
  { "sbc  ", AIDirectX },

  { "sbc  ", AConstant },
  { "sbc  ", DirectDirect },
  { "mov1 ", CAbsoluteBit },
  { "inc  ", Direct },

  { "inc  ", Absolute },
  { "cmp  ", YConstant },
  { "pop  ", A },
  { "mov  ", IXPA },

  //0xb0 - 0xbf
  { "bcs  ", Relative },
  { "tcall", TVector },
  { "clr5 ", Direct },
  { "bbc5 ", DirectRelative },

  { "sbc  ", ADirectX },
  { "sbc  ", AAbsoluteX },
  { "sbc  ", AAbsoluteY },
  { "sbc  ", AIDirectY },

  { "sbc  ", DirectConstant },
  { "sbc  ", IXIY },
  { "movw ", YaDirect },
  { "inc  ", DirectX },

  { "inc  ", A },
  { "mov  ", SpX },
  { "das  ", A },
  { "mov  ", AIXP },

  //0xc0 - 0xcf
  { "di   ", Implied },
  { "tcall", TVector },
  { "set6 ", Direct },
  { "bbs6 ", DirectRelative },

  { "mov  ", DirectA },
  { "mov  ", AbsoluteA },
  { "mov  ", IXA },
  { "mov  ", IDirectXA },

  { "cmp  ", XConstant },
  { "mov  ", AbsoluteX },
  { "mov1 ", AbsoluteBitC },
  { "mov  ", DirectY },

  { "mov  ", AbsoluteY },
  { "mov  ", XConstant },
  { "pop  ", X },
  { "mul  ", Ya },

  //0xd0 - 0xdf
  { "bne  ", Relative },
  { "tcall", TVector },
  { "clr6 ", Relative },
  { "bbc6 ", DirectRelative },

  { "mov  ", DirectXA },
  { "mov  ", AbsoluteXA },
  { "mov  ", AbsoluteYA },
  { "mov  ", IDirectYA },

  { "mov  ", DirectX },
  { "mov  ", DirectYX },
  { "movw ", DirectYa },
  { "mov  ", DirectXY },

  { "dec  ", Y },
  { "mov  ", AY },
  { "cbne ", DirectXRelative },
  { "daa  ", A },

  //0xe0 - 0xef
  { "clrv ", Implied },
  { "tcall", TVector },
  { "set7 ", Direct },
  { "bbs7 ", DirectRelative },

  { "mov  ", ADirect },
  { "mov  ", AAbsolute },
  { "mov  ", AIX },
  { "mov  ", AIDirectX },

  { "mov  ", AConstant },
  { "mov  ", XAbsolute },
  { "not1 ", CAbsoluteBit },
  { "mov  ", YDirect },

  { "mov  ", YAbsolute },
  { "notc ", Implied },
  { "pop  ", Y },
  { "sleep", Implied },

  //0xf0 - 0xff
  { "beq  ", Relative },
  { "tcall", TVector },
  { "clr7 ", Direct },
  { "bbc7 ", DirectRelative },

  { "mov  ", ADirectX },
  { "mov  ", AAbsoluteX },
  { "mov  ", AAbsoluteY },
  { "mov  ", AIDirectY },

  { "mov  ", XDirect },
  { "mov  ", XDirectY },
  { "mov  ", DirectDirect },
  { "mov  ", YDirectX },

  { "inc  ", Y },
  { "mov  ", YA },
  { "dbz  ", YRelative },
  { "stop ", Implied },
};

}

#endif
