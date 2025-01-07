#ifndef R0B44A_H
#define R0B44A_H

// Conditional compilation for section attribute based on compiler support
#ifdef __GNUC__  // Or another condition specific to your toolchain
    #define SECTION_ATTRIBUTE __attribute__((section("seg_pmda")))
#else
    #define SECTION_ATTRIBUTE
#endif

SECTION_ATTRIBUTE float R0B44A[R0B44A_SECT*4+ 2] =
{
  //Wspolczynnik skalowania:
   1.0000000e+000
,  1.0000000e+000
,  //Wspolczynniki filtru
 -9.9949714e-001
,  1.9993673e+000
,  9.9984001e-001
, -1.9997102e+000
}; 

#endif
