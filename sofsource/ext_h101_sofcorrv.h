/********************************************************
 *
 * Structure for ext_data_fetch_event() filling.
 *
 * Do not edit - automatically generated.
 */

#ifndef __GUARD_H101_SOFCORRV_EXT_H101_SOFCORRV_H__
#define __GUARD_H101_SOFCORRV_EXT_H101_SOFCORRV_H__

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

typedef struct EXT_STR_h101_SOFCORRV_t
{
  /* RAW */
  uint32_t SOFCORRV_TRCM /* [1,1] */;
  uint32_t SOFCORRV_TRCMI[1 EXT_STRUCT_CTRL(SOFCORRV_TRCM)] /* [1,1] */;
  uint32_t SOFCORRV_TRCME[1 EXT_STRUCT_CTRL(SOFCORRV_TRCM)] /* [1,100] */;
  uint32_t SOFCORRV_TRC /* [0,100] */;
  uint32_t SOFCORRV_TRCv[100 EXT_STRUCT_CTRL(SOFCORRV_TRC)] /* [0,65535] */;
  uint32_t SOFCORRV_TRFM /* [1,1] */;
  uint32_t SOFCORRV_TRFMI[1 EXT_STRUCT_CTRL(SOFCORRV_TRFM)] /* [1,1] */;
  uint32_t SOFCORRV_TRFME[1 EXT_STRUCT_CTRL(SOFCORRV_TRFM)] /* [1,100] */;
  uint32_t SOFCORRV_TRF /* [0,100] */;
  uint32_t SOFCORRV_TRFv[100 EXT_STRUCT_CTRL(SOFCORRV_TRF)] /* [0,65535] */;

} EXT_STR_h101_SOFCORRV;

/********************************************************
 *
 * Structure with multiple levels of arrays (partially)
 * recovered (recommended):
 */

typedef struct EXT_STR_h101_SOFCORRV_onion_t
{
  /* RAW */
  uint32_t SOFCORRV_TRCM;
  uint32_t SOFCORRV_TRCMI[1 /* SOFCORRV_TRCM */];
  uint32_t SOFCORRV_TRCME[1 /* SOFCORRV_TRCM */];
  uint32_t SOFCORRV_TRC;
  uint32_t SOFCORRV_TRCv[100 /* SOFCORRV_TRC */];
  uint32_t SOFCORRV_TRFM;
  uint32_t SOFCORRV_TRFMI[1 /* SOFCORRV_TRFM */];
  uint32_t SOFCORRV_TRFME[1 /* SOFCORRV_TRFM */];
  uint32_t SOFCORRV_TRF;
  uint32_t SOFCORRV_TRFv[100 /* SOFCORRV_TRF */];

} EXT_STR_h101_SOFCORRV_onion;

/*******************************************************/

#define EXT_STR_h101_SOFCORRV_ITEMS_INFO(ok,si,offset,struct_t,printerr) do { \
  ok = 1; \
  /* RAW */ \
  EXT_STR_ITEM_INFO_LIM(ok,si,offset,struct_t,printerr,\
                     SOFCORRV_TRCM,                   UINT32,\
                    "SOFCORRV_TRCM",1); \
  EXT_STR_ITEM_INFO_ZZP(ok,si,offset,struct_t,printerr,\
                     SOFCORRV_TRCMI,                  UINT32,\
                    "SOFCORRV_TRCMI",                  "SOFCORRV_TRCM"); \
  EXT_STR_ITEM_INFO_ZZP(ok,si,offset,struct_t,printerr,\
                     SOFCORRV_TRCME,                  UINT32,\
                    "SOFCORRV_TRCME",                  "SOFCORRV_TRCM"); \
  EXT_STR_ITEM_INFO_LIM(ok,si,offset,struct_t,printerr,\
                     SOFCORRV_TRC,                    UINT32,\
                    "SOFCORRV_TRC",100); \
  EXT_STR_ITEM_INFO_ZZP(ok,si,offset,struct_t,printerr,\
                     SOFCORRV_TRCv,                   UINT32,\
                    "SOFCORRV_TRCv",                   "SOFCORRV_TRC"); \
  EXT_STR_ITEM_INFO_LIM(ok,si,offset,struct_t,printerr,\
                     SOFCORRV_TRFM,                   UINT32,\
                    "SOFCORRV_TRFM",1); \
  EXT_STR_ITEM_INFO_ZZP(ok,si,offset,struct_t,printerr,\
                     SOFCORRV_TRFMI,                  UINT32,\
                    "SOFCORRV_TRFMI",                  "SOFCORRV_TRFM"); \
  EXT_STR_ITEM_INFO_ZZP(ok,si,offset,struct_t,printerr,\
                     SOFCORRV_TRFME,                  UINT32,\
                    "SOFCORRV_TRFME",                  "SOFCORRV_TRFM"); \
  EXT_STR_ITEM_INFO_LIM(ok,si,offset,struct_t,printerr,\
                     SOFCORRV_TRF,                    UINT32,\
                    "SOFCORRV_TRF",100); \
  EXT_STR_ITEM_INFO_ZZP(ok,si,offset,struct_t,printerr,\
                     SOFCORRV_TRFv,                   UINT32,\
                    "SOFCORRV_TRFv",                   "SOFCORRV_TRF"); \
  \
} while (0);


#endif/*__GUARD_H101_SOFCORRV_EXT_H101_SOFCORRV_H__*/

/*******************************************************/
