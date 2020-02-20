/********************************************************
 *
 * Structure for ext_data_fetch_event() filling.
 *
 * Do not edit - automatically generated.
 */

#ifndef __GUARD_H101_SOFSCALERS_EXT_H101_H__
#define __GUARD_H101_SOFSCALERS_EXT_H101_H__

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

typedef struct EXT_STR_h101_SOFSCALERS_t
{
    /* RAW */
    uint32_t SOFSCALERS_UPSTREAM1 /* [-1,-1] */;
    uint32_t SOFSCALERS_UPSTREAM2 /* [-1,-1] */;
    uint32_t SOFSCALERS_UPSTREAM3 /* [-1,-1] */;
    uint32_t SOFSCALERS_UPSTREAM4 /* [-1,-1] */;
    uint32_t SOFSCALERS_UPSTREAM5 /* [-1,-1] */;
    uint32_t SOFSCALERS_UPSTREAM6 /* [-1,-1] */;
    uint32_t SOFSCALERS_UPSTREAM7 /* [-1,-1] */;
    uint32_t SOFSCALERS_TOFW1 /* [-1,-1] */;
    uint32_t SOFSCALERS_TOFW2 /* [-1,-1] */;
    uint32_t SOFSCALERS_TOFW3 /* [-1,-1] */;
    uint32_t SOFSCALERS_TOFW4 /* [-1,-1] */;
    uint32_t SOFSCALERS_TOFW5 /* [-1,-1] */;
    uint32_t SOFSCALERS_TOFW6 /* [-1,-1] */;
    uint32_t SOFSCALERS_TOFW7 /* [-1,-1] */;
    uint32_t SOFSCALERS_TOFW8 /* [-1,-1] */;
    uint32_t SOFSCALERS_TOFW9 /* [-1,-1] */;
    uint32_t SOFSCALERS_TOFW10 /* [-1,-1] */;
    uint32_t SOFSCALERS_TOFW11 /* [-1,-1] */;
    uint32_t SOFSCALERS_TOFW12 /* [-1,-1] */;
    uint32_t SOFSCALERS_TOFW13 /* [-1,-1] */;
    uint32_t SOFSCALERS_TOFW14 /* [-1,-1] */;
    uint32_t SOFSCALERS_TOFW15 /* [-1,-1] */;
    uint32_t SOFSCALERS_TOFW16 /* [-1,-1] */;
    uint32_t SOFSCALERS_TOFW17 /* [-1,-1] */;
    uint32_t SOFSCALERS_TOFW18 /* [-1,-1] */;
    uint32_t SOFSCALERS_TOFW19 /* [-1,-1] */;
    uint32_t SOFSCALERS_TOFW20 /* [-1,-1] */;
    uint32_t SOFSCALERS_TOFW21 /* [-1,-1] */;
    uint32_t SOFSCALERS_TOFW22 /* [-1,-1] */;
    uint32_t SOFSCALERS_TOFW23 /* [-1,-1] */;
    uint32_t SOFSCALERS_TOFW24 /* [-1,-1] */;
    uint32_t SOFSCALERS_TOFW25 /* [-1,-1] */;
    uint32_t SOFSCALERS_TOFW26 /* [-1,-1] */;
    uint32_t SOFSCALERS_TOFW27 /* [-1,-1] */;
    uint32_t SOFSCALERS_TOFW28 /* [-1,-1] */;
    uint32_t SOFSCALERS_TOFW29 /* [-1,-1] */;
    uint32_t SOFSCALERS_TOFW30 /* [-1,-1] */;
    uint32_t SOFSCALERS_TOFW31 /* [-1,-1] */;
    uint32_t SOFSCALERS_TOFW32 /* [-1,-1] */;
    uint32_t SOFSCALERS_TOFW33 /* [-1,-1] */;
    uint32_t SOFSCALERS_TOFW34 /* [-1,-1] */;
    uint32_t SOFSCALERS_TOFW35 /* [-1,-1] */;
    uint32_t SOFSCALERS_TOFW36 /* [-1,-1] */;
    uint32_t SOFSCALERS_TOFW37 /* [-1,-1] */;
    uint32_t SOFSCALERS_TOFW38 /* [-1,-1] */;
    uint32_t SOFSCALERS_TOFW39 /* [-1,-1] */;
    uint32_t SOFSCALERS_TOFW40 /* [-1,-1] */;
    uint32_t SOFSCALERS_TOFW41 /* [-1,-1] */;
    uint32_t SOFSCALERS_TOFW42 /* [-1,-1] */;
    uint32_t SOFSCALERS_TOFW43 /* [-1,-1] */;
    uint32_t SOFSCALERS_TOFW44 /* [-1,-1] */;
    uint32_t SOFSCALERS_TOFW45 /* [-1,-1] */;
    uint32_t SOFSCALERS_TOFW46 /* [-1,-1] */;
    uint32_t SOFSCALERS_TOFW47 /* [-1,-1] */;
    uint32_t SOFSCALERS_TOFW48 /* [-1,-1] */;
    uint32_t SOFSCALERS_TOFW49 /* [-1,-1] */;
    uint32_t SOFSCALERS_TOFW50 /* [-1,-1] */;
    uint32_t SOFSCALERS_TOFW51 /* [-1,-1] */;
    uint32_t SOFSCALERS_TOFW52 /* [-1,-1] */;
    uint32_t SOFSCALERS_TOFW53 /* [-1,-1] */;
    uint32_t SOFSCALERS_TOFW54 /* [-1,-1] */;
    uint32_t SOFSCALERS_TOFW55 /* [-1,-1] */;
    uint32_t SOFSCALERS_TOFW56 /* [-1,-1] */;

} EXT_STR_h101_SOFSCALERS;

