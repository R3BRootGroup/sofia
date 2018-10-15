/********************************************************
 *
 * Structure for ext_data_fetch_event() filling.
 *
 * Do not edit - automatically generated.
 */

#ifndef __GUARD_H101_SOFTRIM_MADCVFTX_EXT_H101_H__
#define __GUARD_H101_SOFTRIM_MADCVFTX_EXT_H101_H__

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

typedef struct EXT_STR_h101_SOFTRIM_MADCVFTX_t
{
  /* RAW */
  uint32_t SOFTRIM_P1S1TFM /* [1,6] */;
  uint32_t SOFTRIM_P1S1TFMI[6 EXT_STRUCT_CTRL(SOFTRIM_P1S1TFM)] /* [1,6] */;
  uint32_t SOFTRIM_P1S1TFME[6 EXT_STRUCT_CTRL(SOFTRIM_P1S1TFM)] /* [1,60] */;
  uint32_t SOFTRIM_P1S1TF /* [0,60] */;
  uint32_t SOFTRIM_P1S1TFv[60 EXT_STRUCT_CTRL(SOFTRIM_P1S1TF)] /* [0,65535] */;
  uint32_t SOFTRIM_P1S1TCM /* [1,6] */;
  uint32_t SOFTRIM_P1S1TCMI[6 EXT_STRUCT_CTRL(SOFTRIM_P1S1TCM)] /* [1,6] */;
  uint32_t SOFTRIM_P1S1TCME[6 EXT_STRUCT_CTRL(SOFTRIM_P1S1TCM)] /* [1,60] */;
  uint32_t SOFTRIM_P1S1TC /* [0,60] */;
  uint32_t SOFTRIM_P1S1TCv[60 EXT_STRUCT_CTRL(SOFTRIM_P1S1TC)] /* [0,65535] */;
  uint32_t SOFTRIM_P1S1E1 /* [0,65535] */;
  uint32_t SOFTRIM_P1S1E2 /* [0,65535] */;
  uint32_t SOFTRIM_P1S1E3 /* [0,65535] */;
  uint32_t SOFTRIM_P1S1E4 /* [0,65535] */;
  uint32_t SOFTRIM_P1S1E5 /* [0,65535] */;
  uint32_t SOFTRIM_P1S1E6 /* [0,65535] */;
  uint32_t SOFTRIM_P1S2TFM /* [1,6] */;
  uint32_t SOFTRIM_P1S2TFMI[6 EXT_STRUCT_CTRL(SOFTRIM_P1S2TFM)] /* [1,6] */;
  uint32_t SOFTRIM_P1S2TFME[6 EXT_STRUCT_CTRL(SOFTRIM_P1S2TFM)] /* [1,60] */;
  uint32_t SOFTRIM_P1S2TF /* [0,60] */;
  uint32_t SOFTRIM_P1S2TFv[60 EXT_STRUCT_CTRL(SOFTRIM_P1S2TF)] /* [0,65535] */;
  uint32_t SOFTRIM_P1S2TCM /* [1,6] */;
  uint32_t SOFTRIM_P1S2TCMI[6 EXT_STRUCT_CTRL(SOFTRIM_P1S2TCM)] /* [1,6] */;
  uint32_t SOFTRIM_P1S2TCME[6 EXT_STRUCT_CTRL(SOFTRIM_P1S2TCM)] /* [1,60] */;
  uint32_t SOFTRIM_P1S2TC /* [0,60] */;
  uint32_t SOFTRIM_P1S2TCv[60 EXT_STRUCT_CTRL(SOFTRIM_P1S2TC)] /* [0,65535] */;
  uint32_t SOFTRIM_P1S2E1 /* [0,65535] */;
  uint32_t SOFTRIM_P1S2E2 /* [0,65535] */;
  uint32_t SOFTRIM_P1S2E3 /* [0,65535] */;
  uint32_t SOFTRIM_P1S2E4 /* [0,65535] */;
  uint32_t SOFTRIM_P1S2E5 /* [0,65535] */;
  uint32_t SOFTRIM_P1S2E6 /* [0,65535] */;
  uint32_t SOFTRIM_P1S3TFM /* [1,6] */;
  uint32_t SOFTRIM_P1S3TFMI[6 EXT_STRUCT_CTRL(SOFTRIM_P1S3TFM)] /* [1,6] */;
  uint32_t SOFTRIM_P1S3TFME[6 EXT_STRUCT_CTRL(SOFTRIM_P1S3TFM)] /* [1,60] */;
  uint32_t SOFTRIM_P1S3TF /* [0,60] */;
  uint32_t SOFTRIM_P1S3TFv[60 EXT_STRUCT_CTRL(SOFTRIM_P1S3TF)] /* [0,65535] */;
  uint32_t SOFTRIM_P1S3TCM /* [1,6] */;
  uint32_t SOFTRIM_P1S3TCMI[6 EXT_STRUCT_CTRL(SOFTRIM_P1S3TCM)] /* [1,6] */;
  uint32_t SOFTRIM_P1S3TCME[6 EXT_STRUCT_CTRL(SOFTRIM_P1S3TCM)] /* [1,60] */;
  uint32_t SOFTRIM_P1S3TC /* [0,60] */;
  uint32_t SOFTRIM_P1S3TCv[60 EXT_STRUCT_CTRL(SOFTRIM_P1S3TC)] /* [0,65535] */;
  uint32_t SOFTRIM_P1S3E1 /* [0,65535] */;
  uint32_t SOFTRIM_P1S3E2 /* [0,65535] */;
  uint32_t SOFTRIM_P1S3E3 /* [0,65535] */;
  uint32_t SOFTRIM_P1S3E4 /* [0,65535] */;
  uint32_t SOFTRIM_P1S3E5 /* [0,65535] */;
  uint32_t SOFTRIM_P1S3E6 /* [0,65535] */;

} EXT_STR_h101_SOFTRIM_MADCVFTX;

