/********************************************************
 *
 * Structure for ext_data_fetch_event() filling.
 *
 * Do not edit - automatically generated.
 */

#ifndef __GUARD_H101_SOFTRIM_EXT_H101_SOFTRIM_H__
#define __GUARD_H101_SOFTRIM_EXT_H101_SOFTRIM_H__

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

typedef struct EXT_STR_h101_SOFTRIM_t
{
  /* RAW */
  uint32_t SOFTRIM_S1EM /* [1,6] */;
  uint32_t SOFTRIM_S1EMI[6 EXT_STRUCT_CTRL(SOFTRIM_S1EM)] /* [1,6] */;
  uint32_t SOFTRIM_S1EME[6 EXT_STRUCT_CTRL(SOFTRIM_S1EM)] /* [1,600] */;
  uint32_t SOFTRIM_S1E /* [0,600] */;
  uint32_t SOFTRIM_S1Ev[600 EXT_STRUCT_CTRL(SOFTRIM_S1E)] /* [0,1048575] */;
  uint32_t SOFTRIM_S1TM /* [1,6] */;
  uint32_t SOFTRIM_S1TMI[6 EXT_STRUCT_CTRL(SOFTRIM_S1TM)] /* [1,6] */;
  uint32_t SOFTRIM_S1TME[6 EXT_STRUCT_CTRL(SOFTRIM_S1TM)] /* [1,600] */;
  uint32_t SOFTRIM_S1T /* [0,600] */;
  uint32_t SOFTRIM_S1Tv[600 EXT_STRUCT_CTRL(SOFTRIM_S1T)] /* [0,1048575] */;
  uint32_t SOFTRIM_S1TREFM /* [1,1] */;
  uint32_t SOFTRIM_S1TREFMI[1 EXT_STRUCT_CTRL(SOFTRIM_S1TREFM)] /* [1,1] */;
  uint32_t SOFTRIM_S1TREFME[1 EXT_STRUCT_CTRL(SOFTRIM_S1TREFM)] /* [1,100] */;
  uint32_t SOFTRIM_S1TREF /* [0,100] */;
  uint32_t SOFTRIM_S1TREFv[100 EXT_STRUCT_CTRL(SOFTRIM_S1TREF)] /* [0,1048575] */;
  uint32_t SOFTRIM_S1TTRIGM /* [1,1] */;
  uint32_t SOFTRIM_S1TTRIGMI[1 EXT_STRUCT_CTRL(SOFTRIM_S1TTRIGM)] /* [1,1] */;
  uint32_t SOFTRIM_S1TTRIGME[1 EXT_STRUCT_CTRL(SOFTRIM_S1TTRIGM)] /* [1,100] */;
  uint32_t SOFTRIM_S1TTRIG /* [0,100] */;
  uint32_t SOFTRIM_S1TTRIGv[100 EXT_STRUCT_CTRL(SOFTRIM_S1TTRIG)] /* [0,1048575] */;
  uint32_t SOFTRIM_S2EM /* [1,6] */;
  uint32_t SOFTRIM_S2EMI[6 EXT_STRUCT_CTRL(SOFTRIM_S2EM)] /* [1,6] */;
  uint32_t SOFTRIM_S2EME[6 EXT_STRUCT_CTRL(SOFTRIM_S2EM)] /* [1,600] */;
  uint32_t SOFTRIM_S2E /* [0,600] */;
  uint32_t SOFTRIM_S2Ev[600 EXT_STRUCT_CTRL(SOFTRIM_S2E)] /* [0,1048575] */;
  uint32_t SOFTRIM_S2TM /* [1,6] */;
  uint32_t SOFTRIM_S2TMI[6 EXT_STRUCT_CTRL(SOFTRIM_S2TM)] /* [1,6] */;
  uint32_t SOFTRIM_S2TME[6 EXT_STRUCT_CTRL(SOFTRIM_S2TM)] /* [1,600] */;
  uint32_t SOFTRIM_S2T /* [0,600] */;
  uint32_t SOFTRIM_S2Tv[600 EXT_STRUCT_CTRL(SOFTRIM_S2T)] /* [0,1048575] */;
  uint32_t SOFTRIM_S2TREFM /* [1,1] */;
  uint32_t SOFTRIM_S2TREFMI[1 EXT_STRUCT_CTRL(SOFTRIM_S2TREFM)] /* [1,1] */;
  uint32_t SOFTRIM_S2TREFME[1 EXT_STRUCT_CTRL(SOFTRIM_S2TREFM)] /* [1,100] */;
  uint32_t SOFTRIM_S2TREF /* [0,100] */;
  uint32_t SOFTRIM_S2TREFv[100 EXT_STRUCT_CTRL(SOFTRIM_S2TREF)] /* [0,1048575] */;
  uint32_t SOFTRIM_S2TTRIGM /* [1,1] */;
  uint32_t SOFTRIM_S2TTRIGMI[1 EXT_STRUCT_CTRL(SOFTRIM_S2TTRIGM)] /* [1,1] */;
  uint32_t SOFTRIM_S2TTRIGME[1 EXT_STRUCT_CTRL(SOFTRIM_S2TTRIGM)] /* [1,100] */;
  uint32_t SOFTRIM_S2TTRIG /* [0,100] */;
  uint32_t SOFTRIM_S2TTRIGv[100 EXT_STRUCT_CTRL(SOFTRIM_S2TTRIG)] /* [0,1048575] */;
  uint32_t SOFTRIM_S3EM /* [1,6] */;
  uint32_t SOFTRIM_S3EMI[6 EXT_STRUCT_CTRL(SOFTRIM_S3EM)] /* [1,6] */;
  uint32_t SOFTRIM_S3EME[6 EXT_STRUCT_CTRL(SOFTRIM_S3EM)] /* [1,600] */;
  uint32_t SOFTRIM_S3E /* [0,600] */;
  uint32_t SOFTRIM_S3Ev[600 EXT_STRUCT_CTRL(SOFTRIM_S3E)] /* [0,1048575] */;
  uint32_t SOFTRIM_S3TM /* [1,6] */;
  uint32_t SOFTRIM_S3TMI[6 EXT_STRUCT_CTRL(SOFTRIM_S3TM)] /* [1,6] */;
  uint32_t SOFTRIM_S3TME[6 EXT_STRUCT_CTRL(SOFTRIM_S3TM)] /* [1,600] */;
  uint32_t SOFTRIM_S3T /* [0,600] */;
  uint32_t SOFTRIM_S3Tv[600 EXT_STRUCT_CTRL(SOFTRIM_S3T)] /* [0,1048575] */;
  uint32_t SOFTRIM_S3TREFM /* [1,1] */;
  uint32_t SOFTRIM_S3TREFMI[1 EXT_STRUCT_CTRL(SOFTRIM_S3TREFM)] /* [1,1] */;
  uint32_t SOFTRIM_S3TREFME[1 EXT_STRUCT_CTRL(SOFTRIM_S3TREFM)] /* [1,100] */;
  uint32_t SOFTRIM_S3TREF /* [0,100] */;
  uint32_t SOFTRIM_S3TREFv[100 EXT_STRUCT_CTRL(SOFTRIM_S3TREF)] /* [0,1048575] */;
  uint32_t SOFTRIM_S3TTRIGM /* [1,1] */;
  uint32_t SOFTRIM_S3TTRIGMI[1 EXT_STRUCT_CTRL(SOFTRIM_S3TTRIGM)] /* [1,1] */;
  uint32_t SOFTRIM_S3TTRIGME[1 EXT_STRUCT_CTRL(SOFTRIM_S3TTRIGM)] /* [1,100] */;
  uint32_t SOFTRIM_S3TTRIG /* [0,100] */;
  uint32_t SOFTRIM_S3TTRIGv[100 EXT_STRUCT_CTRL(SOFTRIM_S3TTRIG)] /* [0,1048575] */;

} EXT_STR_h101_SOFTRIM;

