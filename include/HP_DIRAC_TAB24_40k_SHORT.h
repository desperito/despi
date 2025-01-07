#ifndef HP_DIRAC_TAB24_40K_SHORT_H
#define HP_DIRAC_TAB24_40K_SHORT_H

// Ensure proper alignment and declaration of the array
//alignas(16) int HP_DIRAC_TAB24_40k[] = {0, 0, 0, static_cast<int>(8.3886e+06)};

__attribute__((section("seg_sdram2")))
int HP_DIRAC_TAB24_40k[] = {0, 0, 0, static_cast<int>(8.3886e+06)};

// section ("seg_sdram2") int HP_DIRAC_TAB24_40k[] = {0,0,0,8.3886e+06};

#endif // HP_DIRAC_TAB24_40K_SHORT_H
