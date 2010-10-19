struct DMA {
  enum CDEN { DmaNormal = 0, DmaCharConversion = 1 };
  enum SD { SourceROM = 0, SourceBWRAM = 1, SourceIRAM = 2 };
  enum DD { DestIRAM = 0, DestBWRAM = 1 };
  uint64_t line;
} dma;

void dma_normal();
void dma_cc1();
uint8 dma_cc1_read(uint64_t addr);
void dma_cc2();
