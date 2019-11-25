/********************************************************
 *
 * Structure for ext_data_fetch_event() filling.
 *
 * Do not edit - automatically generated.
 */

#ifndef __GUARD_H101_SOFSCI_EXT_H101_H__
#define __GUARD_H101_SOFSCI_EXT_H101_H__

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

typedef struct EXT_STR_h101_SOFSCI_t
{
  /* RAW */
  uint32_t SOFSCI1TFM /* [1,3] */;
  uint32_t SOFSCI1TFMI[3 EXT_STRUCT_CTRL(SOFSCI1TFM)] /* [1,3] */;
  uint32_t SOFSCI1TFME[3 EXT_STRUCT_CTRL(SOFSCI1TFM)] /* [1,30] */;
  uint32_t SOFSCI1TF /* [0,30] */;
  uint32_t SOFSCI1TFv[30 EXT_STRUCT_CTRL(SOFSCI1TF)] /* [0,65535] */;
  uint32_t SOFSCI1TCM /* [1,3] */;
  uint32_t SOFSCI1TCMI[3 EXT_STRUCT_CTRL(SOFSCI1TCM)] /* [1,3] */;
  uint32_t SOFSCI1TCME[3 EXT_STRUCT_CTRL(SOFSCI1TCM)] /* [1,30] */;
  uint32_t SOFSCI1TC /* [0,30] */;
  uint32_t SOFSCI1TCv[30 EXT_STRUCT_CTRL(SOFSCI1TC)] /* [0,65535] */;

} EXT_STR_h101_SOFSCI;

/********************************************************
 *
 * Structure with multiple levels of arrays (partially)
 * recovered (recommended):
 */

typedef struct EXT_STR_h101_SOFSCI_onion_t
{
  /* RAW */
  struct {
    uint32_t TFM;
    uint32_t TFMI[3 /* TFM */];
    uint32_t TFME[3 /* TFM */];
    uint32_t TF;
    uint32_t TFv[30 /* TF */];
    uint32_t TCM;
    uint32_t TCMI[3 /* TCM */];
    uint32_t TCME[3 /* TCM */];
    uint32_t TC;
    uint32_t TCv[30 /* TC */];
  } SOFSCI[1];

} EXT_STR_h101_SOFSCI_onion;

/*******************************************************/

#define EXT_STR_h101_SOFSCI_ITEMS_INFO(ok,si,offset,struct_t,printerr) do { \
  ok = 1; \
  /* RAW */ \
  EXT_STR_ITEM_INFO_LIM(ok,si,offset,struct_t,printerr,\
                     SOFSCI1TFM,                      UINT32,\
                    "SOFSCI1TFM",3); \
  EXT_STR_ITEM_INFO_ZZP(ok,si,offset,struct_t,printerr,\
                     SOFSCI1TFMI,                     UINT32,\
                    "SOFSCI1TFMI",                     "SOFSCI1TFM"); \
  EXT_STR_ITEM_INFO_ZZP(ok,si,offset,struct_t,printerr,\
                     SOFSCI1TFME,                     UINT32,\
                    "SOFSCI1TFME",                     "SOFSCI1TFM"); \
  EXT_STR_ITEM_INFO_LIM(ok,si,offset,struct_t,printerr,\
                     SOFSCI1TF,                       UINT32,\
                    "SOFSCI1TF",30); \
  EXT_STR_ITEM_INFO_ZZP(ok,si,offset,struct_t,printerr,\
                     SOFSCI1TFv,                      UINT32,\
                    "SOFSCI1TFv",                      "SOFSCI1TF"); \
  EXT_STR_ITEM_INFO_LIM(ok,si,offset,struct_t,printerr,\
                     SOFSCI1TCM,                      UINT32,\
                    "SOFSCI1TCM",3); \
  EXT_STR_ITEM_INFO_ZZP(ok,si,offset,struct_t,printerr,\
                     SOFSCI1TCMI,                     UINT32,\
                    "SOFSCI1TCMI",                     "SOFSCI1TCM"); \
  EXT_STR_ITEM_INFO_ZZP(ok,si,offset,struct_t,printerr,\
                     SOFSCI1TCME,                     UINT32,\
                    "SOFSCI1TCME",                     "SOFSCI1TCM"); \
  EXT_STR_ITEM_INFO_LIM(ok,si,offset,struct_t,printerr,\
                     SOFSCI1TC,                       UINT32,\
                    "SOFSCI1TC",30); \
  EXT_STR_ITEM_INFO_ZZP(ok,si,offset,struct_t,printerr,\
                     SOFSCI1TCv,                      UINT32,\
                    "SOFSCI1TCv",                      "SOFSCI1TC"); \
  \
} while (0);

/********************************************************
 *
 * For internal use by the network data reader:
 * (version checks, etc)
 */


#endif/*__GUARD_H101_SOFSCI_EXT_H101_H__*/

/*******************************************************/
