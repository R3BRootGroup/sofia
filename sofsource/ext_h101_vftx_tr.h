/********************************************************
 *
 * Structure for ext_data_fetch_event() filling.
 *
 * Do not edit - automatically generated.
 */

#ifndef __GUARD_H101_VFTX_TR_EXT_H101_VFTX_TR_H__
#define __GUARD_H101_VFTX_TR_EXT_H101_VFTX_TR_H__

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

typedef struct EXT_STR_h101_VFTX_TR_t
{
  /* RAW */
  uint32_t VFTX_TR /* [0,6] */;
  uint32_t VFTX_TRI[6 EXT_STRUCT_CTRL(VFTX_TR)] /* [1,6] */;
  uint32_t VFTX_TRv[6 EXT_STRUCT_CTRL(VFTX_TR)] /* [0,65535] */;

} EXT_STR_h101_VFTX_TR;

/********************************************************
 *
 * Structure with multiple levels of arrays (partially)
 * recovered (recommended):
 */

typedef struct EXT_STR_h101_VFTX_TR_onion_t
{
  /* RAW */
  uint32_t VFTX_TR;
  uint32_t VFTX_TRI[6 /* VFTX_TR */];
  uint32_t VFTX_TRv[6 /* VFTX_TR */];

} EXT_STR_h101_VFTX_TR_onion;

/*******************************************************/

#define EXT_STR_h101_VFTX_TR_ITEMS_INFO(ok,si,offset,struct_t,printerr) do { \
  ok = 1; \
  /* RAW */ \
  EXT_STR_ITEM_INFO_LIM(ok,si,offset,struct_t,printerr,\
                     VFTX_TR,                         UINT32,\
                    "VFTX_TR",6); \
  EXT_STR_ITEM_INFO_ZZP(ok,si,offset,struct_t,printerr,\
                     VFTX_TRI,                        UINT32,\
                    "VFTX_TRI",                        "VFTX_TR"); \
  EXT_STR_ITEM_INFO_ZZP(ok,si,offset,struct_t,printerr,\
                     VFTX_TRv,                        UINT32,\
                    "VFTX_TRv",                        "VFTX_TR"); \
  \
} while (0);


#endif/*__GUARD_H101_VFTX_TR_EXT_H101_VFTX_TR_H__*/

/*******************************************************/
