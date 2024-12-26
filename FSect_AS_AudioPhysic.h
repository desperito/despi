
#if !defined(MCP44AS_SECT) && !defined(MCP44AS_TAPS)
    #define  MCP44AS_SECT      58
    section  ("seg_dmda2")    float dm MCP44AS_L[MCP44AS_SECT*2+ 1];
    section  ("seg_dmda2")    float dm MCP44AS_R[MCP44AS_SECT*2+ 1];
  #include    "MCP44AS_B117_len3756.h"
#endif

#if !defined(MCBx44AS_SECT) && !defined(MCBx44AS_TAPS)
    #define  MCBx44AS_SECT      63
    section  ("seg_dmda2")    float dm MCBx44AS_L[MCBx44AS_SECT*2+ 1];
    section  ("seg_dmda2")    float dm MCBx44AS_R[MCBx44AS_SECT*2+ 1];
  #include    "MCBx44AS_B127_len14040.h"
#endif

#if !defined(MCBx48AS_SECT) && !defined(MCBx48AS_TAPS)
    #define  MCBx48AS_SECT      80
    section  ("seg_dmda2")    float dm MCBx48AS_L[MCBx48AS_SECT*2+ 1];
    section  ("seg_dmda2")    float dm MCBx48AS_R[MCBx48AS_SECT*2+ 1];
  #include    "MCBx48AS_B161_len16308.h"
#endif

#if !defined(MCP48AS_SECT) && !defined(MCP48AS_TAPS)
    #define  MCP48AS_SECT      75
    section  ("seg_dmda2")    float dm MCP48AS_L[MCP48AS_SECT*2+ 1];
    section  ("seg_dmda2")    float dm MCP48AS_R[MCP48AS_SECT*2+ 1];
  #include    "MCP48AS_B151_len12874.h"
#endif

#if !defined(EPP48AS_SECT) && !defined(EPP48AS_TAPS)
    #define  EPP48AS_SECT      7
    section  ("seg_dmda2")    float dm EPP48AS_L[EPP48AS_SECT*2+ 1];
    section  ("seg_dmda2")    float dm EPP48AS_R[EPP48AS_SECT*2+ 1];
  #include    "EPP48AS_Off1_B14_len3198.h"
#endif

#if !defined(EPP44AS_SECT) && !defined(EPP44AS_TAPS)
    #define  EPP44AS_SECT      5
    section  ("seg_dmda2")    float dm EPP44AS_L[EPP44AS_SECT*2+ 1];
    section  ("seg_dmda2")    float dm EPP44AS_R[EPP44AS_SECT*2+ 1];
  #include    "EPP44AS_Off2_B10_len10560.h"
#endif
