/********************************************************
 *
 * Structure for ext_data_fetch_event() filling.
 *
 * Do not edit - automatically generated.
 */

#ifndef __GUARD_H101_SOFCOMREF_EXT_H101_H__
#define __GUARD_H101_SOFCOMREF_EXT_H101_H__

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

typedef struct EXT_STR_h101_SOFCOMREF_t
{
  /* RAW */
  uint32_t SOFCOMREF1TFM /* [1,1] */;
  uint32_t SOFCOMREF1TFMI[1 EXT_STRUCT_CTRL(SOFCOMREF1TFM)] /* [1,1] */;
  uint32_t SOFCOMREF1TFME[1 EXT_STRUCT_CTRL(SOFCOMREF1TFM)] /* [1,2] */;
  uint32_t SOFCOMREF1TF /* [0,2] */;
  uint32_t SOFCOMREF1TFv[2 EXT_STRUCT_CTRL(SOFCOMREF1TF)] /* [0,65535] */;
  uint32_t SOFCOMREF1TCM /* [1,1] */;
  uint32_t SOFCOMREF1TCMI[1 EXT_STRUCT_CTRL(SOFCOMREF1TCM)] /* [1,1] */;
  uint32_t SOFCOMREF1TCME[1 EXT_STRUCT_CTRL(SOFCOMREF1TCM)] /* [1,2] */;
  uint32_t SOFCOMREF1TC /* [0,2] */;
  uint32_t SOFCOMREF1TCv[2 EXT_STRUCT_CTRL(SOFCOMREF1TC)] /* [0,65535] */;
  uint32_t SOFCOMREF2TFM /* [1,1] */;
  uint32_t SOFCOMREF2TFMI[1 EXT_STRUCT_CTRL(SOFCOMREF2TFM)] /* [1,1] */;
  uint32_t SOFCOMREF2TFME[1 EXT_STRUCT_CTRL(SOFCOMREF2TFM)] /* [1,2] */;
  uint32_t SOFCOMREF2TF /* [0,2] */;
  uint32_t SOFCOMREF2TFv[2 EXT_STRUCT_CTRL(SOFCOMREF2TF)] /* [0,65535] */;
  uint32_t SOFCOMREF2TCM /* [1,1] */;
  uint32_t SOFCOMREF2TCMI[1 EXT_STRUCT_CTRL(SOFCOMREF2TCM)] /* [1,1] */;
  uint32_t SOFCOMREF2TCME[1 EXT_STRUCT_CTRL(SOFCOMREF2TCM)] /* [1,2] */;
  uint32_t SOFCOMREF2TC /* [0,2] */;
  uint32_t SOFCOMREF2TCv[2 EXT_STRUCT_CTRL(SOFCOMREF2TC)] /* [0,65535] */;

} EXT_STR_h101_SOFCOMREF;

/********************************************************
 *
 * Structure with multiple levels of arrays (partially)
 * recovered (recommended):
 */

typedef struct EXT_STR_h101_SOFCOMREF_onion_t
{
  /* RAW */
  struct {
    uint32_t TFM;
    uint32_t TFMI[1 /* TFM */];
    uint32_t TFME[1 /* TFM */];
    uint32_t TF;
    uint32_t TFv[2 /* TF */];
    uint32_t TCM;
    uint32_t TCMI[1 /* TCM */];
    uint32_t TCME[1 /* TCM */];
    uint32_t TC;
    uint32_t TCv[2 /* TC */];
  } SOFCOMREF[2];

} EXT_STR_h101_SOFCOMREF_onion;

/*******************************************************/

