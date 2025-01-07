#ifndef DYB96B_H
#define DYB96B_H

// Conditional compilation for section attribute based on compiler support
#ifdef __GNUC__  // Or another condition specific to your toolchain
    #define SECTION_ATTRIBUTE __attribute__((section("seg_pmda")))
#else
    #define SECTION_ATTRIBUTE
#endif


SECTION_ATTRIBUTE float DYB96B[DIRACx1_SECT*4+ 2] =
{
  //Wspolczynnik skalowania:
 1.000000000000000e+00
,1.000000000000000e+00
,  //Wspolczynniki filtru
-0.000000000000000e+00
,-0.000000000000000e+00
,0.000000000000000e+00
,   0.0000000e+00
}; 

#endif
