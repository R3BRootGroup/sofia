/********************************************************
 *
 * Structure for ext_data_fetch_event() filling.
 *
 * Do not edit - automatically generated.
 */

#ifndef __GUARD_H101_SOFAT_MADC_EXT_H101_H__
#define __GUARD_H101_SOFAT_MADC_EXT_H101_H__

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

typedef struct EXT_STR_h101_SOFAT_MADC_t
{
  /* RAW */
  uint32_t SOFAT_E1 /* [0,65535] */;
  uint32_t SOFAT_E2 /* [0,65535] */;
  uint32_t SOFAT_E3 /* [0,65535] */;
  uint32_t SOFAT_E4 /* [0,65535] */;
  uint32_t SOFAT_E5 /* [0,65535] */;

} EXT_STR_h101_SOFAT_MADC;

/********************************************************
 *
 * Structure with multiple levels of arrays (partially)
 * recovered (recommended):
 */

typedef struct EXT_STR_h101_SOFAT_MADC_onion_t
{
  /* RAW */
  uint32_t SOFAT_E[5];

} EXT_STR_h101_SOFAT_MADC_onion;

/*******************************************************/

#define EXT_STR_h101_SOFAT_MADC_ITEMS_INFO(ok,si,offset,struct_t,printerr) do { \
  ok = 1; \
  /* RAW */ \
  EXT_STR_ITEM_INFO_LIM(ok,si,offset,struct_t,printerr,\
                     SOFAT_E1,                        UINT32,\
                    "SOFAT_E1",65535); \
  EXT_STR_ITEM_INFO_LIM(ok,si,offset,struct_t,printerr,\
                     SOFAT_E2,                        UINT32,\
                    "SOFAT_E2",65535); \
  EXT_STR_ITEM_INFO_LIM(ok,si,offset,struct_t,printerr,\
                     SOFAT_E3,                        UINT32,\
                    "SOFAT_E3",65535); \
  EXT_STR_ITEM_INFO_LIM(ok,si,offset,struct_t,printerr,\
                     SOFAT_E4,                        UINT32,\
                    "SOFAT_E4",65535); \
  EXT_STR_ITEM_INFO_LIM(ok,si,offset,struct_t,printerr,\
                     SOFAT_E5,                        UINT32,\
                    "SOFAT_E5",65535); \
  \
} while (0);


#endif/*__GUARD_H101_SOFAT_MADC_EXT_H101_H__*/

/*******************************************************/
