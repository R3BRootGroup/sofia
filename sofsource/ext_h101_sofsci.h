/********************************************************
 *
 * Structure for ext_data_fetch_event() filling.
 *
 * Do not edit - automatically generated.
 */

#ifndef __GUARD_H101_SOFSCI_EXT_H101_SOFSCI_H__
#define __GUARD_H101_SOFSCI_EXT_H101_SOFSCI_H__

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

typedef struct EXT_STR_h101_SOFSCI_t
{
    /* RAW */
    uint32_t SOFSCI1TFM /* [1,3] */;
    uint32_t SOFSCI1TFMI[3 EXT_STRUCT_CTRL(SOFSCI1TFM)] /* [1,3] */;
    uint32_t SOFSCI1TFME[3 EXT_STRUCT_CTRL(SOFSCI1TFM)] /* [1,300] */;
    uint32_t SOFSCI1TF /* [0,300] */;
    uint32_t SOFSCI1TFv[300 EXT_STRUCT_CTRL(SOFSCI1TF)] /* [0,65535] */;
    uint32_t SOFSCI1TCM /* [1,3] */;
    uint32_t SOFSCI1TCMI[3 EXT_STRUCT_CTRL(SOFSCI1TCM)] /* [1,3] */;
    uint32_t SOFSCI1TCME[3 EXT_STRUCT_CTRL(SOFSCI1TCM)] /* [1,300] */;
    uint32_t SOFSCI1TC /* [0,300] */;
    uint32_t SOFSCI1TCv[300 EXT_STRUCT_CTRL(SOFSCI1TC)] /* [0,65535] */;
    uint32_t SOFSCI2TFM /* [1,3] */;
    uint32_t SOFSCI2TFMI[3 EXT_STRUCT_CTRL(SOFSCI2TFM)] /* [1,3] */;
    uint32_t SOFSCI2TFME[3 EXT_STRUCT_CTRL(SOFSCI2TFM)] /* [1,300] */;
    uint32_t SOFSCI2TF /* [0,300] */;
    uint32_t SOFSCI2TFv[300 EXT_STRUCT_CTRL(SOFSCI2TF)] /* [0,65535] */;
    uint32_t SOFSCI2TCM /* [1,3] */;
    uint32_t SOFSCI2TCMI[3 EXT_STRUCT_CTRL(SOFSCI2TCM)] /* [1,3] */;
    uint32_t SOFSCI2TCME[3 EXT_STRUCT_CTRL(SOFSCI2TCM)] /* [1,300] */;
    uint32_t SOFSCI2TC /* [0,300] */;
    uint32_t SOFSCI2TCv[300 EXT_STRUCT_CTRL(SOFSCI2TC)] /* [0,65535] */;
    uint32_t SOFSCI3TFM /* [1,3] */;
    uint32_t SOFSCI3TFMI[3 EXT_STRUCT_CTRL(SOFSCI3TFM)] /* [1,3] */;
    uint32_t SOFSCI3TFME[3 EXT_STRUCT_CTRL(SOFSCI3TFM)] /* [1,300] */;
    uint32_t SOFSCI3TF /* [0,300] */;
    uint32_t SOFSCI3TFv[300 EXT_STRUCT_CTRL(SOFSCI3TF)] /* [0,65535] */;
    uint32_t SOFSCI3TCM /* [1,3] */;
    uint32_t SOFSCI3TCMI[3 EXT_STRUCT_CTRL(SOFSCI3TCM)] /* [1,3] */;
    uint32_t SOFSCI3TCME[3 EXT_STRUCT_CTRL(SOFSCI3TCM)] /* [1,300] */;
    uint32_t SOFSCI3TC /* [0,300] */;
    uint32_t SOFSCI3TCv[300 EXT_STRUCT_CTRL(SOFSCI3TC)] /* [0,65535] */;
    uint32_t SOFSCI4TFM /* [1,3] */;
    uint32_t SOFSCI4TFMI[3 EXT_STRUCT_CTRL(SOFSCI4TFM)] /* [1,3] */;
    uint32_t SOFSCI4TFME[3 EXT_STRUCT_CTRL(SOFSCI4TFM)] /* [1,300] */;
    uint32_t SOFSCI4TF /* [0,300] */;
    uint32_t SOFSCI4TFv[300 EXT_STRUCT_CTRL(SOFSCI4TF)] /* [0,65535] */;
    uint32_t SOFSCI4TCM /* [1,3] */;
    uint32_t SOFSCI4TCMI[3 EXT_STRUCT_CTRL(SOFSCI4TCM)] /* [1,3] */;
    uint32_t SOFSCI4TCME[3 EXT_STRUCT_CTRL(SOFSCI4TCM)] /* [1,300] */;
    uint32_t SOFSCI4TC /* [0,300] */;
    uint32_t SOFSCI4TCv[300 EXT_STRUCT_CTRL(SOFSCI4TC)] /* [0,65535] */;

} EXT_STR_h101_SOFSCI;