#define EXT_STR_h101_SOFCOMREF_ITEMS_INFO(ok,si,offset,struct_t,printerr) do { \
  ok = 1; \
  /* RAW */ \
  EXT_STR_ITEM_INFO_LIM(ok,si,offset,struct_t,printerr,\
                     SOFCOMREF1TFM,                   UINT32,\
                    "SOFCOMREF1TFM",1); \
  EXT_STR_ITEM_INFO_ZZP(ok,si,offset,struct_t,printerr,\
                     SOFCOMREF1TFMI,                  UINT32,\
                    "SOFCOMREF1TFMI",                  "SOFCOMREF1TFM"); \
  EXT_STR_ITEM_INFO_ZZP(ok,si,offset,struct_t,printerr,\
                     SOFCOMREF1TFME,                  UINT32,\
                    "SOFCOMREF1TFME",                  "SOFCOMREF1TFM"); \
  EXT_STR_ITEM_INFO_LIM(ok,si,offset,struct_t,printerr,\
                     SOFCOMREF1TF,                    UINT32,\
                    "SOFCOMREF1TF",2); \
  EXT_STR_ITEM_INFO_ZZP(ok,si,offset,struct_t,printerr,\
                     SOFCOMREF1TFv,                   UINT32,\
                    "SOFCOMREF1TFv",                   "SOFCOMREF1TF"); \
  EXT_STR_ITEM_INFO_LIM(ok,si,offset,struct_t,printerr,\
                     SOFCOMREF1TCM,                   UINT32,\
                    "SOFCOMREF1TCM",1); \
  EXT_STR_ITEM_INFO_ZZP(ok,si,offset,struct_t,printerr,\
                     SOFCOMREF1TCMI,                  UINT32,\
                    "SOFCOMREF1TCMI",                  "SOFCOMREF1TCM"); \
  EXT_STR_ITEM_INFO_ZZP(ok,si,offset,struct_t,printerr,\
                     SOFCOMREF1TCME,                  UINT32,\
                    "SOFCOMREF1TCME",                  "SOFCOMREF1TCM"); \
  EXT_STR_ITEM_INFO_LIM(ok,si,offset,struct_t,printerr,\
                     SOFCOMREF1TC,                    UINT32,\
                    "SOFCOMREF1TC",2); \
  EXT_STR_ITEM_INFO_ZZP(ok,si,offset,struct_t,printerr,\
                     SOFCOMREF1TCv,                   UINT32,\
                    "SOFCOMREF1TCv",                   "SOFCOMREF1TC"); \
  EXT_STR_ITEM_INFO_LIM(ok,si,offset,struct_t,printerr,\
                     SOFCOMREF2TFM,                   UINT32,\
                    "SOFCOMREF2TFM",1); \
  EXT_STR_ITEM_INFO_ZZP(ok,si,offset,struct_t,printerr,\
                     SOFCOMREF2TFMI,                  UINT32,\
                    "SOFCOMREF2TFMI",                  "SOFCOMREF2TFM"); \
  EXT_STR_ITEM_INFO_ZZP(ok,si,offset,struct_t,printerr,\
                     SOFCOMREF2TFME,                  UINT32,\
                    "SOFCOMREF2TFME",                  "SOFCOMREF2TFM"); \
  EXT_STR_ITEM_INFO_LIM(ok,si,offset,struct_t,printerr,\
                     SOFCOMREF2TF,                    UINT32,\
                    "SOFCOMREF2TF",2); \
  EXT_STR_ITEM_INFO_ZZP(ok,si,offset,struct_t,printerr,\
                     SOFCOMREF2TFv,                   UINT32,\
                    "SOFCOMREF2TFv",                   "SOFCOMREF2TF"); \
  EXT_STR_ITEM_INFO_LIM(ok,si,offset,struct_t,printerr,\
                     SOFCOMREF2TCM,                   UINT32,\
                    "SOFCOMREF2TCM",1); \
  EXT_STR_ITEM_INFO_ZZP(ok,si,offset,struct_t,printerr,\
                     SOFCOMREF2TCMI,                  UINT32,\
                    "SOFCOMREF2TCMI",                  "SOFCOMREF2TCM"); \
  EXT_STR_ITEM_INFO_ZZP(ok,si,offset,struct_t,printerr,\
                     SOFCOMREF2TCME,                  UINT32,\
                    "SOFCOMREF2TCME",                  "SOFCOMREF2TCM"); \
  EXT_STR_ITEM_INFO_LIM(ok,si,offset,struct_t,printerr,\
                     SOFCOMREF2TC,                    UINT32,\
                    "SOFCOMREF2TC",2); \
  EXT_STR_ITEM_INFO_ZZP(ok,si,offset,struct_t,printerr,\
                     SOFCOMREF2TCv,                   UINT32,\
                    "SOFCOMREF2TCv",                   "SOFCOMREF2TC"); \
  \
} while (0);



#endif/*__GUARD_H101_SOFCOMREF_EXT_H101_H__*/

/*******************************************************/
