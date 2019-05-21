/********************************************************
 *
 * Structure for ext_data_fetch_event() filling.
 *
 * Do not edit - automatically generated.
 */

#ifndef __GUARD_H101_SOFTWIM_EXT_H101_SOFTWIM_H__
#define __GUARD_H101_SOFTWIM_EXT_H101_SOFTWIM_H__

#ifndef __CINT__
# include <stdint.h>
#else
/* For CINT (old version trouble with stdint.h): */
# ifndef uint32_t
typedef unsigned int uint32_t;
typedef          int  int32_t;
# endif
#endif
#ifndef EXT_STRUCT_CTRL
# define EXT_STRUCT_CTRL(x)
#endif

/********************************************************
 *
 * Plain structure (layout as ntuple/root file):
 */

typedef struct EXT_STR_h101_SOFTWIM_t
{
  /* RAW */
  uint32_t SOFTWIM_P1S1EM /* [1,16] */;
  uint32_t SOFTWIM_P1S1EMI[16 EXT_STRUCT_CTRL(SOFTWIM_P1S1EM)] /* [1,16] */;
  uint32_t SOFTWIM_P1S1EME[16 EXT_STRUCT_CTRL(SOFTWIM_P1S1EM)] /* [1,160] */;
  uint32_t SOFTWIM_P1S1E /* [0,160] */;
  uint32_t SOFTWIM_P1S1Ev[160 EXT_STRUCT_CTRL(SOFTWIM_P1S1E)] /* [0,65535] */;
  uint32_t SOFTWIM_P1S1TM /* [1,16] */;
  uint32_t SOFTWIM_P1S1TMI[16 EXT_STRUCT_CTRL(SOFTWIM_P1S1TM)] /* [1,16] */;
  uint32_t SOFTWIM_P1S1TME[16 EXT_STRUCT_CTRL(SOFTWIM_P1S1TM)] /* [1,160] */;
  uint32_t SOFTWIM_P1S1T /* [0,160] */;
  uint32_t SOFTWIM_P1S1Tv[160 EXT_STRUCT_CTRL(SOFTWIM_P1S1T)] /* [0,65535] */;
  uint32_t SOFTWIM_P1S1TREFM /* [1,1] */;
  uint32_t SOFTWIM_P1S1TREFMI[1 EXT_STRUCT_CTRL(SOFTWIM_P1S1TREFM)] /* [1,1] */;
  uint32_t SOFTWIM_P1S1TREFME[1 EXT_STRUCT_CTRL(SOFTWIM_P1S1TREFM)] /* [1,10] */;
  uint32_t SOFTWIM_P1S1TREF /* [0,10] */;
  uint32_t SOFTWIM_P1S1TREFv[10 EXT_STRUCT_CTRL(SOFTWIM_P1S1TREF)] /* [0,65535] */;
  uint32_t SOFTWIM_P1S2EM /* [1,16] */;
  uint32_t SOFTWIM_P1S2EMI[16 EXT_STRUCT_CTRL(SOFTWIM_P1S2EM)] /* [1,16] */;
  uint32_t SOFTWIM_P1S2EME[16 EXT_STRUCT_CTRL(SOFTWIM_P1S2EM)] /* [1,160] */;
  uint32_t SOFTWIM_P1S2E /* [0,160] */;
  uint32_t SOFTWIM_P1S2Ev[160 EXT_STRUCT_CTRL(SOFTWIM_P1S2E)] /* [0,65535] */;
  uint32_t SOFTWIM_P1S2TM /* [1,16] */;
  uint32_t SOFTWIM_P1S2TMI[16 EXT_STRUCT_CTRL(SOFTWIM_P1S2TM)] /* [1,16] */;
  uint32_t SOFTWIM_P1S2TME[16 EXT_STRUCT_CTRL(SOFTWIM_P1S2TM)] /* [1,160] */;
  uint32_t SOFTWIM_P1S2T /* [0,160] */;
  uint32_t SOFTWIM_P1S2Tv[160 EXT_STRUCT_CTRL(SOFTWIM_P1S2T)] /* [0,65535] */;
  uint32_t SOFTWIM_P1S2TREFM /* [1,1] */;
  uint32_t SOFTWIM_P1S2TREFMI[1 EXT_STRUCT_CTRL(SOFTWIM_P1S2TREFM)] /* [1,1] */;
  uint32_t SOFTWIM_P1S2TREFME[1 EXT_STRUCT_CTRL(SOFTWIM_P1S2TREFM)] /* [1,10] */;
  uint32_t SOFTWIM_P1S2TREF /* [0,10] */;
  uint32_t SOFTWIM_P1S2TREFv[10 EXT_STRUCT_CTRL(SOFTWIM_P1S2TREF)] /* [0,65535] */;
  uint32_t SOFTWIM_P2S1EM /* [1,16] */;
  uint32_t SOFTWIM_P2S1EMI[16 EXT_STRUCT_CTRL(SOFTWIM_P2S1EM)] /* [1,16] */;
  uint32_t SOFTWIM_P2S1EME[16 EXT_STRUCT_CTRL(SOFTWIM_P2S1EM)] /* [1,160] */;
  uint32_t SOFTWIM_P2S1E /* [0,160] */;
  uint32_t SOFTWIM_P2S1Ev[160 EXT_STRUCT_CTRL(SOFTWIM_P2S1E)] /* [0,65535] */;
  uint32_t SOFTWIM_P2S1TM /* [1,16] */;
  uint32_t SOFTWIM_P2S1TMI[16 EXT_STRUCT_CTRL(SOFTWIM_P2S1TM)] /* [1,16] */;
  uint32_t SOFTWIM_P2S1TME[16 EXT_STRUCT_CTRL(SOFTWIM_P2S1TM)] /* [1,160] */;
  uint32_t SOFTWIM_P2S1T /* [0,160] */;
  uint32_t SOFTWIM_P2S1Tv[160 EXT_STRUCT_CTRL(SOFTWIM_P2S1T)] /* [0,65535] */;
  uint32_t SOFTWIM_P2S1TREFM /* [1,1] */;
  uint32_t SOFTWIM_P2S1TREFMI[1 EXT_STRUCT_CTRL(SOFTWIM_P2S1TREFM)] /* [1,1] */;
  uint32_t SOFTWIM_P2S1TREFME[1 EXT_STRUCT_CTRL(SOFTWIM_P2S1TREFM)] /* [1,10] */;
  uint32_t SOFTWIM_P2S1TREF /* [0,10] */;
  uint32_t SOFTWIM_P2S1TREFv[10 EXT_STRUCT_CTRL(SOFTWIM_P2S1TREF)] /* [0,65535] */;
  uint32_t SOFTWIM_P2S2EM /* [1,16] */;
  uint32_t SOFTWIM_P2S2EMI[16 EXT_STRUCT_CTRL(SOFTWIM_P2S2EM)] /* [1,16] */;
  uint32_t SOFTWIM_P2S2EME[16 EXT_STRUCT_CTRL(SOFTWIM_P2S2EM)] /* [1,160] */;
  uint32_t SOFTWIM_P2S2E /* [0,160] */;
  uint32_t SOFTWIM_P2S2Ev[160 EXT_STRUCT_CTRL(SOFTWIM_P2S2E)] /* [0,65535] */;
  uint32_t SOFTWIM_P2S2TM /* [1,16] */;
  uint32_t SOFTWIM_P2S2TMI[16 EXT_STRUCT_CTRL(SOFTWIM_P2S2TM)] /* [1,16] */;
  uint32_t SOFTWIM_P2S2TME[16 EXT_STRUCT_CTRL(SOFTWIM_P2S2TM)] /* [1,160] */;
  uint32_t SOFTWIM_P2S2T /* [0,160] */;
  uint32_t SOFTWIM_P2S2Tv[160 EXT_STRUCT_CTRL(SOFTWIM_P2S2T)] /* [0,65535] */;
  uint32_t SOFTWIM_P2S2TREFM /* [1,1] */;
  uint32_t SOFTWIM_P2S2TREFMI[1 EXT_STRUCT_CTRL(SOFTWIM_P2S2TREFM)] /* [1,1] */;
  uint32_t SOFTWIM_P2S2TREFME[1 EXT_STRUCT_CTRL(SOFTWIM_P2S2TREFM)] /* [1,10] */;
  uint32_t SOFTWIM_P2S2TREF /* [0,10] */;
  uint32_t SOFTWIM_P2S2TREFv[10 EXT_STRUCT_CTRL(SOFTWIM_P2S2TREF)] /* [0,65535] */;

} EXT_STR_h101_SOFTWIM;