/********************************************************
 *
 * Structure with multiple levels of arrays (partially)
 * recovered (recommended):
 */

typedef struct EXT_STR_h101_SOFSCALERS_onion_t
{
    /* RAW */
    uint32_t SOFSCALERS_UPSTREAM[7];
    uint32_t SOFSCALERS_TOFW[56];

} EXT_STR_h101_SOFSCALERS_onion;

/*******************************************************/

#define EXT_STR_h101_SOFSCALERS_ITEMS_INFO(ok, si, offset, struct_t, printerr)                                       \
    do                                                                                                               \
    {                                                                                                                \
        ok = 1;                                                                                                      \
        /* RAW */                                                                                                    \
        EXT_STR_ITEM_INFO(ok, si, offset, struct_t, printerr, SOFSCALERS_UPSTREAM1, UINT32, "SOFSCALERS_UPSTREAM1"); \
        EXT_STR_ITEM_INFO(ok, si, offset, struct_t, printerr, SOFSCALERS_UPSTREAM2, UINT32, "SOFSCALERS_UPSTREAM2"); \
        EXT_STR_ITEM_INFO(ok, si, offset, struct_t, printerr, SOFSCALERS_UPSTREAM3, UINT32, "SOFSCALERS_UPSTREAM3"); \
        EXT_STR_ITEM_INFO(ok, si, offset, struct_t, printerr, SOFSCALERS_UPSTREAM4, UINT32, "SOFSCALERS_UPSTREAM4"); \
        EXT_STR_ITEM_INFO(ok, si, offset, struct_t, printerr, SOFSCALERS_UPSTREAM5, UINT32, "SOFSCALERS_UPSTREAM5"); \
        EXT_STR_ITEM_INFO(ok, si, offset, struct_t, printerr, SOFSCALERS_UPSTREAM6, UINT32, "SOFSCALERS_UPSTREAM6"); \
        EXT_STR_ITEM_INFO(ok, si, offset, struct_t, printerr, SOFSCALERS_UPSTREAM7, UINT32, "SOFSCALERS_UPSTREAM7"); \
        EXT_STR_ITEM_INFO(ok, si, offset, struct_t, printerr, SOFSCALERS_TOFW1, UINT32, "SOFSCALERS_TOFW1");         \
        EXT_STR_ITEM_INFO(ok, si, offset, struct_t, printerr, SOFSCALERS_TOFW2, UINT32, "SOFSCALERS_TOFW2");         \
        EXT_STR_ITEM_INFO(ok, si, offset, struct_t, printerr, SOFSCALERS_TOFW3, UINT32, "SOFSCALERS_TOFW3");         \
        EXT_STR_ITEM_INFO(ok, si, offset, struct_t, printerr, SOFSCALERS_TOFW4, UINT32, "SOFSCALERS_TOFW4");         \
        EXT_STR_ITEM_INFO(ok, si, offset, struct_t, printerr, SOFSCALERS_TOFW5, UINT32, "SOFSCALERS_TOFW5");         \
        EXT_STR_ITEM_INFO(ok, si, offset, struct_t, printerr, SOFSCALERS_TOFW6, UINT32, "SOFSCALERS_TOFW6");         \
        EXT_STR_ITEM_INFO(ok, si, offset, struct_t, printerr, SOFSCALERS_TOFW7, UINT32, "SOFSCALERS_TOFW7");         \
        EXT_STR_ITEM_INFO(ok, si, offset, struct_t, printerr, SOFSCALERS_TOFW8, UINT32, "SOFSCALERS_TOFW8");         \
        EXT_STR_ITEM_INFO(ok, si, offset, struct_t, printerr, SOFSCALERS_TOFW9, UINT32, "SOFSCALERS_TOFW9");         \
        EXT_STR_ITEM_INFO(ok, si, offset, struct_t, printerr, SOFSCALERS_TOFW10, UINT32, "SOFSCALERS_TOFW10");       \
        EXT_STR_ITEM_INFO(ok, si, offset, struct_t, printerr, SOFSCALERS_TOFW11, UINT32, "SOFSCALERS_TOFW11");       \
        EXT_STR_ITEM_INFO(ok, si, offset, struct_t, printerr, SOFSCALERS_TOFW12, UINT32, "SOFSCALERS_TOFW12");       \
        EXT_STR_ITEM_INFO(ok, si, offset, struct_t, printerr, SOFSCALERS_TOFW13, UINT32, "SOFSCALERS_TOFW13");       \
        EXT_STR_ITEM_INFO(ok, si, offset, struct_t, printerr, SOFSCALERS_TOFW14, UINT32, "SOFSCALERS_TOFW14");       \
        EXT_STR_ITEM_INFO(ok, si, offset, struct_t, printerr, SOFSCALERS_TOFW15, UINT32, "SOFSCALERS_TOFW15");       \
        EXT_STR_ITEM_INFO(ok, si, offset, struct_t, printerr, SOFSCALERS_TOFW16, UINT32, "SOFSCALERS_TOFW16");       \
        EXT_STR_ITEM_INFO(ok, si, offset, struct_t, printerr, SOFSCALERS_TOFW17, UINT32, "SOFSCALERS_TOFW17");       \
        EXT_STR_ITEM_INFO(ok, si, offset, struct_t, printerr, SOFSCALERS_TOFW18, UINT32, "SOFSCALERS_TOFW18");       \
        EXT_STR_ITEM_INFO(ok, si, offset, struct_t, printerr, SOFSCALERS_TOFW19, UINT32, "SOFSCALERS_TOFW19");       \
        EXT_STR_ITEM_INFO(ok, si, offset, struct_t, printerr, SOFSCALERS_TOFW20, UINT32, "SOFSCALERS_TOFW20");       \
        EXT_STR_ITEM_INFO(ok, si, offset, struct_t, printerr, SOFSCALERS_TOFW21, UINT32, "SOFSCALERS_TOFW21");       \
        EXT_STR_ITEM_INFO(ok, si, offset, struct_t, printerr, SOFSCALERS_TOFW22, UINT32, "SOFSCALERS_TOFW22");       \
        EXT_STR_ITEM_INFO(ok, si, offset, struct_t, printerr, SOFSCALERS_TOFW23, UINT32, "SOFSCALERS_TOFW23");       \
        EXT_STR_ITEM_INFO(ok, si, offset, struct_t, printerr, SOFSCALERS_TOFW24, UINT32, "SOFSCALERS_TOFW24");       \
        EXT_STR_ITEM_INFO(ok, si, offset, struct_t, printerr, SOFSCALERS_TOFW25, UINT32, "SOFSCALERS_TOFW25");       \
        EXT_STR_ITEM_INFO(ok, si, offset, struct_t, printerr, SOFSCALERS_TOFW26, UINT32, "SOFSCALERS_TOFW26");       \
        EXT_STR_ITEM_INFO(ok, si, offset, struct_t, printerr, SOFSCALERS_TOFW27, UINT32, "SOFSCALERS_TOFW27");       \
        EXT_STR_ITEM_INFO(ok, si, offset, struct_t, printerr, SOFSCALERS_TOFW28, UINT32, "SOFSCALERS_TOFW28");       \
        EXT_STR_ITEM_INFO(ok, si, offset, struct_t, printerr, SOFSCALERS_TOFW29, UINT32, "SOFSCALERS_TOFW29");       \
        EXT_STR_ITEM_INFO(ok, si, offset, struct_t, printerr, SOFSCALERS_TOFW30, UINT32, "SOFSCALERS_TOFW30");       \
        EXT_STR_ITEM_INFO(ok, si, offset, struct_t, printerr, SOFSCALERS_TOFW31, UINT32, "SOFSCALERS_TOFW31");       \
        EXT_STR_ITEM_INFO(ok, si, offset, struct_t, printerr, SOFSCALERS_TOFW32, UINT32, "SOFSCALERS_TOFW32");       \
        EXT_STR_ITEM_INFO(ok, si, offset, struct_t, printerr, SOFSCALERS_TOFW33, UINT32, "SOFSCALERS_TOFW33");       \
        EXT_STR_ITEM_INFO(ok, si, offset, struct_t, printerr, SOFSCALERS_TOFW34, UINT32, "SOFSCALERS_TOFW34");       \
        EXT_STR_ITEM_INFO(ok, si, offset, struct_t, printerr, SOFSCALERS_TOFW35, UINT32, "SOFSCALERS_TOFW35");       \
        EXT_STR_ITEM_INFO(ok, si, offset, struct_t, printerr, SOFSCALERS_TOFW36, UINT32, "SOFSCALERS_TOFW36");       \
        EXT_STR_ITEM_INFO(ok, si, offset, struct_t, printerr, SOFSCALERS_TOFW37, UINT32, "SOFSCALERS_TOFW37");       \
        EXT_STR_ITEM_INFO(ok, si, offset, struct_t, printerr, SOFSCALERS_TOFW38, UINT32, "SOFSCALERS_TOFW38");       \
        EXT_STR_ITEM_INFO(ok, si, offset, struct_t, printerr, SOFSCALERS_TOFW39, UINT32, "SOFSCALERS_TOFW39");       \
        EXT_STR_ITEM_INFO(ok, si, offset, struct_t, printerr, SOFSCALERS_TOFW40, UINT32, "SOFSCALERS_TOFW40");       \
        EXT_STR_ITEM_INFO(ok, si, offset, struct_t, printerr, SOFSCALERS_TOFW41, UINT32, "SOFSCALERS_TOFW41");       \
        EXT_STR_ITEM_INFO(ok, si, offset, struct_t, printerr, SOFSCALERS_TOFW42, UINT32, "SOFSCALERS_TOFW42");       \
        EXT_STR_ITEM_INFO(ok, si, offset, struct_t, printerr, SOFSCALERS_TOFW43, UINT32, "SOFSCALERS_TOFW43");       \
        EXT_STR_ITEM_INFO(ok, si, offset, struct_t, printerr, SOFSCALERS_TOFW44, UINT32, "SOFSCALERS_TOFW44");       \
        EXT_STR_ITEM_INFO(ok, si, offset, struct_t, printerr, SOFSCALERS_TOFW45, UINT32, "SOFSCALERS_TOFW45");       \
        EXT_STR_ITEM_INFO(ok, si, offset, struct_t, printerr, SOFSCALERS_TOFW46, UINT32, "SOFSCALERS_TOFW46");       \
        EXT_STR_ITEM_INFO(ok, si, offset, struct_t, printerr, SOFSCALERS_TOFW47, UINT32, "SOFSCALERS_TOFW47");       \
        EXT_STR_ITEM_INFO(ok, si, offset, struct_t, printerr, SOFSCALERS_TOFW48, UINT32, "SOFSCALERS_TOFW48");       \
        EXT_STR_ITEM_INFO(ok, si, offset, struct_t, printerr, SOFSCALERS_TOFW49, UINT32, "SOFSCALERS_TOFW49");       \
        EXT_STR_ITEM_INFO(ok, si, offset, struct_t, printerr, SOFSCALERS_TOFW50, UINT32, "SOFSCALERS_TOFW50");       \
        EXT_STR_ITEM_INFO(ok, si, offset, struct_t, printerr, SOFSCALERS_TOFW51, UINT32, "SOFSCALERS_TOFW51");       \
        EXT_STR_ITEM_INFO(ok, si, offset, struct_t, printerr, SOFSCALERS_TOFW52, UINT32, "SOFSCALERS_TOFW52");       \
        EXT_STR_ITEM_INFO(ok, si, offset, struct_t, printerr, SOFSCALERS_TOFW53, UINT32, "SOFSCALERS_TOFW53");       \
        EXT_STR_ITEM_INFO(ok, si, offset, struct_t, printerr, SOFSCALERS_TOFW54, UINT32, "SOFSCALERS_TOFW54");       \
        EXT_STR_ITEM_INFO(ok, si, offset, struct_t, printerr, SOFSCALERS_TOFW55, UINT32, "SOFSCALERS_TOFW55");       \
        EXT_STR_ITEM_INFO(ok, si, offset, struct_t, printerr, SOFSCALERS_TOFW56, UINT32, "SOFSCALERS_TOFW56");       \
                                                                                                                     \
    } while (0);

/********************************************************
 *
 * For internal use by the network data reader:
 * (version checks, etc)
 */

#endif /*__GUARD_H101_SOFSCALERS_EXT_H101_H__*/

/*******************************************************/
