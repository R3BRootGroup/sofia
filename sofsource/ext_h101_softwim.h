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
  uint32_t SOFTWIM_S1EM /* [1,16] */;
  uint32_t SOFTWIM_S1EMI[16 EXT_STRUCT_CTRL(SOFTWIM_S1EM)] /* [1,16] */;
  uint32_t SOFTWIM_S1EME[16 EXT_STRUCT_CTRL(SOFTWIM_S1EM)] /* [1,1600] */;
  uint32_t SOFTWIM_S1E /* [0,1600] */;
  uint32_t SOFTWIM_S1Ev[1600 EXT_STRUCT_CTRL(SOFTWIM_S1E)] /* [0,1048575] */;
  uint32_t SOFTWIM_S1TM /* [1,16] */;
  uint32_t SOFTWIM_S1TMI[16 EXT_STRUCT_CTRL(SOFTWIM_S1TM)] /* [1,16] */;
  uint32_t SOFTWIM_S1TME[16 EXT_STRUCT_CTRL(SOFTWIM_S1TM)] /* [1,1600] */;
  uint32_t SOFTWIM_S1T /* [0,1600] */;
  uint32_t SOFTWIM_S1Tv[1600 EXT_STRUCT_CTRL(SOFTWIM_S1T)] /* [0,1048575] */;
  uint32_t SOFTWIM_S1TREFM /* [1,2] */;
  uint32_t SOFTWIM_S1TREFMI[2 EXT_STRUCT_CTRL(SOFTWIM_S1TREFM)] /* [1,2] */;
  uint32_t SOFTWIM_S1TREFME[2 EXT_STRUCT_CTRL(SOFTWIM_S1TREFM)] /* [1,200] */;
  uint32_t SOFTWIM_S1TREF /* [0,200] */;
  uint32_t SOFTWIM_S1TREFv[200 EXT_STRUCT_CTRL(SOFTWIM_S1TREF)] /* [0,1048575] */;
  uint32_t SOFTWIM_S1TRIGM /* [1,2] */;
  uint32_t SOFTWIM_S1TRIGMI[2 EXT_STRUCT_CTRL(SOFTWIM_S1TRIGM)] /* [1,2] */;
  uint32_t SOFTWIM_S1TRIGME[2 EXT_STRUCT_CTRL(SOFTWIM_S1TRIGM)] /* [1,200] */;
  uint32_t SOFTWIM_S1TRIG /* [0,200] */;
  uint32_t SOFTWIM_S1TRIGv[200 EXT_STRUCT_CTRL(SOFTWIM_S1TRIG)] /* [0,1048575] */;

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
    uint32_t EM;
    uint32_t EMI[16 /* EM */];
    uint32_t EME[16 /* EM */];
    uint32_t E;
    uint32_t Ev[1600 /* E */];
    uint32_t TM;
    uint32_t TMI[16 /* TM */];
    uint32_t TME[16 /* TM */];
    uint32_t T;
    uint32_t Tv[1600 /* T */];
    uint32_t TREFM;
    uint32_t TREFMI[2 /* TREFM */];
    uint32_t TREFME[2 /* TREFM */];
    uint32_t TREF;
    uint32_t TREFv[200 /* TREF */];
    uint32_t TRIGM;
    uint32_t TRIGMI[2 /* TRIGM */];
    uint32_t TRIGME[2 /* TRIGM */];
    uint32_t TRIG;
    uint32_t TRIGv[200 /* TRIG */];
  } SOFTWIM_S[1];

} EXT_STR_h101_SOFTWIM_onion;

/*******************************************************/

