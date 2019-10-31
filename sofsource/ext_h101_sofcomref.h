/********************************************************
 *
 * Structure for ext_data_fetch_event() filling.
 *
 * Do not edit - automatically generated.
 */

#ifndef __GUARD_H101_SOFCOMREF_EXT_H101_H__
#define __GUARD_H101_SOFCOMREF_EXT_H101_H__

#ifndef __CINT__
#include <stdint.h>
#else
/* For CINT (old version trouble with stdint.h): */
#ifndef uint32_t
typedef unsigned int uint32_t;
typedef int int32_t;
#endif
#endif
#ifndef EXT_STRUCT_CTRL
#define EXT_STRUCT_CTRL(x)
#endif

/********************************************************
 *
 * Plain structure (layout as ntuple/root file):
 */

typedef struct EXT_STR_h101_SOFCOMREF_t
{
    /* RAW */
    uint32_t SOFCOMREF_S1TFM /* [1,1] */;
    uint32_t SOFCOMREF_S1TFMI[1 EXT_STRUCT_CTRL(SOFCOMREF_S1TFM)] /* [1,1] */;
    uint32_t SOFCOMREF_S1TFME[1 EXT_STRUCT_CTRL(SOFCOMREF_S1TFM)] /* [1,2] */;
    uint32_t SOFCOMREF_S1TF /* [0,2] */;
    uint32_t SOFCOMREF_S1TFv[2 EXT_STRUCT_CTRL(SOFCOMREF_S1TF)] /* [0,65535] */;
    uint32_t SOFCOMREF_S1TCM /* [1,1] */;
    uint32_t SOFCOMREF_S1TCMI[1 EXT_STRUCT_CTRL(SOFCOMREF_S1TCM)] /* [1,1] */;
    uint32_t SOFCOMREF_S1TCME[1 EXT_STRUCT_CTRL(SOFCOMREF_S1TCM)] /* [1,2] */;
    uint32_t SOFCOMREF_S1TC /* [0,2] */;
    uint32_t SOFCOMREF_S1TCv[2 EXT_STRUCT_CTRL(SOFCOMREF_S1TC)] /* [0,65535] */;
    uint32_t SOFCOMREF_S2TFM /* [1,1] */;
    uint32_t SOFCOMREF_S2TFMI[1 EXT_STRUCT_CTRL(SOFCOMREF_S2TFM)] /* [1,1] */;
    uint32_t SOFCOMREF_S2TFME[1 EXT_STRUCT_CTRL(SOFCOMREF_S2TFM)] /* [1,2] */;
    uint32_t SOFCOMREF_S2TF /* [0,2] */;
    uint32_t SOFCOMREF_S2TFv[2 EXT_STRUCT_CTRL(SOFCOMREF_S2TF)] /* [0,65535] */;
    uint32_t SOFCOMREF_S2TCM /* [1,1] */;
    uint32_t SOFCOMREF_S2TCMI[1 EXT_STRUCT_CTRL(SOFCOMREF_S2TCM)] /* [1,1] */;
    uint32_t SOFCOMREF_S2TCME[1 EXT_STRUCT_CTRL(SOFCOMREF_S2TCM)] /* [1,2] */;
    uint32_t SOFCOMREF_S2TC /* [0,2] */;
    uint32_t SOFCOMREF_S2TCv[2 EXT_STRUCT_CTRL(SOFCOMREF_S2TC)] /* [0,65535] */;

} EXT_STR_h101_SOFCOMREF;

/********************************************************
 *
 * Structure with multiple levels of arrays (partially)
 * recovered (recommended):
 */

typedef struct EXT_STR_h101_SOFCOMREF_onion_t
{
    /* RAW */
    struct
    {
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
    } SOFCOMREF_S[2];

} EXT_STR_h101_SOFCOMREF_onion;

/*******************************************************/