/********************************************************
 *
 * Structure with multiple levels of arrays (partially)
 * recovered (recommended):
 */

typedef struct EXT_STR_h101_SOFTRIM_onion_t
{
  /* RAW */
  struct {
    uint32_t EM;
    uint32_t EMI[6 /* EM */];
    uint32_t EME[6 /* EM */];
    uint32_t E;
    uint32_t Ev[600 /* E */];
    uint32_t TM;
    uint32_t TMI[6 /* TM */];
    uint32_t TME[6 /* TM */];
    uint32_t T;
    uint32_t Tv[600 /* T */];
    uint32_t TREFM;
    uint32_t TREFMI[1 /* TREFM */];
    uint32_t TREFME[1 /* TREFM */];
    uint32_t TREF;
    uint32_t TREFv[100 /* TREF */];
    uint32_t TTRIGM;
    uint32_t TTRIGMI[1 /* TTRIGM */];
    uint32_t TTRIGME[1 /* TTRIGM */];
    uint32_t TTRIG;
    uint32_t TTRIGv[100 /* TTRIG */];
  } SOFTRIM_S[3];

} EXT_STR_h101_SOFTRIM_onion;

/*******************************************************/

#define EXT_STR_h101_SOFTRIM_ITEMS_INFO(ok,si,offset,struct_t,printerr) do { \
  ok = 1; \
  /* RAW */ \
  EXT_STR_ITEM_INFO_LIM(ok,si,offset,struct_t,printerr,\
                     SOFTRIM_S1EM,                    UINT32,\
                    "SOFTRIM_S1EM",6); \
  EXT_STR_ITEM_INFO_ZZP(ok,si,offset,struct_t,printerr,\
                     SOFTRIM_S1EMI,                   UINT32,\
                    "SOFTRIM_S1EMI",                   "SOFTRIM_S1EM"); \
  EXT_STR_ITEM_INFO_ZZP(ok,si,offset,struct_t,printerr,\
                     SOFTRIM_S1EME,                   UINT32,\
                    "SOFTRIM_S1EME",                   "SOFTRIM_S1EM"); \
  EXT_STR_ITEM_INFO_LIM(ok,si,offset,struct_t,printerr,\
                     SOFTRIM_S1E,                     UINT32,\
                    "SOFTRIM_S1E",600); \
  EXT_STR_ITEM_INFO_ZZP(ok,si,offset,struct_t,printerr,\
                     SOFTRIM_S1Ev,                    UINT32,\
                    "SOFTRIM_S1Ev",                    "SOFTRIM_S1E"); \
  EXT_STR_ITEM_INFO_LIM(ok,si,offset,struct_t,printerr,\
                     SOFTRIM_S1TM,                    UINT32,\
                    "SOFTRIM_S1TM",6); \
  EXT_STR_ITEM_INFO_ZZP(ok,si,offset,struct_t,printerr,\
                     SOFTRIM_S1TMI,                   UINT32,\
                    "SOFTRIM_S1TMI",                   "SOFTRIM_S1TM"); \
  EXT_STR_ITEM_INFO_ZZP(ok,si,offset,struct_t,printerr,\
                     SOFTRIM_S1TME,                   UINT32,\
                    "SOFTRIM_S1TME",                   "SOFTRIM_S1TM"); \
  EXT_STR_ITEM_INFO_LIM(ok,si,offset,struct_t,printerr,\
                     SOFTRIM_S1T,                     UINT32,\
                    "SOFTRIM_S1T",600); \
  EXT_STR_ITEM_INFO_ZZP(ok,si,offset,struct_t,printerr,\
                     SOFTRIM_S1Tv,                    UINT32,\
                    "SOFTRIM_S1Tv",                    "SOFTRIM_S1T"); \
  EXT_STR_ITEM_INFO_LIM(ok,si,offset,struct_t,printerr,\
                     SOFTRIM_S1TREFM,                 UINT32,\
                    "SOFTRIM_S1TREFM",1); \
  EXT_STR_ITEM_INFO_ZZP(ok,si,offset,struct_t,printerr,\
                     SOFTRIM_S1TREFMI,                UINT32,\
                    "SOFTRIM_S1TREFMI",                "SOFTRIM_S1TREFM"); \
  EXT_STR_ITEM_INFO_ZZP(ok,si,offset,struct_t,printerr,\
                     SOFTRIM_S1TREFME,                UINT32,\
                    "SOFTRIM_S1TREFME",                "SOFTRIM_S1TREFM"); \
  EXT_STR_ITEM_INFO_LIM(ok,si,offset,struct_t,printerr,\
                     SOFTRIM_S1TREF,                  UINT32,\
                    "SOFTRIM_S1TREF",100); \
  EXT_STR_ITEM_INFO_ZZP(ok,si,offset,struct_t,printerr,\
                     SOFTRIM_S1TREFv,                 UINT32,\
                    "SOFTRIM_S1TREFv",                 "SOFTRIM_S1TREF"); \
  EXT_STR_ITEM_INFO_LIM(ok,si,offset,struct_t,printerr,\
                     SOFTRIM_S1TTRIGM,                UINT32,\
                    "SOFTRIM_S1TTRIGM",1); \
  EXT_STR_ITEM_INFO_ZZP(ok,si,offset,struct_t,printerr,\
                     SOFTRIM_S1TTRIGMI,               UINT32,\
                    "SOFTRIM_S1TTRIGMI",               "SOFTRIM_S1TTRIGM"); \
  EXT_STR_ITEM_INFO_ZZP(ok,si,offset,struct_t,printerr,\
                     SOFTRIM_S1TTRIGME,               UINT32,\
                    "SOFTRIM_S1TTRIGME",               "SOFTRIM_S1TTRIGM"); \
  EXT_STR_ITEM_INFO_LIM(ok,si,offset,struct_t,printerr,\
                     SOFTRIM_S1TTRIG,                 UINT32,\
                    "SOFTRIM_S1TTRIG",100); \
  EXT_STR_ITEM_INFO_ZZP(ok,si,offset,struct_t,printerr,\
                     SOFTRIM_S1TTRIGv,                UINT32,\
                    "SOFTRIM_S1TTRIGv",                "SOFTRIM_S1TTRIG"); \
  EXT_STR_ITEM_INFO_LIM(ok,si,offset,struct_t,printerr,\
                     SOFTRIM_S2EM,                    UINT32,\
                    "SOFTRIM_S2EM",6); \
  EXT_STR_ITEM_INFO_ZZP(ok,si,offset,struct_t,printerr,\
                     SOFTRIM_S2EMI,                   UINT32,\
                    "SOFTRIM_S2EMI",                   "SOFTRIM_S2EM"); \
  EXT_STR_ITEM_INFO_ZZP(ok,si,offset,struct_t,printerr,\
                     SOFTRIM_S2EME,                   UINT32,\
                    "SOFTRIM_S2EME",                   "SOFTRIM_S2EM"); \
  EXT_STR_ITEM_INFO_LIM(ok,si,offset,struct_t,printerr,\
                     SOFTRIM_S2E,                     UINT32,\
                    "SOFTRIM_S2E",600); \
  EXT_STR_ITEM_INFO_ZZP(ok,si,offset,struct_t,printerr,\
                     SOFTRIM_S2Ev,                    UINT32,\
                    "SOFTRIM_S2Ev",                    "SOFTRIM_S2E"); \
  EXT_STR_ITEM_INFO_LIM(ok,si,offset,struct_t,printerr,\
                     SOFTRIM_S2TM,                    UINT32,\
                    "SOFTRIM_S2TM",6); \
  EXT_STR_ITEM_INFO_ZZP(ok,si,offset,struct_t,printerr,\
                     SOFTRIM_S2TMI,                   UINT32,\
                    "SOFTRIM_S2TMI",                   "SOFTRIM_S2TM"); \
  EXT_STR_ITEM_INFO_ZZP(ok,si,offset,struct_t,printerr,\
                     SOFTRIM_S2TME,                   UINT32,\
                    "SOFTRIM_S2TME",                   "SOFTRIM_S2TM"); \
  EXT_STR_ITEM_INFO_LIM(ok,si,offset,struct_t,printerr,\
                     SOFTRIM_S2T,                     UINT32,\
                    "SOFTRIM_S2T",600); \
  EXT_STR_ITEM_INFO_ZZP(ok,si,offset,struct_t,printerr,\
                     SOFTRIM_S2Tv,                    UINT32,\
                    "SOFTRIM_S2Tv",                    "SOFTRIM_S2T"); \
  EXT_STR_ITEM_INFO_LIM(ok,si,offset,struct_t,printerr,\
                     SOFTRIM_S2TREFM,                 UINT32,\
                    "SOFTRIM_S2TREFM",1); \
  EXT_STR_ITEM_INFO_ZZP(ok,si,offset,struct_t,printerr,\
                     SOFTRIM_S2TREFMI,                UINT32,\
                    "SOFTRIM_S2TREFMI",                "SOFTRIM_S2TREFM"); \
  EXT_STR_ITEM_INFO_ZZP(ok,si,offset,struct_t,printerr,\
                     SOFTRIM_S2TREFME,                UINT32,\
                    "SOFTRIM_S2TREFME",                "SOFTRIM_S2TREFM"); \
  EXT_STR_ITEM_INFO_LIM(ok,si,offset,struct_t,printerr,\
                     SOFTRIM_S2TREF,                  UINT32,\
                    "SOFTRIM_S2TREF",100); \
  EXT_STR_ITEM_INFO_ZZP(ok,si,offset,struct_t,printerr,\
                     SOFTRIM_S2TREFv,                 UINT32,\
                    "SOFTRIM_S2TREFv",                 "SOFTRIM_S2TREF"); \
  EXT_STR_ITEM_INFO_LIM(ok,si,offset,struct_t,printerr,\
                     SOFTRIM_S2TTRIGM,                UINT32,\
                    "SOFTRIM_S2TTRIGM",1); \
  EXT_STR_ITEM_INFO_ZZP(ok,si,offset,struct_t,printerr,\
                     SOFTRIM_S2TTRIGMI,               UINT32,\
                    "SOFTRIM_S2TTRIGMI",               "SOFTRIM_S2TTRIGM"); \
  EXT_STR_ITEM_INFO_ZZP(ok,si,offset,struct_t,printerr,\
                     SOFTRIM_S2TTRIGME,               UINT32,\
                    "SOFTRIM_S2TTRIGME",               "SOFTRIM_S2TTRIGM"); \
  EXT_STR_ITEM_INFO_LIM(ok,si,offset,struct_t,printerr,\
                     SOFTRIM_S2TTRIG,                 UINT32,\
                    "SOFTRIM_S2TTRIG",100); \
  EXT_STR_ITEM_INFO_ZZP(ok,si,offset,struct_t,printerr,\
                     SOFTRIM_S2TTRIGv,                UINT32,\
                    "SOFTRIM_S2TTRIGv",                "SOFTRIM_S2TTRIG"); \
  EXT_STR_ITEM_INFO_LIM(ok,si,offset,struct_t,printerr,\
                     SOFTRIM_S3EM,                    UINT32,\
                    "SOFTRIM_S3EM",6); \
  EXT_STR_ITEM_INFO_ZZP(ok,si,offset,struct_t,printerr,\
                     SOFTRIM_S3EMI,                   UINT32,\
                    "SOFTRIM_S3EMI",                   "SOFTRIM_S3EM"); \
  EXT_STR_ITEM_INFO_ZZP(ok,si,offset,struct_t,printerr,\
                     SOFTRIM_S3EME,                   UINT32,\
                    "SOFTRIM_S3EME",                   "SOFTRIM_S3EM"); \
  EXT_STR_ITEM_INFO_LIM(ok,si,offset,struct_t,printerr,\
                     SOFTRIM_S3E,                     UINT32,\
                    "SOFTRIM_S3E",600); \
  EXT_STR_ITEM_INFO_ZZP(ok,si,offset,struct_t,printerr,\
                     SOFTRIM_S3Ev,                    UINT32,\
                    "SOFTRIM_S3Ev",                    "SOFTRIM_S3E"); \
  EXT_STR_ITEM_INFO_LIM(ok,si,offset,struct_t,printerr,\
                     SOFTRIM_S3TM,                    UINT32,\
                    "SOFTRIM_S3TM",6); \
  EXT_STR_ITEM_INFO_ZZP(ok,si,offset,struct_t,printerr,\
                     SOFTRIM_S3TMI,                   UINT32,\
                    "SOFTRIM_S3TMI",                   "SOFTRIM_S3TM"); \
  EXT_STR_ITEM_INFO_ZZP(ok,si,offset,struct_t,printerr,\
                     SOFTRIM_S3TME,                   UINT32,\
                    "SOFTRIM_S3TME",                   "SOFTRIM_S3TM"); \
  EXT_STR_ITEM_INFO_LIM(ok,si,offset,struct_t,printerr,\
                     SOFTRIM_S3T,                     UINT32,\
                    "SOFTRIM_S3T",600); \
  EXT_STR_ITEM_INFO_ZZP(ok,si,offset,struct_t,printerr,\
                     SOFTRIM_S3Tv,                    UINT32,\
                    "SOFTRIM_S3Tv",                    "SOFTRIM_S3T"); \
  EXT_STR_ITEM_INFO_LIM(ok,si,offset,struct_t,printerr,\
                     SOFTRIM_S3TREFM,                 UINT32,\
                    "SOFTRIM_S3TREFM",1); \
  EXT_STR_ITEM_INFO_ZZP(ok,si,offset,struct_t,printerr,\
                     SOFTRIM_S3TREFMI,                UINT32,\
                    "SOFTRIM_S3TREFMI",                "SOFTRIM_S3TREFM"); \
  EXT_STR_ITEM_INFO_ZZP(ok,si,offset,struct_t,printerr,\
                     SOFTRIM_S3TREFME,                UINT32,\
                    "SOFTRIM_S3TREFME",                "SOFTRIM_S3TREFM"); \
  EXT_STR_ITEM_INFO_LIM(ok,si,offset,struct_t,printerr,\
                     SOFTRIM_S3TREF,                  UINT32,\
                    "SOFTRIM_S3TREF",100); \
  EXT_STR_ITEM_INFO_ZZP(ok,si,offset,struct_t,printerr,\
                     SOFTRIM_S3TREFv,                 UINT32,\
                    "SOFTRIM_S3TREFv",                 "SOFTRIM_S3TREF"); \
  EXT_STR_ITEM_INFO_LIM(ok,si,offset,struct_t,printerr,\
                     SOFTRIM_S3TTRIGM,                UINT32,\
                    "SOFTRIM_S3TTRIGM",1); \
  EXT_STR_ITEM_INFO_ZZP(ok,si,offset,struct_t,printerr,\
                     SOFTRIM_S3TTRIGMI,               UINT32,\
                    "SOFTRIM_S3TTRIGMI",               "SOFTRIM_S3TTRIGM"); \
  EXT_STR_ITEM_INFO_ZZP(ok,si,offset,struct_t,printerr,\
                     SOFTRIM_S3TTRIGME,               UINT32,\
                    "SOFTRIM_S3TTRIGME",               "SOFTRIM_S3TTRIGM"); \
  EXT_STR_ITEM_INFO_LIM(ok,si,offset,struct_t,printerr,\
                     SOFTRIM_S3TTRIG,                 UINT32,\
                    "SOFTRIM_S3TTRIG",100); \
  EXT_STR_ITEM_INFO_ZZP(ok,si,offset,struct_t,printerr,\
                     SOFTRIM_S3TTRIGv,                UINT32,\
                    "SOFTRIM_S3TTRIGv",                "SOFTRIM_S3TTRIG"); \
  \
} while (0);

#endif/*__GUARD_H101_SOFTRIM_EXT_H101_SOFTRIM_H__*/

/*******************************************************/