/********************************************************
 *
 * Structure with multiple levels of arrays (partially)
 * recovered (recommended):
 */

typedef struct EXT_STR_h101_SOFTWIM_onion_t
{
  /* RAW */
  struct {
    struct {
      uint32_t EM;
      uint32_t EMI[16 /* EM */];
      uint32_t EME[16 /* EM */];
      uint32_t E;
      uint32_t Ev[160 /* E */];
      uint32_t TM;
      uint32_t TMI[16 /* TM */];
      uint32_t TME[16 /* TM */];
      uint32_t T;
      uint32_t Tv[160 /* T */];
      uint32_t TREFM;
      uint32_t TREFMI[1 /* TREFM */];
      uint32_t TREFME[1 /* TREFM */];
      uint32_t TREF;
      uint32_t TREFv[10 /* TREF */];
    } S[2];
  } SOFTWIM_P[2];

} EXT_STR_h101_SOFTWIM_onion;

/*******************************************************/

#define EXT_STR_h101_SOFTWIM_ITEMS_INFO(ok,si,offset,struct_t,printerr) do { \
  ok = 1; \
  /* RAW */ \
  EXT_STR_ITEM_INFO_LIM(ok,si,offset,struct_t,printerr,\
                     SOFTWIM_P1S1EM,                  UINT32,\
                    "SOFTWIM_P1S1EM",16); \
  EXT_STR_ITEM_INFO_ZZP(ok,si,offset,struct_t,printerr,\
                     SOFTWIM_P1S1EMI,                 UINT32,\
                    "SOFTWIM_P1S1EMI",                 "SOFTWIM_P1S1EM"); \
  EXT_STR_ITEM_INFO_ZZP(ok,si,offset,struct_t,printerr,\
                     SOFTWIM_P1S1EME,                 UINT32,\
                    "SOFTWIM_P1S1EME",                 "SOFTWIM_P1S1EM"); \
  EXT_STR_ITEM_INFO_LIM(ok,si,offset,struct_t,printerr,\
                     SOFTWIM_P1S1E,                   UINT32,\
                    "SOFTWIM_P1S1E",160); \
  EXT_STR_ITEM_INFO_ZZP(ok,si,offset,struct_t,printerr,\
                     SOFTWIM_P1S1Ev,                  UINT32,\
                    "SOFTWIM_P1S1Ev",                  "SOFTWIM_P1S1E"); \
  EXT_STR_ITEM_INFO_LIM(ok,si,offset,struct_t,printerr,\
                     SOFTWIM_P1S1TM,                  UINT32,\
                    "SOFTWIM_P1S1TM",16); \
  EXT_STR_ITEM_INFO_ZZP(ok,si,offset,struct_t,printerr,\
                     SOFTWIM_P1S1TMI,                 UINT32,\
                    "SOFTWIM_P1S1TMI",                 "SOFTWIM_P1S1TM"); \
  EXT_STR_ITEM_INFO_ZZP(ok,si,offset,struct_t,printerr,\
                     SOFTWIM_P1S1TME,                 UINT32,\
                    "SOFTWIM_P1S1TME",                 "SOFTWIM_P1S1TM"); \
  EXT_STR_ITEM_INFO_LIM(ok,si,offset,struct_t,printerr,\
                     SOFTWIM_P1S1T,                   UINT32,\
                    "SOFTWIM_P1S1T",160); \
  EXT_STR_ITEM_INFO_ZZP(ok,si,offset,struct_t,printerr,\
                     SOFTWIM_P1S1Tv,                  UINT32,\
                    "SOFTWIM_P1S1Tv",                  "SOFTWIM_P1S1T"); \
  EXT_STR_ITEM_INFO_LIM(ok,si,offset,struct_t,printerr,\
                     SOFTWIM_P1S1TREFM,               UINT32,\
                    "SOFTWIM_P1S1TREFM",1); \
  EXT_STR_ITEM_INFO_ZZP(ok,si,offset,struct_t,printerr,\
                     SOFTWIM_P1S1TREFMI,              UINT32,\
                    "SOFTWIM_P1S1TREFMI",              "SOFTWIM_P1S1TREFM"); \
  EXT_STR_ITEM_INFO_ZZP(ok,si,offset,struct_t,printerr,\
                     SOFTWIM_P1S1TREFME,              UINT32,\
                    "SOFTWIM_P1S1TREFME",              "SOFTWIM_P1S1TREFM"); \
  EXT_STR_ITEM_INFO_LIM(ok,si,offset,struct_t,printerr,\
                     SOFTWIM_P1S1TREF,                UINT32,\
                    "SOFTWIM_P1S1TREF",10); \
  EXT_STR_ITEM_INFO_ZZP(ok,si,offset,struct_t,printerr,\
                     SOFTWIM_P1S1TREFv,               UINT32,\
                    "SOFTWIM_P1S1TREFv",               "SOFTWIM_P1S1TREF"); \
  EXT_STR_ITEM_INFO_LIM(ok,si,offset,struct_t,printerr,\
                     SOFTWIM_P1S2EM,                  UINT32,\
                    "SOFTWIM_P1S2EM",16); \
  EXT_STR_ITEM_INFO_ZZP(ok,si,offset,struct_t,printerr,\
                     SOFTWIM_P1S2EMI,                 UINT32,\
                    "SOFTWIM_P1S2EMI",                 "SOFTWIM_P1S2EM"); \
  EXT_STR_ITEM_INFO_ZZP(ok,si,offset,struct_t,printerr,\
                     SOFTWIM_P1S2EME,                 UINT32,\
                    "SOFTWIM_P1S2EME",                 "SOFTWIM_P1S2EM"); \
  EXT_STR_ITEM_INFO_LIM(ok,si,offset,struct_t,printerr,\
                     SOFTWIM_P1S2E,                   UINT32,\
                    "SOFTWIM_P1S2E",160); \
  EXT_STR_ITEM_INFO_ZZP(ok,si,offset,struct_t,printerr,\
                     SOFTWIM_P1S2Ev,                  UINT32,\
                    "SOFTWIM_P1S2Ev",                  "SOFTWIM_P1S2E"); \
  EXT_STR_ITEM_INFO_LIM(ok,si,offset,struct_t,printerr,\
                     SOFTWIM_P1S2TM,                  UINT32,\
                    "SOFTWIM_P1S2TM",16); \
  EXT_STR_ITEM_INFO_ZZP(ok,si,offset,struct_t,printerr,\
                     SOFTWIM_P1S2TMI,                 UINT32,\
                    "SOFTWIM_P1S2TMI",                 "SOFTWIM_P1S2TM"); \
  EXT_STR_ITEM_INFO_ZZP(ok,si,offset,struct_t,printerr,\
                     SOFTWIM_P1S2TME,                 UINT32,\
                    "SOFTWIM_P1S2TME",                 "SOFTWIM_P1S2TM"); \
  EXT_STR_ITEM_INFO_LIM(ok,si,offset,struct_t,printerr,\
                     SOFTWIM_P1S2T,                   UINT32,\
                    "SOFTWIM_P1S2T",160); \
  EXT_STR_ITEM_INFO_ZZP(ok,si,offset,struct_t,printerr,\
                     SOFTWIM_P1S2Tv,                  UINT32,\
                    "SOFTWIM_P1S2Tv",                  "SOFTWIM_P1S2T"); \
  EXT_STR_ITEM_INFO_LIM(ok,si,offset,struct_t,printerr,\
                     SOFTWIM_P1S2TREFM,               UINT32,\
                    "SOFTWIM_P1S2TREFM",1); \
  EXT_STR_ITEM_INFO_ZZP(ok,si,offset,struct_t,printerr,\
                     SOFTWIM_P1S2TREFMI,              UINT32,\
                    "SOFTWIM_P1S2TREFMI",              "SOFTWIM_P1S2TREFM"); \
  EXT_STR_ITEM_INFO_ZZP(ok,si,offset,struct_t,printerr,\
                     SOFTWIM_P1S2TREFME,              UINT32,\
                    "SOFTWIM_P1S2TREFME",              "SOFTWIM_P1S2TREFM"); \
  EXT_STR_ITEM_INFO_LIM(ok,si,offset,struct_t,printerr,\
                     SOFTWIM_P1S2TREF,                UINT32,\
                    "SOFTWIM_P1S2TREF",10); \
  EXT_STR_ITEM_INFO_ZZP(ok,si,offset,struct_t,printerr,\
                     SOFTWIM_P1S2TREFv,               UINT32,\
                    "SOFTWIM_P1S2TREFv",               "SOFTWIM_P1S2TREF"); \
  EXT_STR_ITEM_INFO_LIM(ok,si,offset,struct_t,printerr,\
                     SOFTWIM_P2S1EM,                  UINT32,\
                    "SOFTWIM_P2S1EM",16); \
  EXT_STR_ITEM_INFO_ZZP(ok,si,offset,struct_t,printerr,\
                     SOFTWIM_P2S1EMI,                 UINT32,\
                    "SOFTWIM_P2S1EMI",                 "SOFTWIM_P2S1EM"); \
  EXT_STR_ITEM_INFO_ZZP(ok,si,offset,struct_t,printerr,\
                     SOFTWIM_P2S1EME,                 UINT32,\
                    "SOFTWIM_P2S1EME",                 "SOFTWIM_P2S1EM"); \
  EXT_STR_ITEM_INFO_LIM(ok,si,offset,struct_t,printerr,\
                     SOFTWIM_P2S1E,                   UINT32,\
                    "SOFTWIM_P2S1E",160); \
  EXT_STR_ITEM_INFO_ZZP(ok,si,offset,struct_t,printerr,\
                     SOFTWIM_P2S1Ev,                  UINT32,\
                    "SOFTWIM_P2S1Ev",                  "SOFTWIM_P2S1E"); \
  EXT_STR_ITEM_INFO_LIM(ok,si,offset,struct_t,printerr,\
                     SOFTWIM_P2S1TM,                  UINT32,\
                    "SOFTWIM_P2S1TM",16); \
  EXT_STR_ITEM_INFO_ZZP(ok,si,offset,struct_t,printerr,\
                     SOFTWIM_P2S1TMI,                 UINT32,\
                    "SOFTWIM_P2S1TMI",                 "SOFTWIM_P2S1TM"); \
  EXT_STR_ITEM_INFO_ZZP(ok,si,offset,struct_t,printerr,\
                     SOFTWIM_P2S1TME,                 UINT32,\
                    "SOFTWIM_P2S1TME",                 "SOFTWIM_P2S1TM"); \
  EXT_STR_ITEM_INFO_LIM(ok,si,offset,struct_t,printerr,\
                     SOFTWIM_P2S1T,                   UINT32,\
                    "SOFTWIM_P2S1T",160); \
  EXT_STR_ITEM_INFO_ZZP(ok,si,offset,struct_t,printerr,\
                     SOFTWIM_P2S1Tv,                  UINT32,\
                    "SOFTWIM_P2S1Tv",                  "SOFTWIM_P2S1T"); \
  EXT_STR_ITEM_INFO_LIM(ok,si,offset,struct_t,printerr,\
                     SOFTWIM_P2S1TREFM,               UINT32,\
                    "SOFTWIM_P2S1TREFM",1); \
  EXT_STR_ITEM_INFO_ZZP(ok,si,offset,struct_t,printerr,\
                     SOFTWIM_P2S1TREFMI,              UINT32,\
                    "SOFTWIM_P2S1TREFMI",              "SOFTWIM_P2S1TREFM"); \
  EXT_STR_ITEM_INFO_ZZP(ok,si,offset,struct_t,printerr,\
                     SOFTWIM_P2S1TREFME,              UINT32,\
                    "SOFTWIM_P2S1TREFME",              "SOFTWIM_P2S1TREFM"); \
  EXT_STR_ITEM_INFO_LIM(ok,si,offset,struct_t,printerr,\
                     SOFTWIM_P2S1TREF,                UINT32,\
                    "SOFTWIM_P2S1TREF",10); \
  EXT_STR_ITEM_INFO_ZZP(ok,si,offset,struct_t,printerr,\
                     SOFTWIM_P2S1TREFv,               UINT32,\
                    "SOFTWIM_P2S1TREFv",               "SOFTWIM_P2S1TREF"); \
  EXT_STR_ITEM_INFO_LIM(ok,si,offset,struct_t,printerr,\
                     SOFTWIM_P2S2EM,                  UINT32,\
                    "SOFTWIM_P2S2EM",16); \
  EXT_STR_ITEM_INFO_ZZP(ok,si,offset,struct_t,printerr,\
                     SOFTWIM_P2S2EMI,                 UINT32,\
                    "SOFTWIM_P2S2EMI",                 "SOFTWIM_P2S2EM"); \
  EXT_STR_ITEM_INFO_ZZP(ok,si,offset,struct_t,printerr,\
                     SOFTWIM_P2S2EME,                 UINT32,\
                    "SOFTWIM_P2S2EME",                 "SOFTWIM_P2S2EM"); \
  EXT_STR_ITEM_INFO_LIM(ok,si,offset,struct_t,printerr,\
                     SOFTWIM_P2S2E,                   UINT32,\
                    "SOFTWIM_P2S2E",160); \
  EXT_STR_ITEM_INFO_ZZP(ok,si,offset,struct_t,printerr,\
                     SOFTWIM_P2S2Ev,                  UINT32,\
                    "SOFTWIM_P2S2Ev",                  "SOFTWIM_P2S2E"); \
  EXT_STR_ITEM_INFO_LIM(ok,si,offset,struct_t,printerr,\
                     SOFTWIM_P2S2TM,                  UINT32,\
                    "SOFTWIM_P2S2TM",16); \
  EXT_STR_ITEM_INFO_ZZP(ok,si,offset,struct_t,printerr,\
                     SOFTWIM_P2S2TMI,                 UINT32,\
                    "SOFTWIM_P2S2TMI",                 "SOFTWIM_P2S2TM"); \
  EXT_STR_ITEM_INFO_ZZP(ok,si,offset,struct_t,printerr,\
                     SOFTWIM_P2S2TME,                 UINT32,\
                    "SOFTWIM_P2S2TME",                 "SOFTWIM_P2S2TM"); \
  EXT_STR_ITEM_INFO_LIM(ok,si,offset,struct_t,printerr,\
                     SOFTWIM_P2S2T,                   UINT32,\
                    "SOFTWIM_P2S2T",160); \
  EXT_STR_ITEM_INFO_ZZP(ok,si,offset,struct_t,printerr,\
                     SOFTWIM_P2S2Tv,                  UINT32,\
                    "SOFTWIM_P2S2Tv",                  "SOFTWIM_P2S2T"); \
  EXT_STR_ITEM_INFO_LIM(ok,si,offset,struct_t,printerr,\
                     SOFTWIM_P2S2TREFM,               UINT32,\
                    "SOFTWIM_P2S2TREFM",1); \
  EXT_STR_ITEM_INFO_ZZP(ok,si,offset,struct_t,printerr,\
                     SOFTWIM_P2S2TREFMI,              UINT32,\
                    "SOFTWIM_P2S2TREFMI",              "SOFTWIM_P2S2TREFM"); \
  EXT_STR_ITEM_INFO_ZZP(ok,si,offset,struct_t,printerr,\
                     SOFTWIM_P2S2TREFME,              UINT32,\
                    "SOFTWIM_P2S2TREFME",              "SOFTWIM_P2S2TREFM"); \
  EXT_STR_ITEM_INFO_LIM(ok,si,offset,struct_t,printerr,\
                     SOFTWIM_P2S2TREF,                UINT32,\
                    "SOFTWIM_P2S2TREF",10); \
  EXT_STR_ITEM_INFO_ZZP(ok,si,offset,struct_t,printerr,\
                     SOFTWIM_P2S2TREFv,               UINT32,\
                    "SOFTWIM_P2S2TREFv",               "SOFTWIM_P2S2TREF"); \
  \
} while (0);


#endif/*__GUARD_H101_SOFTWIM_EXT_H101_SOFTWIM_H__*/

/*******************************************************/