#define EXT_STR_h101_SOFCOMREF_ITEMS_INFO(ok, si, offset, struct_t, printerr)                                     \
    do                                                                                                            \
    {                                                                                                             \
        ok = 1;                                                                                                   \
        /* RAW */                                                                                                 \
        EXT_STR_ITEM_INFO_LIM(ok, si, offset, struct_t, printerr, SOFCOMREF_S1TFM, UINT32, "SOFCOMREF_S1TFM", 1); \
        EXT_STR_ITEM_INFO_ZZP(                                                                                    \
            ok, si, offset, struct_t, printerr, SOFCOMREF_S1TFMI, UINT32, "SOFCOMREF_S1TFMI", "SOFCOMREF_S1TFM"); \
        EXT_STR_ITEM_INFO_ZZP(                                                                                    \
            ok, si, offset, struct_t, printerr, SOFCOMREF_S1TFME, UINT32, "SOFCOMREF_S1TFME", "SOFCOMREF_S1TFM"); \
        EXT_STR_ITEM_INFO_LIM(ok, si, offset, struct_t, printerr, SOFCOMREF_S1TF, UINT32, "SOFCOMREF_S1TF", 2);   \
        EXT_STR_ITEM_INFO_ZZP(                                                                                    \
            ok, si, offset, struct_t, printerr, SOFCOMREF_S1TFv, UINT32, "SOFCOMREF_S1TFv", "SOFCOMREF_S1TF");    \
        EXT_STR_ITEM_INFO_LIM(ok, si, offset, struct_t, printerr, SOFCOMREF_S1TCM, UINT32, "SOFCOMREF_S1TCM", 1); \
        EXT_STR_ITEM_INFO_ZZP(                                                                                    \
            ok, si, offset, struct_t, printerr, SOFCOMREF_S1TCMI, UINT32, "SOFCOMREF_S1TCMI", "SOFCOMREF_S1TCM"); \
        EXT_STR_ITEM_INFO_ZZP(                                                                                    \
            ok, si, offset, struct_t, printerr, SOFCOMREF_S1TCME, UINT32, "SOFCOMREF_S1TCME", "SOFCOMREF_S1TCM"); \
        EXT_STR_ITEM_INFO_LIM(ok, si, offset, struct_t, printerr, SOFCOMREF_S1TC, UINT32, "SOFCOMREF_S1TC", 2);   \
        EXT_STR_ITEM_INFO_ZZP(                                                                                    \
            ok, si, offset, struct_t, printerr, SOFCOMREF_S1TCv, UINT32, "SOFCOMREF_S1TCv", "SOFCOMREF_S1TC");    \
        EXT_STR_ITEM_INFO_LIM(ok, si, offset, struct_t, printerr, SOFCOMREF_S2TFM, UINT32, "SOFCOMREF_S2TFM", 1); \
        EXT_STR_ITEM_INFO_ZZP(                                                                                    \
            ok, si, offset, struct_t, printerr, SOFCOMREF_S2TFMI, UINT32, "SOFCOMREF_S2TFMI", "SOFCOMREF_S2TFM"); \
        EXT_STR_ITEM_INFO_ZZP(                                                                                    \
            ok, si, offset, struct_t, printerr, SOFCOMREF_S2TFME, UINT32, "SOFCOMREF_S2TFME", "SOFCOMREF_S2TFM"); \
        EXT_STR_ITEM_INFO_LIM(ok, si, offset, struct_t, printerr, SOFCOMREF_S2TF, UINT32, "SOFCOMREF_S2TF", 2);   \
        EXT_STR_ITEM_INFO_ZZP(                                                                                    \
            ok, si, offset, struct_t, printerr, SOFCOMREF_S2TFv, UINT32, "SOFCOMREF_S2TFv", "SOFCOMREF_S2TF");    \
        EXT_STR_ITEM_INFO_LIM(ok, si, offset, struct_t, printerr, SOFCOMREF_S2TCM, UINT32, "SOFCOMREF_S2TCM", 1); \
        EXT_STR_ITEM_INFO_ZZP(                                                                                    \
            ok, si, offset, struct_t, printerr, SOFCOMREF_S2TCMI, UINT32, "SOFCOMREF_S2TCMI", "SOFCOMREF_S2TCM"); \
        EXT_STR_ITEM_INFO_ZZP(                                                                                    \
            ok, si, offset, struct_t, printerr, SOFCOMREF_S2TCME, UINT32, "SOFCOMREF_S2TCME", "SOFCOMREF_S2TCM"); \
        EXT_STR_ITEM_INFO_LIM(ok, si, offset, struct_t, printerr, SOFCOMREF_S2TC, UINT32, "SOFCOMREF_S2TC", 2);   \
        EXT_STR_ITEM_INFO_ZZP(                                                                                    \
            ok, si, offset, struct_t, printerr, SOFCOMREF_S2TCv, UINT32, "SOFCOMREF_S2TCv", "SOFCOMREF_S2TC");    \
                                                                                                                  \
    } while (0);

#endif /*__GUARD_H101_SOFCONREF_EXT_H101_H__*/
