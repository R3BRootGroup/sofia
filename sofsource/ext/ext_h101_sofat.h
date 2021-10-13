/********************************************************
 *
 * Structure for ext_data_fetch_event() filling.
 *
 * Do not edit - automatically generated.
 */

#ifndef __GUARD_H101_SOFAT_EXT_H101_SOFAT_H__
#define __GUARD_H101_SOFAT_EXT_H101_SOFAT_H__

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

typedef struct EXT_STR_h101_SOFAT_t
{
  /* RAW */
  uint32_t SOFAT_EM /* [1,4] */;
  uint32_t SOFAT_EMI[4 EXT_STRUCT_CTRL(SOFAT_EM)] /* [1,4] */;
  uint32_t SOFAT_EME[4 EXT_STRUCT_CTRL(SOFAT_EM)] /* [1,400] */;
  uint32_t SOFAT_E /* [0,400] */;
  uint32_t SOFAT_Ev[400 EXT_STRUCT_CTRL(SOFAT_E)] /* [0,1048575] */;
  uint32_t SOFAT_TM /* [1,4] */;
  uint32_t SOFAT_TMI[4 EXT_STRUCT_CTRL(SOFAT_TM)] /* [1,4] */;
  uint32_t SOFAT_TME[4 EXT_STRUCT_CTRL(SOFAT_TM)] /* [1,400] */;
  uint32_t SOFAT_T /* [0,400] */;
  uint32_t SOFAT_Tv[400 EXT_STRUCT_CTRL(SOFAT_T)] /* [0,1048575] */;

} EXT_STR_h101_SOFAT;

/********************************************************
 *
 * Structure with multiple levels of arrays (partially)
 * recovered (recommended):
 */

typedef struct EXT_STR_h101_SOFAT_onion_t
{
  /* RAW */
  uint32_t SOFAT_EM;
  uint32_t SOFAT_EMI[4 /* SOFAT_EM */];
  uint32_t SOFAT_EME[4 /* SOFAT_EM */];
  uint32_t SOFAT_E;
  uint32_t SOFAT_Ev[400 /* SOFAT_E */];
  uint32_t SOFAT_TM;
  uint32_t SOFAT_TMI[4 /* SOFAT_TM */];
  uint32_t SOFAT_TME[4 /* SOFAT_TM */];
  uint32_t SOFAT_T;
  uint32_t SOFAT_Tv[400 /* SOFAT_T */];

} EXT_STR_h101_SOFAT_onion;

/*******************************************************/

#define EXT_STR_h101_SOFAT_ITEMS_INFO(ok,si,offset,struct_t,printerr) do { \
  ok = 1; \
  /* RAW */ \
  EXT_STR_ITEM_INFO_LIM(ok,si,offset,struct_t,printerr,\
                     SOFAT_EM,                        UINT32,\
                    "SOFAT_EM",4); \
  EXT_STR_ITEM_INFO_ZZP(ok,si,offset,struct_t,printerr,\
                     SOFAT_EMI,                       UINT32,\
                    "SOFAT_EMI",                       "SOFAT_EM"); \
  EXT_STR_ITEM_INFO_ZZP(ok,si,offset,struct_t,printerr,\
                     SOFAT_EME,                       UINT32,\
                    "SOFAT_EME",                       "SOFAT_EM"); \
  EXT_STR_ITEM_INFO_LIM(ok,si,offset,struct_t,printerr,\
                     SOFAT_E,                         UINT32,\
                    "SOFAT_E",400); \
  EXT_STR_ITEM_INFO_ZZP(ok,si,offset,struct_t,printerr,\
                     SOFAT_Ev,                        UINT32,\
                    "SOFAT_Ev",                        "SOFAT_E"); \
  EXT_STR_ITEM_INFO_LIM(ok,si,offset,struct_t,printerr,\
                     SOFAT_TM,                        UINT32,\
                    "SOFAT_TM",4); \
  EXT_STR_ITEM_INFO_ZZP(ok,si,offset,struct_t,printerr,\
                     SOFAT_TMI,                       UINT32,\
                    "SOFAT_TMI",                       "SOFAT_TM"); \
  EXT_STR_ITEM_INFO_ZZP(ok,si,offset,struct_t,printerr,\
                     SOFAT_TME,                       UINT32,\
                    "SOFAT_TME",                       "SOFAT_TM"); \
  EXT_STR_ITEM_INFO_LIM(ok,si,offset,struct_t,printerr,\
                     SOFAT_T,                         UINT32,\
                    "SOFAT_T",400); \
  EXT_STR_ITEM_INFO_ZZP(ok,si,offset,struct_t,printerr,\
                     SOFAT_Tv,                        UINT32,\
                    "SOFAT_Tv",                        "SOFAT_T"); \
  \
} while (0);

#endif/*__GUARD_H100_SOFAT_EXT_H101_SOFAT_H__*/

/*******************************************************/