/********************************************************
 *
 * Structure with multiple levels of arrays (partially)
 * recovered (recommended):
 */

typedef struct EXT_STR_h101_SOFSCI_onion_t
{
    /* RAW */
    struct
    {
        uint32_t TFM;
        uint32_t TFMI[3 /* TFM */];
        uint32_t TFME[3 /* TFM */];
        uint32_t TF;
        uint32_t TFv[300 /* TF */];
        uint32_t TCM;
        uint32_t TCMI[3 /* TCM */];
        uint32_t TCME[3 /* TCM */];
        uint32_t TC;
        uint32_t TCv[300 /* TC */];
    } SOFSCI[4];

} EXT_STR_h101_SOFSCI_onion;

/*******************************************************/

#define EXT_STR_h101_SOFSCI_ITEMS_INFO(ok, si, offset, struct_t, printerr)                                           \
    do                                                                                                               \
    {                                                                                                                \
        ok = 1;                                                                                                      \
        /* RAW */                                                                                                    \
        EXT_STR_ITEM_INFO_LIM(ok, si, offset, struct_t, printerr, SOFSCI1TFM, UINT32, "SOFSCI1TFM", 3);              \
        EXT_STR_ITEM_INFO_ZZP(ok, si, offset, struct_t, printerr, SOFSCI1TFMI, UINT32, "SOFSCI1TFMI", "SOFSCI1TFM"); \
        EXT_STR_ITEM_INFO_ZZP(ok, si, offset, struct_t, printerr, SOFSCI1TFME, UINT32, "SOFSCI1TFME", "SOFSCI1TFM"); \
        EXT_STR_ITEM_INFO_LIM(ok, si, offset, struct_t, printerr, SOFSCI1TF, UINT32, "SOFSCI1TF", 300);              \
        EXT_STR_ITEM_INFO_ZZP(ok, si, offset, struct_t, printerr, SOFSCI1TFv, UINT32, "SOFSCI1TFv", "SOFSCI1TF");    \
        EXT_STR_ITEM_INFO_LIM(ok, si, offset, struct_t, printerr, SOFSCI1TCM, UINT32, "SOFSCI1TCM", 3);              \
        EXT_STR_ITEM_INFO_ZZP(ok, si, offset, struct_t, printerr, SOFSCI1TCMI, UINT32, "SOFSCI1TCMI", "SOFSCI1TCM"); \
        EXT_STR_ITEM_INFO_ZZP(ok, si, offset, struct_t, printerr, SOFSCI1TCME, UINT32, "SOFSCI1TCME", "SOFSCI1TCM"); \
        EXT_STR_ITEM_INFO_LIM(ok, si, offset, struct_t, printerr, SOFSCI1TC, UINT32, "SOFSCI1TC", 300);              \
        EXT_STR_ITEM_INFO_ZZP(ok, si, offset, struct_t, printerr, SOFSCI1TCv, UINT32, "SOFSCI1TCv", "SOFSCI1TC");    \
        EXT_STR_ITEM_INFO_LIM(ok, si, offset, struct_t, printerr, SOFSCI2TFM, UINT32, "SOFSCI2TFM", 3);              \
        EXT_STR_ITEM_INFO_ZZP(ok, si, offset, struct_t, printerr, SOFSCI2TFMI, UINT32, "SOFSCI2TFMI", "SOFSCI2TFM"); \
        EXT_STR_ITEM_INFO_ZZP(ok, si, offset, struct_t, printerr, SOFSCI2TFME, UINT32, "SOFSCI2TFME", "SOFSCI2TFM"); \
        EXT_STR_ITEM_INFO_LIM(ok, si, offset, struct_t, printerr, SOFSCI2TF, UINT32, "SOFSCI2TF", 300);              \
        EXT_STR_ITEM_INFO_ZZP(ok, si, offset, struct_t, printerr, SOFSCI2TFv, UINT32, "SOFSCI2TFv", "SOFSCI2TF");    \
        EXT_STR_ITEM_INFO_LIM(ok, si, offset, struct_t, printerr, SOFSCI2TCM, UINT32, "SOFSCI2TCM", 3);              \
        EXT_STR_ITEM_INFO_ZZP(ok, si, offset, struct_t, printerr, SOFSCI2TCMI, UINT32, "SOFSCI2TCMI", "SOFSCI2TCM"); \
        EXT_STR_ITEM_INFO_ZZP(ok, si, offset, struct_t, printerr, SOFSCI2TCME, UINT32, "SOFSCI2TCME", "SOFSCI2TCM"); \
        EXT_STR_ITEM_INFO_LIM(ok, si, offset, struct_t, printerr, SOFSCI2TC, UINT32, "SOFSCI2TC", 300);              \
        EXT_STR_ITEM_INFO_ZZP(ok, si, offset, struct_t, printerr, SOFSCI2TCv, UINT32, "SOFSCI2TCv", "SOFSCI2TC");    \
        EXT_STR_ITEM_INFO_LIM(ok, si, offset, struct_t, printerr, SOFSCI3TFM, UINT32, "SOFSCI3TFM", 3);              \
        EXT_STR_ITEM_INFO_ZZP(ok, si, offset, struct_t, printerr, SOFSCI3TFMI, UINT32, "SOFSCI3TFMI", "SOFSCI3TFM"); \
        EXT_STR_ITEM_INFO_ZZP(ok, si, offset, struct_t, printerr, SOFSCI3TFME, UINT32, "SOFSCI3TFME", "SOFSCI3TFM"); \
        EXT_STR_ITEM_INFO_LIM(ok, si, offset, struct_t, printerr, SOFSCI3TF, UINT32, "SOFSCI3TF", 300);              \
        EXT_STR_ITEM_INFO_ZZP(ok, si, offset, struct_t, printerr, SOFSCI3TFv, UINT32, "SOFSCI3TFv", "SOFSCI3TF");    \
        EXT_STR_ITEM_INFO_LIM(ok, si, offset, struct_t, printerr, SOFSCI3TCM, UINT32, "SOFSCI3TCM", 3);              \
        EXT_STR_ITEM_INFO_ZZP(ok, si, offset, struct_t, printerr, SOFSCI3TCMI, UINT32, "SOFSCI3TCMI", "SOFSCI3TCM"); \
        EXT_STR_ITEM_INFO_ZZP(ok, si, offset, struct_t, printerr, SOFSCI3TCME, UINT32, "SOFSCI3TCME", "SOFSCI3TCM"); \
        EXT_STR_ITEM_INFO_LIM(ok, si, offset, struct_t, printerr, SOFSCI3TC, UINT32, "SOFSCI3TC", 300);              \
        EXT_STR_ITEM_INFO_ZZP(ok, si, offset, struct_t, printerr, SOFSCI3TCv, UINT32, "SOFSCI3TCv", "SOFSCI3TC");    \
        EXT_STR_ITEM_INFO_LIM(ok, si, offset, struct_t, printerr, SOFSCI4TFM, UINT32, "SOFSCI4TFM", 3);              \
        EXT_STR_ITEM_INFO_ZZP(ok, si, offset, struct_t, printerr, SOFSCI4TFMI, UINT32, "SOFSCI4TFMI", "SOFSCI4TFM"); \
        EXT_STR_ITEM_INFO_ZZP(ok, si, offset, struct_t, printerr, SOFSCI4TFME, UINT32, "SOFSCI4TFME", "SOFSCI4TFM"); \
        EXT_STR_ITEM_INFO_LIM(ok, si, offset, struct_t, printerr, SOFSCI4TF, UINT32, "SOFSCI4TF", 300);              \
        EXT_STR_ITEM_INFO_ZZP(ok, si, offset, struct_t, printerr, SOFSCI4TFv, UINT32, "SOFSCI4TFv", "SOFSCI4TF");    \
        EXT_STR_ITEM_INFO_LIM(ok, si, offset, struct_t, printerr, SOFSCI4TCM, UINT32, "SOFSCI4TCM", 3);              \
        EXT_STR_ITEM_INFO_ZZP(ok, si, offset, struct_t, printerr, SOFSCI4TCMI, UINT32, "SOFSCI4TCMI", "SOFSCI4TCM"); \
        EXT_STR_ITEM_INFO_ZZP(ok, si, offset, struct_t, printerr, SOFSCI4TCME, UINT32, "SOFSCI4TCME", "SOFSCI4TCM"); \
        EXT_STR_ITEM_INFO_LIM(ok, si, offset, struct_t, printerr, SOFSCI4TC, UINT32, "SOFSCI4TC", 300);              \
        EXT_STR_ITEM_INFO_ZZP(ok, si, offset, struct_t, printerr, SOFSCI4TCv, UINT32, "SOFSCI4TCv", "SOFSCI4TC");    \
                                                                                                                     \
    } while (0);

/********************************************************
 *
 * For internal use by the network data reader:
 * (version checks, etc)
 */
#endif /*__GUARD_H101_SOFSCI_EXT_H101_SOFSCI_H__*/

/*******************************************************/
