/********************************************************
 *
 * Structure for ext_data_fetch_event() filling.
 *
 * Do not edit - automatically generated.
 */

#ifndef __GUARD_H101_SOFCORRM_EXT_H101_SOFCORRM_H__
#define __GUARD_H101_SOFCORRM_EXT_H101_SOFCORRM_H__

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

typedef struct EXT_STR_h101_SOFCORRM_t
{
  /* RAW */
  uint32_t SOFCORRM_TRM /* [1,1] */;
  uint32_t SOFCORRM_TRMI[1 EXT_STRUCT_CTRL(SOFCORRM_TRM)] /* [1,1] */;
  uint32_t SOFCORRM_TRME[1 EXT_STRUCT_CTRL(SOFCORRM_TRM)] /* [1,100] */;
  uint32_t SOFCORRM_TR /* [0,100] */;
  uint32_t SOFCORRM_TRv[100 EXT_STRUCT_CTRL(SOFCORRM_TR)] /* [0,1048575] */;
  uint32_t SOFCORRM_TTM /* [1,1] */;
  uint32_t SOFCORRM_TTMI[1 EXT_STRUCT_CTRL(SOFCORRM_TTM)] /* [1,1] */;
  uint32_t SOFCORRM_TTME[1 EXT_STRUCT_CTRL(SOFCORRM_TTM)] /* [1,100] */;
  uint32_t SOFCORRM_TT /* [0,100] */;
  uint32_t SOFCORRM_TTv[100 EXT_STRUCT_CTRL(SOFCORRM_TT)] /* [0,1048575] */;

} EXT_STR_h101_SOFCORRM;

/********************************************************
 *
 * Structure with multiple levels of arrays (partially)
 * recovered (recommended):
 */

typedef struct EXT_STR_h101_SOFCORRM_onion_t
{
  /* RAW */
  uint32_t SOFCORRM_TRM;
  uint32_t SOFCORRM_TRMI[1 /* SOFCORRM_TRM */];
  uint32_t SOFCORRM_TRME[1 /* SOFCORRM_TRM */];
  uint32_t SOFCORRM_TR;
  uint32_t SOFCORRM_TRv[100 /* SOFCORRM_TR */];
  uint32_t SOFCORRM_TTM;
  uint32_t SOFCORRM_TTMI[1 /* SOFCORRM_TTM */];
  uint32_t SOFCORRM_TTME[1 /* SOFCORRM_TTM */];
  uint32_t SOFCORRM_TT;
  uint32_t SOFCORRM_TTv[100 /* SOFCORRM_TT */];

} EXT_STR_h101_SOFCORRM_onion;

/*******************************************************/

#define EXT_STR_h101_SOFCORRM_ITEMS_INFO(ok,si,offset,struct_t,printerr) do { \
  ok = 1; \
  /* RAW */ \
  EXT_STR_ITEM_INFO_LIM(ok,si,offset,struct_t,printerr,\
                     SOFCORRM_TRM,                    UINT32,\
                    "SOFCORRM_TRM",1); \
  EXT_STR_ITEM_INFO_ZZP(ok,si,offset,struct_t,printerr,\
                     SOFCORRM_TRMI,                   UINT32,\
                    "SOFCORRM_TRMI",                   "SOFCORRM_TRM"); \
  EXT_STR_ITEM_INFO_ZZP(ok,si,offset,struct_t,printerr,\
                     SOFCORRM_TRME,                   UINT32,\
                    "SOFCORRM_TRME",                   "SOFCORRM_TRM"); \
  EXT_STR_ITEM_INFO_LIM(ok,si,offset,struct_t,printerr,\
                     SOFCORRM_TR,                     UINT32,\
                    "SOFCORRM_TR",100); \
  EXT_STR_ITEM_INFO_ZZP(ok,si,offset,struct_t,printerr,\
                     SOFCORRM_TRv,                    UINT32,\
                    "SOFCORRM_TRv",                    "SOFCORRM_TR"); \
  EXT_STR_ITEM_INFO_LIM(ok,si,offset,struct_t,printerr,\
                     SOFCORRM_TTM,                    UINT32,\
                    "SOFCORRM_TTM",1); \
  EXT_STR_ITEM_INFO_ZZP(ok,si,offset,struct_t,printerr,\
                     SOFCORRM_TTMI,                   UINT32,\
                    "SOFCORRM_TTMI",                   "SOFCORRM_TTM"); \
  EXT_STR_ITEM_INFO_ZZP(ok,si,offset,struct_t,printerr,\
                     SOFCORRM_TTME,                   UINT32,\
                    "SOFCORRM_TTME",                   "SOFCORRM_TTM"); \
  EXT_STR_ITEM_INFO_LIM(ok,si,offset,struct_t,printerr,\
                     SOFCORRM_TT,                     UINT32,\
                    "SOFCORRM_TT",100); \
  EXT_STR_ITEM_INFO_ZZP(ok,si,offset,struct_t,printerr,\
                     SOFCORRM_TTv,                    UINT32,\
                    "SOFCORRM_TTv",                    "SOFCORRM_TT"); \
  \
} while (0);

#endif/*__GUARD_H101_SOFCORRM_EXT_H101_SOFCORRM_H__*/

/*******************************************************/