/********************************************************
 *
 * Structure with multiple levels of arrays (partially)
 * recovered (recommended):
 */

typedef struct EXT_STR_h101_SOFTRIM_MADCVFTX_onion_t
{
  /* RAW */
  struct {
    struct {
      uint32_t TFM;
      uint32_t TFMI[6 /* TFM */];
      uint32_t TFME[6 /* TFM */];
      uint32_t TF;
      uint32_t TFv[60 /* TF */];
      uint32_t TCM;
      uint32_t TCMI[6 /* TCM */];
      uint32_t TCME[6 /* TCM */];
      uint32_t TC;
      uint32_t TCv[60 /* TC */];
      uint32_t E[6];
    } S[3];
  } SOFTRIM_P[1];

} EXT_STR_h101_SOFTRIM_MADCVFTX_onion;

/*******************************************************/

#define EXT_STR_h101_SOFTRIM_MADCVFTX_ITEMS_INFO(ok,si,offset,struct_t,printerr) do { \
  ok = 1; \
  /* RAW */ \
  EXT_STR_ITEM_INFO_LIM(ok,si,offset,struct_t,printerr,\
                     SOFTRIM_P1S1TFM,                 UINT32,\
                    "SOFTRIM_P1S1TFM",6); \
  EXT_STR_ITEM_INFO_ZZP(ok,si,offset,struct_t,printerr,\
                     SOFTRIM_P1S1TFMI,                UINT32,\
                    "SOFTRIM_P1S1TFMI",                "SOFTRIM_P1S1TFM"); \
  EXT_STR_ITEM_INFO_ZZP(ok,si,offset,struct_t,printerr,\
                     SOFTRIM_P1S1TFME,                UINT32,\
                    "SOFTRIM_P1S1TFME",                "SOFTRIM_P1S1TFM"); \
  EXT_STR_ITEM_INFO_LIM(ok,si,offset,struct_t,printerr,\
                     SOFTRIM_P1S1TF,                  UINT32,\
                    "SOFTRIM_P1S1TF",60); \
  EXT_STR_ITEM_INFO_ZZP(ok,si,offset,struct_t,printerr,\
                     SOFTRIM_P1S1TFv,                 UINT32,\
                    "SOFTRIM_P1S1TFv",                 "SOFTRIM_P1S1TF"); \
  EXT_STR_ITEM_INFO_LIM(ok,si,offset,struct_t,printerr,\
                     SOFTRIM_P1S1TCM,                 UINT32,\
                    "SOFTRIM_P1S1TCM",6); \
  EXT_STR_ITEM_INFO_ZZP(ok,si,offset,struct_t,printerr,\
                     SOFTRIM_P1S1TCMI,                UINT32,\
                    "SOFTRIM_P1S1TCMI",                "SOFTRIM_P1S1TCM"); \
  EXT_STR_ITEM_INFO_ZZP(ok,si,offset,struct_t,printerr,\
                     SOFTRIM_P1S1TCME,                UINT32,\
                    "SOFTRIM_P1S1TCME",                "SOFTRIM_P1S1TCM"); \
  EXT_STR_ITEM_INFO_LIM(ok,si,offset,struct_t,printerr,\
                     SOFTRIM_P1S1TC,                  UINT32,\
                    "SOFTRIM_P1S1TC",60); \
  EXT_STR_ITEM_INFO_ZZP(ok,si,offset,struct_t,printerr,\
                     SOFTRIM_P1S1TCv,                 UINT32,\
                    "SOFTRIM_P1S1TCv",                 "SOFTRIM_P1S1TC"); \
  EXT_STR_ITEM_INFO_LIM(ok,si,offset,struct_t,printerr,\
                     SOFTRIM_P1S1E1,                  UINT32,\
                    "SOFTRIM_P1S1E1",65535); \
  EXT_STR_ITEM_INFO_LIM(ok,si,offset,struct_t,printerr,\
                     SOFTRIM_P1S1E2,                  UINT32,\
                    "SOFTRIM_P1S1E2",65535); \
  EXT_STR_ITEM_INFO_LIM(ok,si,offset,struct_t,printerr,\
                     SOFTRIM_P1S1E3,                  UINT32,\
                    "SOFTRIM_P1S1E3",65535); \
  EXT_STR_ITEM_INFO_LIM(ok,si,offset,struct_t,printerr,\
                     SOFTRIM_P1S1E4,                  UINT32,\
                    "SOFTRIM_P1S1E4",65535); \
  EXT_STR_ITEM_INFO_LIM(ok,si,offset,struct_t,printerr,\
                     SOFTRIM_P1S1E5,                  UINT32,\
                    "SOFTRIM_P1S1E5",65535); \
  EXT_STR_ITEM_INFO_LIM(ok,si,offset,struct_t,printerr,\
                     SOFTRIM_P1S1E6,                  UINT32,\
                    "SOFTRIM_P1S1E6",65535); \
  EXT_STR_ITEM_INFO_LIM(ok,si,offset,struct_t,printerr,\
                     SOFTRIM_P1S2TFM,                 UINT32,\
                    "SOFTRIM_P1S2TFM",6); \
  EXT_STR_ITEM_INFO_ZZP(ok,si,offset,struct_t,printerr,\
                     SOFTRIM_P1S2TFMI,                UINT32,\
                    "SOFTRIM_P1S2TFMI",                "SOFTRIM_P1S2TFM"); \
  EXT_STR_ITEM_INFO_ZZP(ok,si,offset,struct_t,printerr,\
                     SOFTRIM_P1S2TFME,                UINT32,\
                    "SOFTRIM_P1S2TFME",                "SOFTRIM_P1S2TFM"); \
  EXT_STR_ITEM_INFO_LIM(ok,si,offset,struct_t,printerr,\
                     SOFTRIM_P1S2TF,                  UINT32,\
                    "SOFTRIM_P1S2TF",60); \
  EXT_STR_ITEM_INFO_ZZP(ok,si,offset,struct_t,printerr,\
                     SOFTRIM_P1S2TFv,                 UINT32,\
                    "SOFTRIM_P1S2TFv",                 "SOFTRIM_P1S2TF"); \
  EXT_STR_ITEM_INFO_LIM(ok,si,offset,struct_t,printerr,\
                     SOFTRIM_P1S2TCM,                 UINT32,\
                    "SOFTRIM_P1S2TCM",6); \
  EXT_STR_ITEM_INFO_ZZP(ok,si,offset,struct_t,printerr,\
                     SOFTRIM_P1S2TCMI,                UINT32,\
                    "SOFTRIM_P1S2TCMI",                "SOFTRIM_P1S2TCM"); \
  EXT_STR_ITEM_INFO_ZZP(ok,si,offset,struct_t,printerr,\
                     SOFTRIM_P1S2TCME,                UINT32,\
                    "SOFTRIM_P1S2TCME",                "SOFTRIM_P1S2TCM"); \
  EXT_STR_ITEM_INFO_LIM(ok,si,offset,struct_t,printerr,\
                     SOFTRIM_P1S2TC,                  UINT32,\
                    "SOFTRIM_P1S2TC",60); \
  EXT_STR_ITEM_INFO_ZZP(ok,si,offset,struct_t,printerr,\
                     SOFTRIM_P1S2TCv,                 UINT32,\
                    "SOFTRIM_P1S2TCv",                 "SOFTRIM_P1S2TC"); \
  EXT_STR_ITEM_INFO_LIM(ok,si,offset,struct_t,printerr,\
                     SOFTRIM_P1S2E1,                  UINT32,\
                    "SOFTRIM_P1S2E1",65535); \
  EXT_STR_ITEM_INFO_LIM(ok,si,offset,struct_t,printerr,\
                     SOFTRIM_P1S2E2,                  UINT32,\
                    "SOFTRIM_P1S2E2",65535); \
  EXT_STR_ITEM_INFO_LIM(ok,si,offset,struct_t,printerr,\
                     SOFTRIM_P1S2E3,                  UINT32,\
                    "SOFTRIM_P1S2E3",65535); \
  EXT_STR_ITEM_INFO_LIM(ok,si,offset,struct_t,printerr,\
                     SOFTRIM_P1S2E4,                  UINT32,\
                    "SOFTRIM_P1S2E4",65535); \
  EXT_STR_ITEM_INFO_LIM(ok,si,offset,struct_t,printerr,\
                     SOFTRIM_P1S2E5,                  UINT32,\
                    "SOFTRIM_P1S2E5",65535); \
  EXT_STR_ITEM_INFO_LIM(ok,si,offset,struct_t,printerr,\
                     SOFTRIM_P1S2E6,                  UINT32,\
                    "SOFTRIM_P1S2E6",65535); \
  EXT_STR_ITEM_INFO_LIM(ok,si,offset,struct_t,printerr,\
                     SOFTRIM_P1S3TFM,                 UINT32,\
                    "SOFTRIM_P1S3TFM",6); \
  EXT_STR_ITEM_INFO_ZZP(ok,si,offset,struct_t,printerr,\
                     SOFTRIM_P1S3TFMI,                UINT32,\
                    "SOFTRIM_P1S3TFMI",                "SOFTRIM_P1S3TFM"); \
  EXT_STR_ITEM_INFO_ZZP(ok,si,offset,struct_t,printerr,\
                     SOFTRIM_P1S3TFME,                UINT32,\
                    "SOFTRIM_P1S3TFME",                "SOFTRIM_P1S3TFM"); \
  EXT_STR_ITEM_INFO_LIM(ok,si,offset,struct_t,printerr,\
                     SOFTRIM_P1S3TF,                  UINT32,\
                    "SOFTRIM_P1S3TF",60); \
  EXT_STR_ITEM_INFO_ZZP(ok,si,offset,struct_t,printerr,\
                     SOFTRIM_P1S3TFv,                 UINT32,\
                    "SOFTRIM_P1S3TFv",                 "SOFTRIM_P1S3TF"); \
  EXT_STR_ITEM_INFO_LIM(ok,si,offset,struct_t,printerr,\
                     SOFTRIM_P1S3TCM,                 UINT32,\
                    "SOFTRIM_P1S3TCM",6); \
  EXT_STR_ITEM_INFO_ZZP(ok,si,offset,struct_t,printerr,\
                     SOFTRIM_P1S3TCMI,                UINT32,\
                    "SOFTRIM_P1S3TCMI",                "SOFTRIM_P1S3TCM"); \
  EXT_STR_ITEM_INFO_ZZP(ok,si,offset,struct_t,printerr,\
                     SOFTRIM_P1S3TCME,                UINT32,\
                    "SOFTRIM_P1S3TCME",                "SOFTRIM_P1S3TCM"); \
  EXT_STR_ITEM_INFO_LIM(ok,si,offset,struct_t,printerr,\
                     SOFTRIM_P1S3TC,                  UINT32,\
                    "SOFTRIM_P1S3TC",60); \
  EXT_STR_ITEM_INFO_ZZP(ok,si,offset,struct_t,printerr,\
                     SOFTRIM_P1S3TCv,                 UINT32,\
                    "SOFTRIM_P1S3TCv",                 "SOFTRIM_P1S3TC"); \
  EXT_STR_ITEM_INFO_LIM(ok,si,offset,struct_t,printerr,\
                     SOFTRIM_P1S3E1,                  UINT32,\
                    "SOFTRIM_P1S3E1",65535); \
  EXT_STR_ITEM_INFO_LIM(ok,si,offset,struct_t,printerr,\
                     SOFTRIM_P1S3E2,                  UINT32,\
                    "SOFTRIM_P1S3E2",65535); \
  EXT_STR_ITEM_INFO_LIM(ok,si,offset,struct_t,printerr,\
                     SOFTRIM_P1S3E3,                  UINT32,\
                    "SOFTRIM_P1S3E3",65535); \
  EXT_STR_ITEM_INFO_LIM(ok,si,offset,struct_t,printerr,\
                     SOFTRIM_P1S3E4,                  UINT32,\
                    "SOFTRIM_P1S3E4",65535); \
  EXT_STR_ITEM_INFO_LIM(ok,si,offset,struct_t,printerr,\
                     SOFTRIM_P1S3E5,                  UINT32,\
                    "SOFTRIM_P1S3E5",65535); \
  EXT_STR_ITEM_INFO_LIM(ok,si,offset,struct_t,printerr,\
                     SOFTRIM_P1S3E6,                  UINT32,\
                    "SOFTRIM_P1S3E6",65535); \
  \
} while (0);

#endif
