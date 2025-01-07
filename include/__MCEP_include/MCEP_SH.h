#ifndef MCEP_H
#define MCEP_H

// Conditional compilation for section attribute based on compiler support
#ifdef __GNUC__  // Or another condition specific to your toolchain
    #define SECTION_ATTRIBUTE __attribute__((section("seg_sdram2")))
#else
    #define SECTION_ATTRIBUTE
#endif


SECTION_ATTRIBUTE MCEP_T TME[MCEP_MAXTABSIZE] = {
    { "HLMN" 
    #include "./__MCEP_include/HLMN/MCB_HLMN.h"
    #include "./__MCEP_include/HLMN/MCT_HLMN.h"
    #include "./__MCEP_include/EPS_default.h"
    #include "./__MCEP_include/EPF_default.h"
    },
    { "H2S3_21_99" 
    #include "./__MCEP_include/HW4N/MCB_HW4N.h"
    #include "./__MCEP_include/HW4N/MCT_HW4N.h"
    #include "./__MCEP_include/HW4N/EPS_H2S3_21_99.h"
    #include "./__MCEP_include/HW4N/EPF_H2S3_21_99.h"
    }
};

#endif