#define EXT_STR_h101_SOFTWIM_ITEMS_INFO(ok,si,offset,struct_t,printerr) do { \
  ok = 1; \
  /* RAW */ \
  EXT_STR_ITEM_INFO_LIM(ok,si,offset,struct_t,printerr,\
                     SOFTWIM_S1EM,                    UINT32,\
                    "SOFTWIM_S1EM",16); \
  EXT_STR_ITEM_INFO_ZZP(ok,si,offset,struct_t,printerr,\
                     SOFTWIM_S1EMI,                   UINT32,\
                    "SOFTWIM_S1EMI",                   "SOFTWIM_S1EM"); \
  EXT_STR_ITEM_INFO_ZZP(ok,si,offset,struct_t,printerr,\
                     SOFTWIM_S1EME,                   UINT32,\
                    "SOFTWIM_S1EME",                   "SOFTWIM_S1EM"); \
  EXT_STR_ITEM_INFO_LIM(ok,si,offset,struct_t,printerr,\
                     SOFTWIM_S1E,                     UINT32,\
                    "SOFTWIM_S1E",1600); \
  EXT_STR_ITEM_INFO_ZZP(ok,si,offset,struct_t,printerr,\
                     SOFTWIM_S1Ev,                    UINT32,\
                    "SOFTWIM_S1Ev",                    "SOFTWIM_S1E"); \
  EXT_STR_ITEM_INFO_LIM(ok,si,offset,struct_t,printerr,\
                     SOFTWIM_S1TM,                    UINT32,\
                    "SOFTWIM_S1TM",16); \
  EXT_STR_ITEM_INFO_ZZP(ok,si,offset,struct_t,printerr,\
                     SOFTWIM_S1TMI,                   UINT32,\
                    "SOFTWIM_S1TMI",                   "SOFTWIM_S1TM"); \
  EXT_STR_ITEM_INFO_ZZP(ok,si,offset,struct_t,printerr,\
                     SOFTWIM_S1TME,                   UINT32,\
                    "SOFTWIM_S1TME",                   "SOFTWIM_S1TM"); \
  EXT_STR_ITEM_INFO_LIM(ok,si,offset,struct_t,printerr,\
                     SOFTWIM_S1T,                     UINT32,\
                    "SOFTWIM_S1T",1600); \
  EXT_STR_ITEM_INFO_ZZP(ok,si,offset,struct_t,printerr,\
                     SOFTWIM_S1Tv,                    UINT32,\
                    "SOFTWIM_S1Tv",                    "SOFTWIM_S1T"); \
  EXT_STR_ITEM_INFO_LIM(ok,si,offset,struct_t,printerr,\
                     SOFTWIM_S1TREFM,                 UINT32,\
                    "SOFTWIM_S1TREFM",2); \
  EXT_STR_ITEM_INFO_ZZP(ok,si,offset,struct_t,printerr,\
                     SOFTWIM_S1TREFMI,                UINT32,\
                    "SOFTWIM_S1TREFMI",                "SOFTWIM_S1TREFM"); \
  EXT_STR_ITEM_INFO_ZZP(ok,si,offset,struct_t,printerr,\
                     SOFTWIM_S1TREFME,                UINT32,\
                    "SOFTWIM_S1TREFME",                "SOFTWIM_S1TREFM"); \
  EXT_STR_ITEM_INFO_LIM(ok,si,offset,struct_t,printerr,\
                     SOFTWIM_S1TREF,                  UINT32,\
                    "SOFTWIM_S1TREF",200); \
  EXT_STR_ITEM_INFO_ZZP(ok,si,offset,struct_t,printerr,\
                     SOFTWIM_S1TREFv,                 UINT32,\
                    "SOFTWIM_S1TREFv",                 "SOFTWIM_S1TREF"); \
  EXT_STR_ITEM_INFO_LIM(ok,si,offset,struct_t,printerr,\
                     SOFTWIM_S1TRIGM,                 UINT32,\
                    "SOFTWIM_S1TRIGM",2); \
  EXT_STR_ITEM_INFO_ZZP(ok,si,offset,struct_t,printerr,\
                     SOFTWIM_S1TRIGMI,                UINT32,\
                    "SOFTWIM_S1TRIGMI",                "SOFTWIM_S1TRIGM"); \
  EXT_STR_ITEM_INFO_ZZP(ok,si,offset,struct_t,printerr,\
                     SOFTWIM_S1TRIGME,                UINT32,\
                    "SOFTWIM_S1TRIGME",                "SOFTWIM_S1TRIGM"); \
  EXT_STR_ITEM_INFO_LIM(ok,si,offset,struct_t,printerr,\
                     SOFTWIM_S1TRIG,                  UINT32,\
                    "SOFTWIM_S1TRIG",200); \
  EXT_STR_ITEM_INFO_ZZP(ok,si,offset,struct_t,printerr,\
                     SOFTWIM_S1TRIGv,                 UINT32,\
                    "SOFTWIM_S1TRIGv",                 "SOFTWIM_S1TRIG"); \
  \
} while (0);

#endif/*__GUARD_H101_SOFTWIM_EXT_H101_SOFTWIM_H__*/

/*******************************************************/
