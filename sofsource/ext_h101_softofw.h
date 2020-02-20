/********************************************************
 *
 * Structure for ext_data_fetch_event() filling.
 *
 * Do not edit - automatically generated.
 */

#ifndef __GUARD_H101_SOFTOFW_EXT_H101_H__
#define __GUARD_H101_SOFTOFW_EXT_H101_H__

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

typedef struct EXT_STR_h101_SOFTOFW_t
{
    /* RAW */
    uint32_t SOFTOFW_P1TFM /* [1,2] */;
    uint32_t SOFTOFW_P1TFMI[2 EXT_STRUCT_CTRL(SOFTOFW_P1TFM)] /* [1,2] */;
    uint32_t SOFTOFW_P1TFME[2 EXT_STRUCT_CTRL(SOFTOFW_P1TFM)] /* [1,20] */;
    uint32_t SOFTOFW_P1TF /* [0,20] */;
    uint32_t SOFTOFW_P1TFv[20 EXT_STRUCT_CTRL(SOFTOFW_P1TF)] /* [0,65535] */;
    uint32_t SOFTOFW_P1TCM /* [1,2] */;
    uint32_t SOFTOFW_P1TCMI[2 EXT_STRUCT_CTRL(SOFTOFW_P1TCM)] /* [1,2] */;
    uint32_t SOFTOFW_P1TCME[2 EXT_STRUCT_CTRL(SOFTOFW_P1TCM)] /* [1,20] */;
    uint32_t SOFTOFW_P1TC /* [0,20] */;
    uint32_t SOFTOFW_P1TCv[20 EXT_STRUCT_CTRL(SOFTOFW_P1TC)] /* [0,65535] */;
    uint32_t SOFTOFW_P1E1 /* [0,65535] */;
    uint32_t SOFTOFW_P1E2 /* [0,65535] */;
    uint32_t SOFTOFW_P2TFM /* [1,2] */;
    uint32_t SOFTOFW_P2TFMI[2 EXT_STRUCT_CTRL(SOFTOFW_P2TFM)] /* [1,2] */;
    uint32_t SOFTOFW_P2TFME[2 EXT_STRUCT_CTRL(SOFTOFW_P2TFM)] /* [1,20] */;
    uint32_t SOFTOFW_P2TF /* [0,20] */;
    uint32_t SOFTOFW_P2TFv[20 EXT_STRUCT_CTRL(SOFTOFW_P2TF)] /* [0,65535] */;
    uint32_t SOFTOFW_P2TCM /* [1,2] */;
    uint32_t SOFTOFW_P2TCMI[2 EXT_STRUCT_CTRL(SOFTOFW_P2TCM)] /* [1,2] */;
    uint32_t SOFTOFW_P2TCME[2 EXT_STRUCT_CTRL(SOFTOFW_P2TCM)] /* [1,20] */;
    uint32_t SOFTOFW_P2TC /* [0,20] */;
    uint32_t SOFTOFW_P2TCv[20 EXT_STRUCT_CTRL(SOFTOFW_P2TC)] /* [0,65535] */;
    uint32_t SOFTOFW_P2E1 /* [0,65535] */;
    uint32_t SOFTOFW_P2E2 /* [0,65535] */;
    uint32_t SOFTOFW_P3TFM /* [1,2] */;
    uint32_t SOFTOFW_P3TFMI[2 EXT_STRUCT_CTRL(SOFTOFW_P3TFM)] /* [1,2] */;
    uint32_t SOFTOFW_P3TFME[2 EXT_STRUCT_CTRL(SOFTOFW_P3TFM)] /* [1,20] */;
    uint32_t SOFTOFW_P3TF /* [0,20] */;
    uint32_t SOFTOFW_P3TFv[20 EXT_STRUCT_CTRL(SOFTOFW_P3TF)] /* [0,65535] */;
    uint32_t SOFTOFW_P3TCM /* [1,2] */;
    uint32_t SOFTOFW_P3TCMI[2 EXT_STRUCT_CTRL(SOFTOFW_P3TCM)] /* [1,2] */;
    uint32_t SOFTOFW_P3TCME[2 EXT_STRUCT_CTRL(SOFTOFW_P3TCM)] /* [1,20] */;
    uint32_t SOFTOFW_P3TC /* [0,20] */;
    uint32_t SOFTOFW_P3TCv[20 EXT_STRUCT_CTRL(SOFTOFW_P3TC)] /* [0,65535] */;
    uint32_t SOFTOFW_P3E1 /* [0,65535] */;
    uint32_t SOFTOFW_P3E2 /* [0,65535] */;
    uint32_t SOFTOFW_P4TFM /* [1,2] */;
    uint32_t SOFTOFW_P4TFMI[2 EXT_STRUCT_CTRL(SOFTOFW_P4TFM)] /* [1,2] */;
    uint32_t SOFTOFW_P4TFME[2 EXT_STRUCT_CTRL(SOFTOFW_P4TFM)] /* [1,20] */;
    uint32_t SOFTOFW_P4TF /* [0,20] */;
    uint32_t SOFTOFW_P4TFv[20 EXT_STRUCT_CTRL(SOFTOFW_P4TF)] /* [0,65535] */;
    uint32_t SOFTOFW_P4TCM /* [1,2] */;
    uint32_t SOFTOFW_P4TCMI[2 EXT_STRUCT_CTRL(SOFTOFW_P4TCM)] /* [1,2] */;
    uint32_t SOFTOFW_P4TCME[2 EXT_STRUCT_CTRL(SOFTOFW_P4TCM)] /* [1,20] */;
    uint32_t SOFTOFW_P4TC /* [0,20] */;
    uint32_t SOFTOFW_P4TCv[20 EXT_STRUCT_CTRL(SOFTOFW_P4TC)] /* [0,65535] */;
    uint32_t SOFTOFW_P4E1 /* [0,65535] */;
    uint32_t SOFTOFW_P4E2 /* [0,65535] */;
    uint32_t SOFTOFW_P5TFM /* [1,2] */;
    uint32_t SOFTOFW_P5TFMI[2 EXT_STRUCT_CTRL(SOFTOFW_P5TFM)] /* [1,2] */;
    uint32_t SOFTOFW_P5TFME[2 EXT_STRUCT_CTRL(SOFTOFW_P5TFM)] /* [1,20] */;
    uint32_t SOFTOFW_P5TF /* [0,20] */;
    uint32_t SOFTOFW_P5TFv[20 EXT_STRUCT_CTRL(SOFTOFW_P5TF)] /* [0,65535] */;
    uint32_t SOFTOFW_P5TCM /* [1,2] */;
    uint32_t SOFTOFW_P5TCMI[2 EXT_STRUCT_CTRL(SOFTOFW_P5TCM)] /* [1,2] */;
    uint32_t SOFTOFW_P5TCME[2 EXT_STRUCT_CTRL(SOFTOFW_P5TCM)] /* [1,20] */;
    uint32_t SOFTOFW_P5TC /* [0,20] */;
    uint32_t SOFTOFW_P5TCv[20 EXT_STRUCT_CTRL(SOFTOFW_P5TC)] /* [0,65535] */;
    uint32_t SOFTOFW_P5E1 /* [0,65535] */;
    uint32_t SOFTOFW_P5E2 /* [0,65535] */;
    uint32_t SOFTOFW_P6TFM /* [1,2] */;
    uint32_t SOFTOFW_P6TFMI[2 EXT_STRUCT_CTRL(SOFTOFW_P6TFM)] /* [1,2] */;
    uint32_t SOFTOFW_P6TFME[2 EXT_STRUCT_CTRL(SOFTOFW_P6TFM)] /* [1,20] */;
    uint32_t SOFTOFW_P6TF /* [0,20] */;
    uint32_t SOFTOFW_P6TFv[20 EXT_STRUCT_CTRL(SOFTOFW_P6TF)] /* [0,65535] */;
    uint32_t SOFTOFW_P6TCM /* [1,2] */;
    uint32_t SOFTOFW_P6TCMI[2 EXT_STRUCT_CTRL(SOFTOFW_P6TCM)] /* [1,2] */;
    uint32_t SOFTOFW_P6TCME[2 EXT_STRUCT_CTRL(SOFTOFW_P6TCM)] /* [1,20] */;
    uint32_t SOFTOFW_P6TC /* [0,20] */;
    uint32_t SOFTOFW_P6TCv[20 EXT_STRUCT_CTRL(SOFTOFW_P6TC)] /* [0,65535] */;
    uint32_t SOFTOFW_P6E1 /* [0,65535] */;
    uint32_t SOFTOFW_P6E2 /* [0,65535] */;
    uint32_t SOFTOFW_P7TFM /* [1,2] */;
    uint32_t SOFTOFW_P7TFMI[2 EXT_STRUCT_CTRL(SOFTOFW_P7TFM)] /* [1,2] */;
    uint32_t SOFTOFW_P7TFME[2 EXT_STRUCT_CTRL(SOFTOFW_P7TFM)] /* [1,20] */;
    uint32_t SOFTOFW_P7TF /* [0,20] */;
    uint32_t SOFTOFW_P7TFv[20 EXT_STRUCT_CTRL(SOFTOFW_P7TF)] /* [0,65535] */;
    uint32_t SOFTOFW_P7TCM /* [1,2] */;
    uint32_t SOFTOFW_P7TCMI[2 EXT_STRUCT_CTRL(SOFTOFW_P7TCM)] /* [1,2] */;
    uint32_t SOFTOFW_P7TCME[2 EXT_STRUCT_CTRL(SOFTOFW_P7TCM)] /* [1,20] */;
    uint32_t SOFTOFW_P7TC /* [0,20] */;
    uint32_t SOFTOFW_P7TCv[20 EXT_STRUCT_CTRL(SOFTOFW_P7TC)] /* [0,65535] */;
    uint32_t SOFTOFW_P7E1 /* [0,65535] */;
    uint32_t SOFTOFW_P7E2 /* [0,65535] */;
    uint32_t SOFTOFW_P8TFM /* [1,2] */;
    uint32_t SOFTOFW_P8TFMI[2 EXT_STRUCT_CTRL(SOFTOFW_P8TFM)] /* [1,2] */;
    uint32_t SOFTOFW_P8TFME[2 EXT_STRUCT_CTRL(SOFTOFW_P8TFM)] /* [1,20] */;
    uint32_t SOFTOFW_P8TF /* [0,20] */;
    uint32_t SOFTOFW_P8TFv[20 EXT_STRUCT_CTRL(SOFTOFW_P8TF)] /* [0,65535] */;
    uint32_t SOFTOFW_P8TCM /* [1,2] */;
    uint32_t SOFTOFW_P8TCMI[2 EXT_STRUCT_CTRL(SOFTOFW_P8TCM)] /* [1,2] */;
    uint32_t SOFTOFW_P8TCME[2 EXT_STRUCT_CTRL(SOFTOFW_P8TCM)] /* [1,20] */;
    uint32_t SOFTOFW_P8TC /* [0,20] */;
    uint32_t SOFTOFW_P8TCv[20 EXT_STRUCT_CTRL(SOFTOFW_P8TC)] /* [0,65535] */;
    uint32_t SOFTOFW_P8E1 /* [0,65535] */;
    uint32_t SOFTOFW_P8E2 /* [0,65535] */;
    uint32_t SOFTOFW_P9TFM /* [1,2] */;
    uint32_t SOFTOFW_P9TFMI[2 EXT_STRUCT_CTRL(SOFTOFW_P9TFM)] /* [1,2] */;
    uint32_t SOFTOFW_P9TFME[2 EXT_STRUCT_CTRL(SOFTOFW_P9TFM)] /* [1,20] */;
    uint32_t SOFTOFW_P9TF /* [0,20] */;
    uint32_t SOFTOFW_P9TFv[20 EXT_STRUCT_CTRL(SOFTOFW_P9TF)] /* [0,65535] */;
    uint32_t SOFTOFW_P9TCM /* [1,2] */;
    uint32_t SOFTOFW_P9TCMI[2 EXT_STRUCT_CTRL(SOFTOFW_P9TCM)] /* [1,2] */;
    uint32_t SOFTOFW_P9TCME[2 EXT_STRUCT_CTRL(SOFTOFW_P9TCM)] /* [1,20] */;
    uint32_t SOFTOFW_P9TC /* [0,20] */;
    uint32_t SOFTOFW_P9TCv[20 EXT_STRUCT_CTRL(SOFTOFW_P9TC)] /* [0,65535] */;
    uint32_t SOFTOFW_P9E1 /* [0,65535] */;
    uint32_t SOFTOFW_P9E2 /* [0,65535] */;
    uint32_t SOFTOFW_P10TFM /* [1,2] */;
    uint32_t SOFTOFW_P10TFMI[2 EXT_STRUCT_CTRL(SOFTOFW_P10TFM)] /* [1,2] */;
    uint32_t SOFTOFW_P10TFME[2 EXT_STRUCT_CTRL(SOFTOFW_P10TFM)] /* [1,20] */;
    uint32_t SOFTOFW_P10TF /* [0,20] */;
    uint32_t SOFTOFW_P10TFv[20 EXT_STRUCT_CTRL(SOFTOFW_P10TF)] /* [0,65535] */;
    uint32_t SOFTOFW_P10TCM /* [1,2] */;
    uint32_t SOFTOFW_P10TCMI[2 EXT_STRUCT_CTRL(SOFTOFW_P10TCM)] /* [1,2] */;
    uint32_t SOFTOFW_P10TCME[2 EXT_STRUCT_CTRL(SOFTOFW_P10TCM)] /* [1,20] */;
    uint32_t SOFTOFW_P10TC /* [0,20] */;
    uint32_t SOFTOFW_P10TCv[20 EXT_STRUCT_CTRL(SOFTOFW_P10TC)] /* [0,65535] */;
    uint32_t SOFTOFW_P10E1 /* [0,65535] */;
    uint32_t SOFTOFW_P10E2 /* [0,65535] */;
    uint32_t SOFTOFW_P11TFM /* [1,2] */;
    uint32_t SOFTOFW_P11TFMI[2 EXT_STRUCT_CTRL(SOFTOFW_P11TFM)] /* [1,2] */;
    uint32_t SOFTOFW_P11TFME[2 EXT_STRUCT_CTRL(SOFTOFW_P11TFM)] /* [1,20] */;
    uint32_t SOFTOFW_P11TF /* [0,20] */;
    uint32_t SOFTOFW_P11TFv[20 EXT_STRUCT_CTRL(SOFTOFW_P11TF)] /* [0,65535] */;
    uint32_t SOFTOFW_P11TCM /* [1,2] */;
    uint32_t SOFTOFW_P11TCMI[2 EXT_STRUCT_CTRL(SOFTOFW_P11TCM)] /* [1,2] */;
    uint32_t SOFTOFW_P11TCME[2 EXT_STRUCT_CTRL(SOFTOFW_P11TCM)] /* [1,20] */;
    uint32_t SOFTOFW_P11TC /* [0,20] */;
    uint32_t SOFTOFW_P11TCv[20 EXT_STRUCT_CTRL(SOFTOFW_P11TC)] /* [0,65535] */;
    uint32_t SOFTOFW_P11E1 /* [0,65535] */;
    uint32_t SOFTOFW_P11E2 /* [0,65535] */;
    uint32_t SOFTOFW_P12TFM /* [1,2] */;
    uint32_t SOFTOFW_P12TFMI[2 EXT_STRUCT_CTRL(SOFTOFW_P12TFM)] /* [1,2] */;
    uint32_t SOFTOFW_P12TFME[2 EXT_STRUCT_CTRL(SOFTOFW_P12TFM)] /* [1,20] */;
    uint32_t SOFTOFW_P12TF /* [0,20] */;
    uint32_t SOFTOFW_P12TFv[20 EXT_STRUCT_CTRL(SOFTOFW_P12TF)] /* [0,65535] */;
    uint32_t SOFTOFW_P12TCM /* [1,2] */;
    uint32_t SOFTOFW_P12TCMI[2 EXT_STRUCT_CTRL(SOFTOFW_P12TCM)] /* [1,2] */;
    uint32_t SOFTOFW_P12TCME[2 EXT_STRUCT_CTRL(SOFTOFW_P12TCM)] /* [1,20] */;
    uint32_t SOFTOFW_P12TC /* [0,20] */;
    uint32_t SOFTOFW_P12TCv[20 EXT_STRUCT_CTRL(SOFTOFW_P12TC)] /* [0,65535] */;
    uint32_t SOFTOFW_P12E1 /* [0,65535] */;
    uint32_t SOFTOFW_P12E2 /* [0,65535] */;
    uint32_t SOFTOFW_P13TFM /* [1,2] */;
    uint32_t SOFTOFW_P13TFMI[2 EXT_STRUCT_CTRL(SOFTOFW_P13TFM)] /* [1,2] */;
    uint32_t SOFTOFW_P13TFME[2 EXT_STRUCT_CTRL(SOFTOFW_P13TFM)] /* [1,20] */;
    uint32_t SOFTOFW_P13TF /* [0,20] */;
    uint32_t SOFTOFW_P13TFv[20 EXT_STRUCT_CTRL(SOFTOFW_P13TF)] /* [0,65535] */;
    uint32_t SOFTOFW_P13TCM /* [1,2] */;
    uint32_t SOFTOFW_P13TCMI[2 EXT_STRUCT_CTRL(SOFTOFW_P13TCM)] /* [1,2] */;
    uint32_t SOFTOFW_P13TCME[2 EXT_STRUCT_CTRL(SOFTOFW_P13TCM)] /* [1,20] */;
    uint32_t SOFTOFW_P13TC /* [0,20] */;
    uint32_t SOFTOFW_P13TCv[20 EXT_STRUCT_CTRL(SOFTOFW_P13TC)] /* [0,65535] */;
    uint32_t SOFTOFW_P13E1 /* [0,65535] */;
    uint32_t SOFTOFW_P13E2 /* [0,65535] */;
    uint32_t SOFTOFW_P14TFM /* [1,2] */;
    uint32_t SOFTOFW_P14TFMI[2 EXT_STRUCT_CTRL(SOFTOFW_P14TFM)] /* [1,2] */;
    uint32_t SOFTOFW_P14TFME[2 EXT_STRUCT_CTRL(SOFTOFW_P14TFM)] /* [1,20] */;
    uint32_t SOFTOFW_P14TF /* [0,20] */;
    uint32_t SOFTOFW_P14TFv[20 EXT_STRUCT_CTRL(SOFTOFW_P14TF)] /* [0,65535] */;
    uint32_t SOFTOFW_P14TCM /* [1,2] */;
    uint32_t SOFTOFW_P14TCMI[2 EXT_STRUCT_CTRL(SOFTOFW_P14TCM)] /* [1,2] */;
    uint32_t SOFTOFW_P14TCME[2 EXT_STRUCT_CTRL(SOFTOFW_P14TCM)] /* [1,20] */;
    uint32_t SOFTOFW_P14TC /* [0,20] */;
    uint32_t SOFTOFW_P14TCv[20 EXT_STRUCT_CTRL(SOFTOFW_P14TC)] /* [0,65535] */;
    uint32_t SOFTOFW_P14E1 /* [0,65535] */;
    uint32_t SOFTOFW_P14E2 /* [0,65535] */;
    uint32_t SOFTOFW_P15TFM /* [1,2] */;
    uint32_t SOFTOFW_P15TFMI[2 EXT_STRUCT_CTRL(SOFTOFW_P15TFM)] /* [1,2] */;
    uint32_t SOFTOFW_P15TFME[2 EXT_STRUCT_CTRL(SOFTOFW_P15TFM)] /* [1,20] */;
    uint32_t SOFTOFW_P15TF /* [0,20] */;
    uint32_t SOFTOFW_P15TFv[20 EXT_STRUCT_CTRL(SOFTOFW_P15TF)] /* [0,65535] */;
    uint32_t SOFTOFW_P15TCM /* [1,2] */;
    uint32_t SOFTOFW_P15TCMI[2 EXT_STRUCT_CTRL(SOFTOFW_P15TCM)] /* [1,2] */;
    uint32_t SOFTOFW_P15TCME[2 EXT_STRUCT_CTRL(SOFTOFW_P15TCM)] /* [1,20] */;
    uint32_t SOFTOFW_P15TC /* [0,20] */;
    uint32_t SOFTOFW_P15TCv[20 EXT_STRUCT_CTRL(SOFTOFW_P15TC)] /* [0,65535] */;
    uint32_t SOFTOFW_P15E1 /* [0,65535] */;
    uint32_t SOFTOFW_P15E2 /* [0,65535] */;
    uint32_t SOFTOFW_P16TFM /* [1,2] */;
    uint32_t SOFTOFW_P16TFMI[2 EXT_STRUCT_CTRL(SOFTOFW_P16TFM)] /* [1,2] */;
    uint32_t SOFTOFW_P16TFME[2 EXT_STRUCT_CTRL(SOFTOFW_P16TFM)] /* [1,20] */;
    uint32_t SOFTOFW_P16TF /* [0,20] */;
    uint32_t SOFTOFW_P16TFv[20 EXT_STRUCT_CTRL(SOFTOFW_P16TF)] /* [0,65535] */;
    uint32_t SOFTOFW_P16TCM /* [1,2] */;
    uint32_t SOFTOFW_P16TCMI[2 EXT_STRUCT_CTRL(SOFTOFW_P16TCM)] /* [1,2] */;
    uint32_t SOFTOFW_P16TCME[2 EXT_STRUCT_CTRL(SOFTOFW_P16TCM)] /* [1,20] */;
    uint32_t SOFTOFW_P16TC /* [0,20] */;
    uint32_t SOFTOFW_P16TCv[20 EXT_STRUCT_CTRL(SOFTOFW_P16TC)] /* [0,65535] */;
    uint32_t SOFTOFW_P16E1 /* [0,65535] */;
    uint32_t SOFTOFW_P16E2 /* [0,65535] */;
    uint32_t SOFTOFW_P17TFM /* [1,2] */;
    uint32_t SOFTOFW_P17TFMI[2 EXT_STRUCT_CTRL(SOFTOFW_P17TFM)] /* [1,2] */;
    uint32_t SOFTOFW_P17TFME[2 EXT_STRUCT_CTRL(SOFTOFW_P17TFM)] /* [1,20] */;
    uint32_t SOFTOFW_P17TF /* [0,20] */;
    uint32_t SOFTOFW_P17TFv[20 EXT_STRUCT_CTRL(SOFTOFW_P17TF)] /* [0,65535] */;
    uint32_t SOFTOFW_P17TCM /* [1,2] */;
    uint32_t SOFTOFW_P17TCMI[2 EXT_STRUCT_CTRL(SOFTOFW_P17TCM)] /* [1,2] */;
    uint32_t SOFTOFW_P17TCME[2 EXT_STRUCT_CTRL(SOFTOFW_P17TCM)] /* [1,20] */;
    uint32_t SOFTOFW_P17TC /* [0,20] */;
    uint32_t SOFTOFW_P17TCv[20 EXT_STRUCT_CTRL(SOFTOFW_P17TC)] /* [0,65535] */;
    uint32_t SOFTOFW_P17E1 /* [0,65535] */;
    uint32_t SOFTOFW_P17E2 /* [0,65535] */;
    uint32_t SOFTOFW_P18TFM /* [1,2] */;
    uint32_t SOFTOFW_P18TFMI[2 EXT_STRUCT_CTRL(SOFTOFW_P18TFM)] /* [1,2] */;
    uint32_t SOFTOFW_P18TFME[2 EXT_STRUCT_CTRL(SOFTOFW_P18TFM)] /* [1,20] */;
    uint32_t SOFTOFW_P18TF /* [0,20] */;
    uint32_t SOFTOFW_P18TFv[20 EXT_STRUCT_CTRL(SOFTOFW_P18TF)] /* [0,65535] */;
    uint32_t SOFTOFW_P18TCM /* [1,2] */;
    uint32_t SOFTOFW_P18TCMI[2 EXT_STRUCT_CTRL(SOFTOFW_P18TCM)] /* [1,2] */;
    uint32_t SOFTOFW_P18TCME[2 EXT_STRUCT_CTRL(SOFTOFW_P18TCM)] /* [1,20] */;
    uint32_t SOFTOFW_P18TC /* [0,20] */;
    uint32_t SOFTOFW_P18TCv[20 EXT_STRUCT_CTRL(SOFTOFW_P18TC)] /* [0,65535] */;
    uint32_t SOFTOFW_P18E1 /* [0,65535] */;
    uint32_t SOFTOFW_P18E2 /* [0,65535] */;
    uint32_t SOFTOFW_P19TFM /* [1,2] */;
    uint32_t SOFTOFW_P19TFMI[2 EXT_STRUCT_CTRL(SOFTOFW_P19TFM)] /* [1,2] */;
    uint32_t SOFTOFW_P19TFME[2 EXT_STRUCT_CTRL(SOFTOFW_P19TFM)] /* [1,20] */;
    uint32_t SOFTOFW_P19TF /* [0,20] */;
    uint32_t SOFTOFW_P19TFv[20 EXT_STRUCT_CTRL(SOFTOFW_P19TF)] /* [0,65535] */;
    uint32_t SOFTOFW_P19TCM /* [1,2] */;
    uint32_t SOFTOFW_P19TCMI[2 EXT_STRUCT_CTRL(SOFTOFW_P19TCM)] /* [1,2] */;
    uint32_t SOFTOFW_P19TCME[2 EXT_STRUCT_CTRL(SOFTOFW_P19TCM)] /* [1,20] */;
    uint32_t SOFTOFW_P19TC /* [0,20] */;
    uint32_t SOFTOFW_P19TCv[20 EXT_STRUCT_CTRL(SOFTOFW_P19TC)] /* [0,65535] */;
    uint32_t SOFTOFW_P19E1 /* [0,65535] */;
    uint32_t SOFTOFW_P19E2 /* [0,65535] */;
    uint32_t SOFTOFW_P20TFM /* [1,2] */;
    uint32_t SOFTOFW_P20TFMI[2 EXT_STRUCT_CTRL(SOFTOFW_P20TFM)] /* [1,2] */;
    uint32_t SOFTOFW_P20TFME[2 EXT_STRUCT_CTRL(SOFTOFW_P20TFM)] /* [1,20] */;
    uint32_t SOFTOFW_P20TF /* [0,20] */;
    uint32_t SOFTOFW_P20TFv[20 EXT_STRUCT_CTRL(SOFTOFW_P20TF)] /* [0,65535] */;
    uint32_t SOFTOFW_P20TCM /* [1,2] */;
    uint32_t SOFTOFW_P20TCMI[2 EXT_STRUCT_CTRL(SOFTOFW_P20TCM)] /* [1,2] */;
    uint32_t SOFTOFW_P20TCME[2 EXT_STRUCT_CTRL(SOFTOFW_P20TCM)] /* [1,20] */;
    uint32_t SOFTOFW_P20TC /* [0,20] */;
    uint32_t SOFTOFW_P20TCv[20 EXT_STRUCT_CTRL(SOFTOFW_P20TC)] /* [0,65535] */;
    uint32_t SOFTOFW_P20E1 /* [0,65535] */;
    uint32_t SOFTOFW_P20E2 /* [0,65535] */;
    uint32_t SOFTOFW_P21TFM /* [1,2] */;
    uint32_t SOFTOFW_P21TFMI[2 EXT_STRUCT_CTRL(SOFTOFW_P21TFM)] /* [1,2] */;
    uint32_t SOFTOFW_P21TFME[2 EXT_STRUCT_CTRL(SOFTOFW_P21TFM)] /* [1,20] */;
    uint32_t SOFTOFW_P21TF /* [0,20] */;
    uint32_t SOFTOFW_P21TFv[20 EXT_STRUCT_CTRL(SOFTOFW_P21TF)] /* [0,65535] */;
    uint32_t SOFTOFW_P21TCM /* [1,2] */;
    uint32_t SOFTOFW_P21TCMI[2 EXT_STRUCT_CTRL(SOFTOFW_P21TCM)] /* [1,2] */;
    uint32_t SOFTOFW_P21TCME[2 EXT_STRUCT_CTRL(SOFTOFW_P21TCM)] /* [1,20] */;
    uint32_t SOFTOFW_P21TC /* [0,20] */;
    uint32_t SOFTOFW_P21TCv[20 EXT_STRUCT_CTRL(SOFTOFW_P21TC)] /* [0,65535] */;
    uint32_t SOFTOFW_P21E1 /* [0,65535] */;
    uint32_t SOFTOFW_P21E2 /* [0,65535] */;
    uint32_t SOFTOFW_P22TFM /* [1,2] */;
    uint32_t SOFTOFW_P22TFMI[2 EXT_STRUCT_CTRL(SOFTOFW_P22TFM)] /* [1,2] */;
    uint32_t SOFTOFW_P22TFME[2 EXT_STRUCT_CTRL(SOFTOFW_P22TFM)] /* [1,20] */;
    uint32_t SOFTOFW_P22TF /* [0,20] */;
    uint32_t SOFTOFW_P22TFv[20 EXT_STRUCT_CTRL(SOFTOFW_P22TF)] /* [0,65535] */;
    uint32_t SOFTOFW_P22TCM /* [1,2] */;
    uint32_t SOFTOFW_P22TCMI[2 EXT_STRUCT_CTRL(SOFTOFW_P22TCM)] /* [1,2] */;
    uint32_t SOFTOFW_P22TCME[2 EXT_STRUCT_CTRL(SOFTOFW_P22TCM)] /* [1,20] */;
    uint32_t SOFTOFW_P22TC /* [0,20] */;
    uint32_t SOFTOFW_P22TCv[20 EXT_STRUCT_CTRL(SOFTOFW_P22TC)] /* [0,65535] */;
    uint32_t SOFTOFW_P22E1 /* [0,65535] */;
    uint32_t SOFTOFW_P22E2 /* [0,65535] */;
    uint32_t SOFTOFW_P23TFM /* [1,2] */;
    uint32_t SOFTOFW_P23TFMI[2 EXT_STRUCT_CTRL(SOFTOFW_P23TFM)] /* [1,2] */;
    uint32_t SOFTOFW_P23TFME[2 EXT_STRUCT_CTRL(SOFTOFW_P23TFM)] /* [1,20] */;
    uint32_t SOFTOFW_P23TF /* [0,20] */;
    uint32_t SOFTOFW_P23TFv[20 EXT_STRUCT_CTRL(SOFTOFW_P23TF)] /* [0,65535] */;
    uint32_t SOFTOFW_P23TCM /* [1,2] */;
    uint32_t SOFTOFW_P23TCMI[2 EXT_STRUCT_CTRL(SOFTOFW_P23TCM)] /* [1,2] */;
    uint32_t SOFTOFW_P23TCME[2 EXT_STRUCT_CTRL(SOFTOFW_P23TCM)] /* [1,20] */;
    uint32_t SOFTOFW_P23TC /* [0,20] */;
    uint32_t SOFTOFW_P23TCv[20 EXT_STRUCT_CTRL(SOFTOFW_P23TC)] /* [0,65535] */;
    uint32_t SOFTOFW_P23E1 /* [0,65535] */;
    uint32_t SOFTOFW_P23E2 /* [0,65535] */;
    uint32_t SOFTOFW_P24TFM /* [1,2] */;
    uint32_t SOFTOFW_P24TFMI[2 EXT_STRUCT_CTRL(SOFTOFW_P24TFM)] /* [1,2] */;
    uint32_t SOFTOFW_P24TFME[2 EXT_STRUCT_CTRL(SOFTOFW_P24TFM)] /* [1,20] */;
    uint32_t SOFTOFW_P24TF /* [0,20] */;
    uint32_t SOFTOFW_P24TFv[20 EXT_STRUCT_CTRL(SOFTOFW_P24TF)] /* [0,65535] */;
    uint32_t SOFTOFW_P24TCM /* [1,2] */;
    uint32_t SOFTOFW_P24TCMI[2 EXT_STRUCT_CTRL(SOFTOFW_P24TCM)] /* [1,2] */;
    uint32_t SOFTOFW_P24TCME[2 EXT_STRUCT_CTRL(SOFTOFW_P24TCM)] /* [1,20] */;
    uint32_t SOFTOFW_P24TC /* [0,20] */;
    uint32_t SOFTOFW_P24TCv[20 EXT_STRUCT_CTRL(SOFTOFW_P24TC)] /* [0,65535] */;
    uint32_t SOFTOFW_P24E1 /* [0,65535] */;
    uint32_t SOFTOFW_P24E2 /* [0,65535] */;
    uint32_t SOFTOFW_P25TFM /* [1,2] */;
    uint32_t SOFTOFW_P25TFMI[2 EXT_STRUCT_CTRL(SOFTOFW_P25TFM)] /* [1,2] */;
    uint32_t SOFTOFW_P25TFME[2 EXT_STRUCT_CTRL(SOFTOFW_P25TFM)] /* [1,20] */;
    uint32_t SOFTOFW_P25TF /* [0,20] */;
    uint32_t SOFTOFW_P25TFv[20 EXT_STRUCT_CTRL(SOFTOFW_P25TF)] /* [0,65535] */;
    uint32_t SOFTOFW_P25TCM /* [1,2] */;
    uint32_t SOFTOFW_P25TCMI[2 EXT_STRUCT_CTRL(SOFTOFW_P25TCM)] /* [1,2] */;
    uint32_t SOFTOFW_P25TCME[2 EXT_STRUCT_CTRL(SOFTOFW_P25TCM)] /* [1,20] */;
    uint32_t SOFTOFW_P25TC /* [0,20] */;
    uint32_t SOFTOFW_P25TCv[20 EXT_STRUCT_CTRL(SOFTOFW_P25TC)] /* [0,65535] */;
    uint32_t SOFTOFW_P25E1 /* [0,65535] */;
    uint32_t SOFTOFW_P25E2 /* [0,65535] */;
    uint32_t SOFTOFW_P26TFM /* [1,2] */;
    uint32_t SOFTOFW_P26TFMI[2 EXT_STRUCT_CTRL(SOFTOFW_P26TFM)] /* [1,2] */;
    uint32_t SOFTOFW_P26TFME[2 EXT_STRUCT_CTRL(SOFTOFW_P26TFM)] /* [1,20] */;
    uint32_t SOFTOFW_P26TF /* [0,20] */;
    uint32_t SOFTOFW_P26TFv[20 EXT_STRUCT_CTRL(SOFTOFW_P26TF)] /* [0,65535] */;
    uint32_t SOFTOFW_P26TCM /* [1,2] */;
    uint32_t SOFTOFW_P26TCMI[2 EXT_STRUCT_CTRL(SOFTOFW_P26TCM)] /* [1,2] */;
    uint32_t SOFTOFW_P26TCME[2 EXT_STRUCT_CTRL(SOFTOFW_P26TCM)] /* [1,20] */;
    uint32_t SOFTOFW_P26TC /* [0,20] */;
    uint32_t SOFTOFW_P26TCv[20 EXT_STRUCT_CTRL(SOFTOFW_P26TC)] /* [0,65535] */;
    uint32_t SOFTOFW_P26E1 /* [0,65535] */;
    uint32_t SOFTOFW_P26E2 /* [0,65535] */;
    uint32_t SOFTOFW_P27TFM /* [1,2] */;
    uint32_t SOFTOFW_P27TFMI[2 EXT_STRUCT_CTRL(SOFTOFW_P27TFM)] /* [1,2] */;
    uint32_t SOFTOFW_P27TFME[2 EXT_STRUCT_CTRL(SOFTOFW_P27TFM)] /* [1,20] */;
    uint32_t SOFTOFW_P27TF /* [0,20] */;
    uint32_t SOFTOFW_P27TFv[20 EXT_STRUCT_CTRL(SOFTOFW_P27TF)] /* [0,65535] */;
    uint32_t SOFTOFW_P27TCM /* [1,2] */;
    uint32_t SOFTOFW_P27TCMI[2 EXT_STRUCT_CTRL(SOFTOFW_P27TCM)] /* [1,2] */;
    uint32_t SOFTOFW_P27TCME[2 EXT_STRUCT_CTRL(SOFTOFW_P27TCM)] /* [1,20] */;
    uint32_t SOFTOFW_P27TC /* [0,20] */;
    uint32_t SOFTOFW_P27TCv[20 EXT_STRUCT_CTRL(SOFTOFW_P27TC)] /* [0,65535] */;
    uint32_t SOFTOFW_P27E1 /* [0,65535] */;
    uint32_t SOFTOFW_P27E2 /* [0,65535] */;
    uint32_t SOFTOFW_P28TFM /* [1,2] */;
    uint32_t SOFTOFW_P28TFMI[2 EXT_STRUCT_CTRL(SOFTOFW_P28TFM)] /* [1,2] */;
    uint32_t SOFTOFW_P28TFME[2 EXT_STRUCT_CTRL(SOFTOFW_P28TFM)] /* [1,20] */;
    uint32_t SOFTOFW_P28TF /* [0,20] */;
    uint32_t SOFTOFW_P28TFv[20 EXT_STRUCT_CTRL(SOFTOFW_P28TF)] /* [0,65535] */;
    uint32_t SOFTOFW_P28TCM /* [1,2] */;
    uint32_t SOFTOFW_P28TCMI[2 EXT_STRUCT_CTRL(SOFTOFW_P28TCM)] /* [1,2] */;
    uint32_t SOFTOFW_P28TCME[2 EXT_STRUCT_CTRL(SOFTOFW_P28TCM)] /* [1,20] */;
    uint32_t SOFTOFW_P28TC /* [0,20] */;
    uint32_t SOFTOFW_P28TCv[20 EXT_STRUCT_CTRL(SOFTOFW_P28TC)] /* [0,65535] */;
    uint32_t SOFTOFW_P28E1 /* [0,65535] */;
    uint32_t SOFTOFW_P28E2 /* [0,65535] */;

} EXT_STR_h101_SOFTOFW;

/********************************************************
 *
 * Structure with multiple levels of arrays (partially)
 * recovered (recommended):
 */

typedef struct EXT_STR_h101_SOFTOFW_onion_t
{
    /* RAW */
    struct
    {
        uint32_t TFM;
        uint32_t TFMI[2 /* TFM */];
        uint32_t TFME[2 /* TFM */];
        uint32_t TF;
        uint32_t TFv[20 /* TF */];
        uint32_t TCM;
        uint32_t TCMI[2 /* TCM */];
        uint32_t TCME[2 /* TCM */];
        uint32_t TC;
        uint32_t TCv[20 /* TC */];
        uint32_t E[2];
    } SOFTOFW_P[28];

} EXT_STR_h101_SOFTOFW_onion;

/*******************************************************/

#define EXT_STR_h101_SOFTOFW_ITEMS_INFO(ok, si, offset, struct_t, printerr)                                       \
    do                                                                                                            \
    {                                                                                                             \
        ok = 1;                                                                                                   \
        /* RAW */                                                                                                 \
        EXT_STR_ITEM_INFO_LIM(ok, si, offset, struct_t, printerr, SOFTOFW_P1TFM, UINT32, "SOFTOFW_P1TFM", 2);     \
        EXT_STR_ITEM_INFO_ZZP(                                                                                    \
            ok, si, offset, struct_t, printerr, SOFTOFW_P1TFMI, UINT32, "SOFTOFW_P1TFMI", "SOFTOFW_P1TFM");       \
        EXT_STR_ITEM_INFO_ZZP(                                                                                    \
            ok, si, offset, struct_t, printerr, SOFTOFW_P1TFME, UINT32, "SOFTOFW_P1TFME", "SOFTOFW_P1TFM");       \
        EXT_STR_ITEM_INFO_LIM(ok, si, offset, struct_t, printerr, SOFTOFW_P1TF, UINT32, "SOFTOFW_P1TF", 20);      \
        EXT_STR_ITEM_INFO_ZZP(                                                                                    \
            ok, si, offset, struct_t, printerr, SOFTOFW_P1TFv, UINT32, "SOFTOFW_P1TFv", "SOFTOFW_P1TF");          \
        EXT_STR_ITEM_INFO_LIM(ok, si, offset, struct_t, printerr, SOFTOFW_P1TCM, UINT32, "SOFTOFW_P1TCM", 2);     \
        EXT_STR_ITEM_INFO_ZZP(                                                                                    \
            ok, si, offset, struct_t, printerr, SOFTOFW_P1TCMI, UINT32, "SOFTOFW_P1TCMI", "SOFTOFW_P1TCM");       \
        EXT_STR_ITEM_INFO_ZZP(                                                                                    \
            ok, si, offset, struct_t, printerr, SOFTOFW_P1TCME, UINT32, "SOFTOFW_P1TCME", "SOFTOFW_P1TCM");       \
        EXT_STR_ITEM_INFO_LIM(ok, si, offset, struct_t, printerr, SOFTOFW_P1TC, UINT32, "SOFTOFW_P1TC", 20);      \
        EXT_STR_ITEM_INFO_ZZP(                                                                                    \
            ok, si, offset, struct_t, printerr, SOFTOFW_P1TCv, UINT32, "SOFTOFW_P1TCv", "SOFTOFW_P1TC");          \
        EXT_STR_ITEM_INFO_LIM(ok, si, offset, struct_t, printerr, SOFTOFW_P1E1, UINT32, "SOFTOFW_P1E1", 65535);   \
        EXT_STR_ITEM_INFO_LIM(ok, si, offset, struct_t, printerr, SOFTOFW_P1E2, UINT32, "SOFTOFW_P1E2", 65535);   \
        EXT_STR_ITEM_INFO_LIM(ok, si, offset, struct_t, printerr, SOFTOFW_P2TFM, UINT32, "SOFTOFW_P2TFM", 2);     \
        EXT_STR_ITEM_INFO_ZZP(                                                                                    \
            ok, si, offset, struct_t, printerr, SOFTOFW_P2TFMI, UINT32, "SOFTOFW_P2TFMI", "SOFTOFW_P2TFM");       \
        EXT_STR_ITEM_INFO_ZZP(                                                                                    \
            ok, si, offset, struct_t, printerr, SOFTOFW_P2TFME, UINT32, "SOFTOFW_P2TFME", "SOFTOFW_P2TFM");       \
        EXT_STR_ITEM_INFO_LIM(ok, si, offset, struct_t, printerr, SOFTOFW_P2TF, UINT32, "SOFTOFW_P2TF", 20);      \
        EXT_STR_ITEM_INFO_ZZP(                                                                                    \
            ok, si, offset, struct_t, printerr, SOFTOFW_P2TFv, UINT32, "SOFTOFW_P2TFv", "SOFTOFW_P2TF");          \
        EXT_STR_ITEM_INFO_LIM(ok, si, offset, struct_t, printerr, SOFTOFW_P2TCM, UINT32, "SOFTOFW_P2TCM", 2);     \
        EXT_STR_ITEM_INFO_ZZP(                                                                                    \
            ok, si, offset, struct_t, printerr, SOFTOFW_P2TCMI, UINT32, "SOFTOFW_P2TCMI", "SOFTOFW_P2TCM");       \
        EXT_STR_ITEM_INFO_ZZP(                                                                                    \
            ok, si, offset, struct_t, printerr, SOFTOFW_P2TCME, UINT32, "SOFTOFW_P2TCME", "SOFTOFW_P2TCM");       \
        EXT_STR_ITEM_INFO_LIM(ok, si, offset, struct_t, printerr, SOFTOFW_P2TC, UINT32, "SOFTOFW_P2TC", 20);      \
        EXT_STR_ITEM_INFO_ZZP(                                                                                    \
            ok, si, offset, struct_t, printerr, SOFTOFW_P2TCv, UINT32, "SOFTOFW_P2TCv", "SOFTOFW_P2TC");          \
        EXT_STR_ITEM_INFO_LIM(ok, si, offset, struct_t, printerr, SOFTOFW_P2E1, UINT32, "SOFTOFW_P2E1", 65535);   \
        EXT_STR_ITEM_INFO_LIM(ok, si, offset, struct_t, printerr, SOFTOFW_P2E2, UINT32, "SOFTOFW_P2E2", 65535);   \
        EXT_STR_ITEM_INFO_LIM(ok, si, offset, struct_t, printerr, SOFTOFW_P3TFM, UINT32, "SOFTOFW_P3TFM", 2);     \
        EXT_STR_ITEM_INFO_ZZP(                                                                                    \
            ok, si, offset, struct_t, printerr, SOFTOFW_P3TFMI, UINT32, "SOFTOFW_P3TFMI", "SOFTOFW_P3TFM");       \
        EXT_STR_ITEM_INFO_ZZP(                                                                                    \
            ok, si, offset, struct_t, printerr, SOFTOFW_P3TFME, UINT32, "SOFTOFW_P3TFME", "SOFTOFW_P3TFM");       \
        EXT_STR_ITEM_INFO_LIM(ok, si, offset, struct_t, printerr, SOFTOFW_P3TF, UINT32, "SOFTOFW_P3TF", 20);      \
        EXT_STR_ITEM_INFO_ZZP(                                                                                    \
            ok, si, offset, struct_t, printerr, SOFTOFW_P3TFv, UINT32, "SOFTOFW_P3TFv", "SOFTOFW_P3TF");          \
        EXT_STR_ITEM_INFO_LIM(ok, si, offset, struct_t, printerr, SOFTOFW_P3TCM, UINT32, "SOFTOFW_P3TCM", 2);     \
        EXT_STR_ITEM_INFO_ZZP(                                                                                    \
            ok, si, offset, struct_t, printerr, SOFTOFW_P3TCMI, UINT32, "SOFTOFW_P3TCMI", "SOFTOFW_P3TCM");       \
        EXT_STR_ITEM_INFO_ZZP(                                                                                    \
            ok, si, offset, struct_t, printerr, SOFTOFW_P3TCME, UINT32, "SOFTOFW_P3TCME", "SOFTOFW_P3TCM");       \
        EXT_STR_ITEM_INFO_LIM(ok, si, offset, struct_t, printerr, SOFTOFW_P3TC, UINT32, "SOFTOFW_P3TC", 20);      \
        EXT_STR_ITEM_INFO_ZZP(                                                                                    \
            ok, si, offset, struct_t, printerr, SOFTOFW_P3TCv, UINT32, "SOFTOFW_P3TCv", "SOFTOFW_P3TC");          \
        EXT_STR_ITEM_INFO_LIM(ok, si, offset, struct_t, printerr, SOFTOFW_P3E1, UINT32, "SOFTOFW_P3E1", 65535);   \
        EXT_STR_ITEM_INFO_LIM(ok, si, offset, struct_t, printerr, SOFTOFW_P3E2, UINT32, "SOFTOFW_P3E2", 65535);   \
        EXT_STR_ITEM_INFO_LIM(ok, si, offset, struct_t, printerr, SOFTOFW_P4TFM, UINT32, "SOFTOFW_P4TFM", 2);     \
        EXT_STR_ITEM_INFO_ZZP(                                                                                    \
            ok, si, offset, struct_t, printerr, SOFTOFW_P4TFMI, UINT32, "SOFTOFW_P4TFMI", "SOFTOFW_P4TFM");       \
        EXT_STR_ITEM_INFO_ZZP(                                                                                    \
            ok, si, offset, struct_t, printerr, SOFTOFW_P4TFME, UINT32, "SOFTOFW_P4TFME", "SOFTOFW_P4TFM");       \
        EXT_STR_ITEM_INFO_LIM(ok, si, offset, struct_t, printerr, SOFTOFW_P4TF, UINT32, "SOFTOFW_P4TF", 20);      \
        EXT_STR_ITEM_INFO_ZZP(                                                                                    \
            ok, si, offset, struct_t, printerr, SOFTOFW_P4TFv, UINT32, "SOFTOFW_P4TFv", "SOFTOFW_P4TF");          \
        EXT_STR_ITEM_INFO_LIM(ok, si, offset, struct_t, printerr, SOFTOFW_P4TCM, UINT32, "SOFTOFW_P4TCM", 2);     \
        EXT_STR_ITEM_INFO_ZZP(                                                                                    \
            ok, si, offset, struct_t, printerr, SOFTOFW_P4TCMI, UINT32, "SOFTOFW_P4TCMI", "SOFTOFW_P4TCM");       \
        EXT_STR_ITEM_INFO_ZZP(                                                                                    \
            ok, si, offset, struct_t, printerr, SOFTOFW_P4TCME, UINT32, "SOFTOFW_P4TCME", "SOFTOFW_P4TCM");       \
        EXT_STR_ITEM_INFO_LIM(ok, si, offset, struct_t, printerr, SOFTOFW_P4TC, UINT32, "SOFTOFW_P4TC", 20);      \
        EXT_STR_ITEM_INFO_ZZP(                                                                                    \
            ok, si, offset, struct_t, printerr, SOFTOFW_P4TCv, UINT32, "SOFTOFW_P4TCv", "SOFTOFW_P4TC");          \
        EXT_STR_ITEM_INFO_LIM(ok, si, offset, struct_t, printerr, SOFTOFW_P4E1, UINT32, "SOFTOFW_P4E1", 65535);   \
        EXT_STR_ITEM_INFO_LIM(ok, si, offset, struct_t, printerr, SOFTOFW_P4E2, UINT32, "SOFTOFW_P4E2", 65535);   \
        EXT_STR_ITEM_INFO_LIM(ok, si, offset, struct_t, printerr, SOFTOFW_P5TFM, UINT32, "SOFTOFW_P5TFM", 2);     \
        EXT_STR_ITEM_INFO_ZZP(                                                                                    \
            ok, si, offset, struct_t, printerr, SOFTOFW_P5TFMI, UINT32, "SOFTOFW_P5TFMI", "SOFTOFW_P5TFM");       \
        EXT_STR_ITEM_INFO_ZZP(                                                                                    \
            ok, si, offset, struct_t, printerr, SOFTOFW_P5TFME, UINT32, "SOFTOFW_P5TFME", "SOFTOFW_P5TFM");       \
        EXT_STR_ITEM_INFO_LIM(ok, si, offset, struct_t, printerr, SOFTOFW_P5TF, UINT32, "SOFTOFW_P5TF", 20);      \
        EXT_STR_ITEM_INFO_ZZP(                                                                                    \
            ok, si, offset, struct_t, printerr, SOFTOFW_P5TFv, UINT32, "SOFTOFW_P5TFv", "SOFTOFW_P5TF");          \
        EXT_STR_ITEM_INFO_LIM(ok, si, offset, struct_t, printerr, SOFTOFW_P5TCM, UINT32, "SOFTOFW_P5TCM", 2);     \
        EXT_STR_ITEM_INFO_ZZP(                                                                                    \
            ok, si, offset, struct_t, printerr, SOFTOFW_P5TCMI, UINT32, "SOFTOFW_P5TCMI", "SOFTOFW_P5TCM");       \
        EXT_STR_ITEM_INFO_ZZP(                                                                                    \
            ok, si, offset, struct_t, printerr, SOFTOFW_P5TCME, UINT32, "SOFTOFW_P5TCME", "SOFTOFW_P5TCM");       \
        EXT_STR_ITEM_INFO_LIM(ok, si, offset, struct_t, printerr, SOFTOFW_P5TC, UINT32, "SOFTOFW_P5TC", 20);      \
        EXT_STR_ITEM_INFO_ZZP(                                                                                    \
            ok, si, offset, struct_t, printerr, SOFTOFW_P5TCv, UINT32, "SOFTOFW_P5TCv", "SOFTOFW_P5TC");          \
        EXT_STR_ITEM_INFO_LIM(ok, si, offset, struct_t, printerr, SOFTOFW_P5E1, UINT32, "SOFTOFW_P5E1", 65535);   \
        EXT_STR_ITEM_INFO_LIM(ok, si, offset, struct_t, printerr, SOFTOFW_P5E2, UINT32, "SOFTOFW_P5E2", 65535);   \
        EXT_STR_ITEM_INFO_LIM(ok, si, offset, struct_t, printerr, SOFTOFW_P6TFM, UINT32, "SOFTOFW_P6TFM", 2);     \
        EXT_STR_ITEM_INFO_ZZP(                                                                                    \
            ok, si, offset, struct_t, printerr, SOFTOFW_P6TFMI, UINT32, "SOFTOFW_P6TFMI", "SOFTOFW_P6TFM");       \
        EXT_STR_ITEM_INFO_ZZP(                                                                                    \
            ok, si, offset, struct_t, printerr, SOFTOFW_P6TFME, UINT32, "SOFTOFW_P6TFME", "SOFTOFW_P6TFM");       \
        EXT_STR_ITEM_INFO_LIM(ok, si, offset, struct_t, printerr, SOFTOFW_P6TF, UINT32, "SOFTOFW_P6TF", 20);      \
        EXT_STR_ITEM_INFO_ZZP(                                                                                    \
            ok, si, offset, struct_t, printerr, SOFTOFW_P6TFv, UINT32, "SOFTOFW_P6TFv", "SOFTOFW_P6TF");          \
        EXT_STR_ITEM_INFO_LIM(ok, si, offset, struct_t, printerr, SOFTOFW_P6TCM, UINT32, "SOFTOFW_P6TCM", 2);     \
        EXT_STR_ITEM_INFO_ZZP(                                                                                    \
            ok, si, offset, struct_t, printerr, SOFTOFW_P6TCMI, UINT32, "SOFTOFW_P6TCMI", "SOFTOFW_P6TCM");       \
        EXT_STR_ITEM_INFO_ZZP(                                                                                    \
            ok, si, offset, struct_t, printerr, SOFTOFW_P6TCME, UINT32, "SOFTOFW_P6TCME", "SOFTOFW_P6TCM");       \
        EXT_STR_ITEM_INFO_LIM(ok, si, offset, struct_t, printerr, SOFTOFW_P6TC, UINT32, "SOFTOFW_P6TC", 20);      \
        EXT_STR_ITEM_INFO_ZZP(                                                                                    \
            ok, si, offset, struct_t, printerr, SOFTOFW_P6TCv, UINT32, "SOFTOFW_P6TCv", "SOFTOFW_P6TC");          \
        EXT_STR_ITEM_INFO_LIM(ok, si, offset, struct_t, printerr, SOFTOFW_P6E1, UINT32, "SOFTOFW_P6E1", 65535);   \
        EXT_STR_ITEM_INFO_LIM(ok, si, offset, struct_t, printerr, SOFTOFW_P6E2, UINT32, "SOFTOFW_P6E2", 65535);   \
        EXT_STR_ITEM_INFO_LIM(ok, si, offset, struct_t, printerr, SOFTOFW_P7TFM, UINT32, "SOFTOFW_P7TFM", 2);     \
        EXT_STR_ITEM_INFO_ZZP(                                                                                    \
            ok, si, offset, struct_t, printerr, SOFTOFW_P7TFMI, UINT32, "SOFTOFW_P7TFMI", "SOFTOFW_P7TFM");       \
        EXT_STR_ITEM_INFO_ZZP(                                                                                    \
            ok, si, offset, struct_t, printerr, SOFTOFW_P7TFME, UINT32, "SOFTOFW_P7TFME", "SOFTOFW_P7TFM");       \
        EXT_STR_ITEM_INFO_LIM(ok, si, offset, struct_t, printerr, SOFTOFW_P7TF, UINT32, "SOFTOFW_P7TF", 20);      \
        EXT_STR_ITEM_INFO_ZZP(                                                                                    \
            ok, si, offset, struct_t, printerr, SOFTOFW_P7TFv, UINT32, "SOFTOFW_P7TFv", "SOFTOFW_P7TF");          \
        EXT_STR_ITEM_INFO_LIM(ok, si, offset, struct_t, printerr, SOFTOFW_P7TCM, UINT32, "SOFTOFW_P7TCM", 2);     \
        EXT_STR_ITEM_INFO_ZZP(                                                                                    \
            ok, si, offset, struct_t, printerr, SOFTOFW_P7TCMI, UINT32, "SOFTOFW_P7TCMI", "SOFTOFW_P7TCM");       \
        EXT_STR_ITEM_INFO_ZZP(                                                                                    \
            ok, si, offset, struct_t, printerr, SOFTOFW_P7TCME, UINT32, "SOFTOFW_P7TCME", "SOFTOFW_P7TCM");       \
        EXT_STR_ITEM_INFO_LIM(ok, si, offset, struct_t, printerr, SOFTOFW_P7TC, UINT32, "SOFTOFW_P7TC", 20);      \
        EXT_STR_ITEM_INFO_ZZP(                                                                                    \
            ok, si, offset, struct_t, printerr, SOFTOFW_P7TCv, UINT32, "SOFTOFW_P7TCv", "SOFTOFW_P7TC");          \
        EXT_STR_ITEM_INFO_LIM(ok, si, offset, struct_t, printerr, SOFTOFW_P7E1, UINT32, "SOFTOFW_P7E1", 65535);   \
        EXT_STR_ITEM_INFO_LIM(ok, si, offset, struct_t, printerr, SOFTOFW_P7E2, UINT32, "SOFTOFW_P7E2", 65535);   \
        EXT_STR_ITEM_INFO_LIM(ok, si, offset, struct_t, printerr, SOFTOFW_P8TFM, UINT32, "SOFTOFW_P8TFM", 2);     \
        EXT_STR_ITEM_INFO_ZZP(                                                                                    \
            ok, si, offset, struct_t, printerr, SOFTOFW_P8TFMI, UINT32, "SOFTOFW_P8TFMI", "SOFTOFW_P8TFM");       \
        EXT_STR_ITEM_INFO_ZZP(                                                                                    \
            ok, si, offset, struct_t, printerr, SOFTOFW_P8TFME, UINT32, "SOFTOFW_P8TFME", "SOFTOFW_P8TFM");       \
        EXT_STR_ITEM_INFO_LIM(ok, si, offset, struct_t, printerr, SOFTOFW_P8TF, UINT32, "SOFTOFW_P8TF", 20);      \
        EXT_STR_ITEM_INFO_ZZP(                                                                                    \
            ok, si, offset, struct_t, printerr, SOFTOFW_P8TFv, UINT32, "SOFTOFW_P8TFv", "SOFTOFW_P8TF");          \
        EXT_STR_ITEM_INFO_LIM(ok, si, offset, struct_t, printerr, SOFTOFW_P8TCM, UINT32, "SOFTOFW_P8TCM", 2);     \
        EXT_STR_ITEM_INFO_ZZP(                                                                                    \
            ok, si, offset, struct_t, printerr, SOFTOFW_P8TCMI, UINT32, "SOFTOFW_P8TCMI", "SOFTOFW_P8TCM");       \
        EXT_STR_ITEM_INFO_ZZP(                                                                                    \
            ok, si, offset, struct_t, printerr, SOFTOFW_P8TCME, UINT32, "SOFTOFW_P8TCME", "SOFTOFW_P8TCM");       \
        EXT_STR_ITEM_INFO_LIM(ok, si, offset, struct_t, printerr, SOFTOFW_P8TC, UINT32, "SOFTOFW_P8TC", 20);      \
        EXT_STR_ITEM_INFO_ZZP(                                                                                    \
            ok, si, offset, struct_t, printerr, SOFTOFW_P8TCv, UINT32, "SOFTOFW_P8TCv", "SOFTOFW_P8TC");          \
        EXT_STR_ITEM_INFO_LIM(ok, si, offset, struct_t, printerr, SOFTOFW_P8E1, UINT32, "SOFTOFW_P8E1", 65535);   \
        EXT_STR_ITEM_INFO_LIM(ok, si, offset, struct_t, printerr, SOFTOFW_P8E2, UINT32, "SOFTOFW_P8E2", 65535);   \
        EXT_STR_ITEM_INFO_LIM(ok, si, offset, struct_t, printerr, SOFTOFW_P9TFM, UINT32, "SOFTOFW_P9TFM", 2);     \
        EXT_STR_ITEM_INFO_ZZP(                                                                                    \
            ok, si, offset, struct_t, printerr, SOFTOFW_P9TFMI, UINT32, "SOFTOFW_P9TFMI", "SOFTOFW_P9TFM");       \
        EXT_STR_ITEM_INFO_ZZP(                                                                                    \
            ok, si, offset, struct_t, printerr, SOFTOFW_P9TFME, UINT32, "SOFTOFW_P9TFME", "SOFTOFW_P9TFM");       \
        EXT_STR_ITEM_INFO_LIM(ok, si, offset, struct_t, printerr, SOFTOFW_P9TF, UINT32, "SOFTOFW_P9TF", 20);      \
        EXT_STR_ITEM_INFO_ZZP(                                                                                    \
            ok, si, offset, struct_t, printerr, SOFTOFW_P9TFv, UINT32, "SOFTOFW_P9TFv", "SOFTOFW_P9TF");          \
        EXT_STR_ITEM_INFO_LIM(ok, si, offset, struct_t, printerr, SOFTOFW_P9TCM, UINT32, "SOFTOFW_P9TCM", 2);     \
        EXT_STR_ITEM_INFO_ZZP(                                                                                    \
            ok, si, offset, struct_t, printerr, SOFTOFW_P9TCMI, UINT32, "SOFTOFW_P9TCMI", "SOFTOFW_P9TCM");       \
        EXT_STR_ITEM_INFO_ZZP(                                                                                    \
            ok, si, offset, struct_t, printerr, SOFTOFW_P9TCME, UINT32, "SOFTOFW_P9TCME", "SOFTOFW_P9TCM");       \
        EXT_STR_ITEM_INFO_LIM(ok, si, offset, struct_t, printerr, SOFTOFW_P9TC, UINT32, "SOFTOFW_P9TC", 20);      \
        EXT_STR_ITEM_INFO_ZZP(                                                                                    \
            ok, si, offset, struct_t, printerr, SOFTOFW_P9TCv, UINT32, "SOFTOFW_P9TCv", "SOFTOFW_P9TC");          \
        EXT_STR_ITEM_INFO_LIM(ok, si, offset, struct_t, printerr, SOFTOFW_P9E1, UINT32, "SOFTOFW_P9E1", 65535);   \
        EXT_STR_ITEM_INFO_LIM(ok, si, offset, struct_t, printerr, SOFTOFW_P9E2, UINT32, "SOFTOFW_P9E2", 65535);   \
        EXT_STR_ITEM_INFO_LIM(ok, si, offset, struct_t, printerr, SOFTOFW_P10TFM, UINT32, "SOFTOFW_P10TFM", 2);   \
        EXT_STR_ITEM_INFO_ZZP(                                                                                    \
            ok, si, offset, struct_t, printerr, SOFTOFW_P10TFMI, UINT32, "SOFTOFW_P10TFMI", "SOFTOFW_P10TFM");    \
        EXT_STR_ITEM_INFO_ZZP(                                                                                    \
            ok, si, offset, struct_t, printerr, SOFTOFW_P10TFME, UINT32, "SOFTOFW_P10TFME", "SOFTOFW_P10TFM");    \
        EXT_STR_ITEM_INFO_LIM(ok, si, offset, struct_t, printerr, SOFTOFW_P10TF, UINT32, "SOFTOFW_P10TF", 20);    \
        EXT_STR_ITEM_INFO_ZZP(                                                                                    \
            ok, si, offset, struct_t, printerr, SOFTOFW_P10TFv, UINT32, "SOFTOFW_P10TFv", "SOFTOFW_P10TF");       \
        EXT_STR_ITEM_INFO_LIM(ok, si, offset, struct_t, printerr, SOFTOFW_P10TCM, UINT32, "SOFTOFW_P10TCM", 2);   \
        EXT_STR_ITEM_INFO_ZZP(                                                                                    \
            ok, si, offset, struct_t, printerr, SOFTOFW_P10TCMI, UINT32, "SOFTOFW_P10TCMI", "SOFTOFW_P10TCM");    \
        EXT_STR_ITEM_INFO_ZZP(                                                                                    \
            ok, si, offset, struct_t, printerr, SOFTOFW_P10TCME, UINT32, "SOFTOFW_P10TCME", "SOFTOFW_P10TCM");    \
        EXT_STR_ITEM_INFO_LIM(ok, si, offset, struct_t, printerr, SOFTOFW_P10TC, UINT32, "SOFTOFW_P10TC", 20);    \
        EXT_STR_ITEM_INFO_ZZP(                                                                                    \
            ok, si, offset, struct_t, printerr, SOFTOFW_P10TCv, UINT32, "SOFTOFW_P10TCv", "SOFTOFW_P10TC");       \
        EXT_STR_ITEM_INFO_LIM(ok, si, offset, struct_t, printerr, SOFTOFW_P10E1, UINT32, "SOFTOFW_P10E1", 65535); \
        EXT_STR_ITEM_INFO_LIM(ok, si, offset, struct_t, printerr, SOFTOFW_P10E2, UINT32, "SOFTOFW_P10E2", 65535); \
        EXT_STR_ITEM_INFO_LIM(ok, si, offset, struct_t, printerr, SOFTOFW_P11TFM, UINT32, "SOFTOFW_P11TFM", 2);   \
        EXT_STR_ITEM_INFO_ZZP(                                                                                    \
            ok, si, offset, struct_t, printerr, SOFTOFW_P11TFMI, UINT32, "SOFTOFW_P11TFMI", "SOFTOFW_P11TFM");    \
        EXT_STR_ITEM_INFO_ZZP(                                                                                    \
            ok, si, offset, struct_t, printerr, SOFTOFW_P11TFME, UINT32, "SOFTOFW_P11TFME", "SOFTOFW_P11TFM");    \
        EXT_STR_ITEM_INFO_LIM(ok, si, offset, struct_t, printerr, SOFTOFW_P11TF, UINT32, "SOFTOFW_P11TF", 20);    \
        EXT_STR_ITEM_INFO_ZZP(                                                                                    \
            ok, si, offset, struct_t, printerr, SOFTOFW_P11TFv, UINT32, "SOFTOFW_P11TFv", "SOFTOFW_P11TF");       \
        EXT_STR_ITEM_INFO_LIM(ok, si, offset, struct_t, printerr, SOFTOFW_P11TCM, UINT32, "SOFTOFW_P11TCM", 2);   \
        EXT_STR_ITEM_INFO_ZZP(                                                                                    \
            ok, si, offset, struct_t, printerr, SOFTOFW_P11TCMI, UINT32, "SOFTOFW_P11TCMI", "SOFTOFW_P11TCM");    \
        EXT_STR_ITEM_INFO_ZZP(                                                                                    \
            ok, si, offset, struct_t, printerr, SOFTOFW_P11TCME, UINT32, "SOFTOFW_P11TCME", "SOFTOFW_P11TCM");    \
        EXT_STR_ITEM_INFO_LIM(ok, si, offset, struct_t, printerr, SOFTOFW_P11TC, UINT32, "SOFTOFW_P11TC", 20);    \
        EXT_STR_ITEM_INFO_ZZP(                                                                                    \
            ok, si, offset, struct_t, printerr, SOFTOFW_P11TCv, UINT32, "SOFTOFW_P11TCv", "SOFTOFW_P11TC");       \
        EXT_STR_ITEM_INFO_LIM(ok, si, offset, struct_t, printerr, SOFTOFW_P11E1, UINT32, "SOFTOFW_P11E1", 65535); \
        EXT_STR_ITEM_INFO_LIM(ok, si, offset, struct_t, printerr, SOFTOFW_P11E2, UINT32, "SOFTOFW_P11E2", 65535); \
        EXT_STR_ITEM_INFO_LIM(ok, si, offset, struct_t, printerr, SOFTOFW_P12TFM, UINT32, "SOFTOFW_P12TFM", 2);   \
        EXT_STR_ITEM_INFO_ZZP(                                                                                    \
            ok, si, offset, struct_t, printerr, SOFTOFW_P12TFMI, UINT32, "SOFTOFW_P12TFMI", "SOFTOFW_P12TFM");    \
        EXT_STR_ITEM_INFO_ZZP(                                                                                    \
            ok, si, offset, struct_t, printerr, SOFTOFW_P12TFME, UINT32, "SOFTOFW_P12TFME", "SOFTOFW_P12TFM");    \
        EXT_STR_ITEM_INFO_LIM(ok, si, offset, struct_t, printerr, SOFTOFW_P12TF, UINT32, "SOFTOFW_P12TF", 20);    \
        EXT_STR_ITEM_INFO_ZZP(                                                                                    \
            ok, si, offset, struct_t, printerr, SOFTOFW_P12TFv, UINT32, "SOFTOFW_P12TFv", "SOFTOFW_P12TF");       \
        EXT_STR_ITEM_INFO_LIM(ok, si, offset, struct_t, printerr, SOFTOFW_P12TCM, UINT32, "SOFTOFW_P12TCM", 2);   \
        EXT_STR_ITEM_INFO_ZZP(                                                                                    \
            ok, si, offset, struct_t, printerr, SOFTOFW_P12TCMI, UINT32, "SOFTOFW_P12TCMI", "SOFTOFW_P12TCM");    \
        EXT_STR_ITEM_INFO_ZZP(                                                                                    \
            ok, si, offset, struct_t, printerr, SOFTOFW_P12TCME, UINT32, "SOFTOFW_P12TCME", "SOFTOFW_P12TCM");    \
        EXT_STR_ITEM_INFO_LIM(ok, si, offset, struct_t, printerr, SOFTOFW_P12TC, UINT32, "SOFTOFW_P12TC", 20);    \
        EXT_STR_ITEM_INFO_ZZP(                                                                                    \
            ok, si, offset, struct_t, printerr, SOFTOFW_P12TCv, UINT32, "SOFTOFW_P12TCv", "SOFTOFW_P12TC");       \
        EXT_STR_ITEM_INFO_LIM(ok, si, offset, struct_t, printerr, SOFTOFW_P12E1, UINT32, "SOFTOFW_P12E1", 65535); \
        EXT_STR_ITEM_INFO_LIM(ok, si, offset, struct_t, printerr, SOFTOFW_P12E2, UINT32, "SOFTOFW_P12E2", 65535); \
        EXT_STR_ITEM_INFO_LIM(ok, si, offset, struct_t, printerr, SOFTOFW_P13TFM, UINT32, "SOFTOFW_P13TFM", 2);   \
        EXT_STR_ITEM_INFO_ZZP(                                                                                    \
            ok, si, offset, struct_t, printerr, SOFTOFW_P13TFMI, UINT32, "SOFTOFW_P13TFMI", "SOFTOFW_P13TFM");    \
        EXT_STR_ITEM_INFO_ZZP(                                                                                    \
            ok, si, offset, struct_t, printerr, SOFTOFW_P13TFME, UINT32, "SOFTOFW_P13TFME", "SOFTOFW_P13TFM");    \
        EXT_STR_ITEM_INFO_LIM(ok, si, offset, struct_t, printerr, SOFTOFW_P13TF, UINT32, "SOFTOFW_P13TF", 20);    \
        EXT_STR_ITEM_INFO_ZZP(                                                                                    \
            ok, si, offset, struct_t, printerr, SOFTOFW_P13TFv, UINT32, "SOFTOFW_P13TFv", "SOFTOFW_P13TF");       \
        EXT_STR_ITEM_INFO_LIM(ok, si, offset, struct_t, printerr, SOFTOFW_P13TCM, UINT32, "SOFTOFW_P13TCM", 2);   \
        EXT_STR_ITEM_INFO_ZZP(                                                                                    \
            ok, si, offset, struct_t, printerr, SOFTOFW_P13TCMI, UINT32, "SOFTOFW_P13TCMI", "SOFTOFW_P13TCM");    \
        EXT_STR_ITEM_INFO_ZZP(                                                                                    \
            ok, si, offset, struct_t, printerr, SOFTOFW_P13TCME, UINT32, "SOFTOFW_P13TCME", "SOFTOFW_P13TCM");    \
        EXT_STR_ITEM_INFO_LIM(ok, si, offset, struct_t, printerr, SOFTOFW_P13TC, UINT32, "SOFTOFW_P13TC", 20);    \
        EXT_STR_ITEM_INFO_ZZP(                                                                                    \
            ok, si, offset, struct_t, printerr, SOFTOFW_P13TCv, UINT32, "SOFTOFW_P13TCv", "SOFTOFW_P13TC");       \
        EXT_STR_ITEM_INFO_LIM(ok, si, offset, struct_t, printerr, SOFTOFW_P13E1, UINT32, "SOFTOFW_P13E1", 65535); \
        EXT_STR_ITEM_INFO_LIM(ok, si, offset, struct_t, printerr, SOFTOFW_P13E2, UINT32, "SOFTOFW_P13E2", 65535); \
        EXT_STR_ITEM_INFO_LIM(ok, si, offset, struct_t, printerr, SOFTOFW_P14TFM, UINT32, "SOFTOFW_P14TFM", 2);   \
        EXT_STR_ITEM_INFO_ZZP(                                                                                    \
            ok, si, offset, struct_t, printerr, SOFTOFW_P14TFMI, UINT32, "SOFTOFW_P14TFMI", "SOFTOFW_P14TFM");    \
        EXT_STR_ITEM_INFO_ZZP(                                                                                    \
            ok, si, offset, struct_t, printerr, SOFTOFW_P14TFME, UINT32, "SOFTOFW_P14TFME", "SOFTOFW_P14TFM");    \
        EXT_STR_ITEM_INFO_LIM(ok, si, offset, struct_t, printerr, SOFTOFW_P14TF, UINT32, "SOFTOFW_P14TF", 20);    \
        EXT_STR_ITEM_INFO_ZZP(                                                                                    \
            ok, si, offset, struct_t, printerr, SOFTOFW_P14TFv, UINT32, "SOFTOFW_P14TFv", "SOFTOFW_P14TF");       \
        EXT_STR_ITEM_INFO_LIM(ok, si, offset, struct_t, printerr, SOFTOFW_P14TCM, UINT32, "SOFTOFW_P14TCM", 2);   \
        EXT_STR_ITEM_INFO_ZZP(                                                                                    \
            ok, si, offset, struct_t, printerr, SOFTOFW_P14TCMI, UINT32, "SOFTOFW_P14TCMI", "SOFTOFW_P14TCM");    \
        EXT_STR_ITEM_INFO_ZZP(                                                                                    \
            ok, si, offset, struct_t, printerr, SOFTOFW_P14TCME, UINT32, "SOFTOFW_P14TCME", "SOFTOFW_P14TCM");    \
        EXT_STR_ITEM_INFO_LIM(ok, si, offset, struct_t, printerr, SOFTOFW_P14TC, UINT32, "SOFTOFW_P14TC", 20);    \
        EXT_STR_ITEM_INFO_ZZP(                                                                                    \
            ok, si, offset, struct_t, printerr, SOFTOFW_P14TCv, UINT32, "SOFTOFW_P14TCv", "SOFTOFW_P14TC");       \
        EXT_STR_ITEM_INFO_LIM(ok, si, offset, struct_t, printerr, SOFTOFW_P14E1, UINT32, "SOFTOFW_P14E1", 65535); \
        EXT_STR_ITEM_INFO_LIM(ok, si, offset, struct_t, printerr, SOFTOFW_P14E2, UINT32, "SOFTOFW_P14E2", 65535); \
        EXT_STR_ITEM_INFO_LIM(ok, si, offset, struct_t, printerr, SOFTOFW_P15TFM, UINT32, "SOFTOFW_P15TFM", 2);   \
        EXT_STR_ITEM_INFO_ZZP(                                                                                    \
            ok, si, offset, struct_t, printerr, SOFTOFW_P15TFMI, UINT32, "SOFTOFW_P15TFMI", "SOFTOFW_P15TFM");    \
        EXT_STR_ITEM_INFO_ZZP(                                                                                    \
            ok, si, offset, struct_t, printerr, SOFTOFW_P15TFME, UINT32, "SOFTOFW_P15TFME", "SOFTOFW_P15TFM");    \
        EXT_STR_ITEM_INFO_LIM(ok, si, offset, struct_t, printerr, SOFTOFW_P15TF, UINT32, "SOFTOFW_P15TF", 20);    \
        EXT_STR_ITEM_INFO_ZZP(                                                                                    \
            ok, si, offset, struct_t, printerr, SOFTOFW_P15TFv, UINT32, "SOFTOFW_P15TFv", "SOFTOFW_P15TF");       \
        EXT_STR_ITEM_INFO_LIM(ok, si, offset, struct_t, printerr, SOFTOFW_P15TCM, UINT32, "SOFTOFW_P15TCM", 2);   \
        EXT_STR_ITEM_INFO_ZZP(                                                                                    \
            ok, si, offset, struct_t, printerr, SOFTOFW_P15TCMI, UINT32, "SOFTOFW_P15TCMI", "SOFTOFW_P15TCM");    \
        EXT_STR_ITEM_INFO_ZZP(                                                                                    \
            ok, si, offset, struct_t, printerr, SOFTOFW_P15TCME, UINT32, "SOFTOFW_P15TCME", "SOFTOFW_P15TCM");    \
        EXT_STR_ITEM_INFO_LIM(ok, si, offset, struct_t, printerr, SOFTOFW_P15TC, UINT32, "SOFTOFW_P15TC", 20);    \
        EXT_STR_ITEM_INFO_ZZP(                                                                                    \
            ok, si, offset, struct_t, printerr, SOFTOFW_P15TCv, UINT32, "SOFTOFW_P15TCv", "SOFTOFW_P15TC");       \
        EXT_STR_ITEM_INFO_LIM(ok, si, offset, struct_t, printerr, SOFTOFW_P15E1, UINT32, "SOFTOFW_P15E1", 65535); \
        EXT_STR_ITEM_INFO_LIM(ok, si, offset, struct_t, printerr, SOFTOFW_P15E2, UINT32, "SOFTOFW_P15E2", 65535); \
        EXT_STR_ITEM_INFO_LIM(ok, si, offset, struct_t, printerr, SOFTOFW_P16TFM, UINT32, "SOFTOFW_P16TFM", 2);   \
        EXT_STR_ITEM_INFO_ZZP(                                                                                    \
            ok, si, offset, struct_t, printerr, SOFTOFW_P16TFMI, UINT32, "SOFTOFW_P16TFMI", "SOFTOFW_P16TFM");    \
        EXT_STR_ITEM_INFO_ZZP(                                                                                    \
            ok, si, offset, struct_t, printerr, SOFTOFW_P16TFME, UINT32, "SOFTOFW_P16TFME", "SOFTOFW_P16TFM");    \
        EXT_STR_ITEM_INFO_LIM(ok, si, offset, struct_t, printerr, SOFTOFW_P16TF, UINT32, "SOFTOFW_P16TF", 20);    \
        EXT_STR_ITEM_INFO_ZZP(                                                                                    \
            ok, si, offset, struct_t, printerr, SOFTOFW_P16TFv, UINT32, "SOFTOFW_P16TFv", "SOFTOFW_P16TF");       \
        EXT_STR_ITEM_INFO_LIM(ok, si, offset, struct_t, printerr, SOFTOFW_P16TCM, UINT32, "SOFTOFW_P16TCM", 2);   \
        EXT_STR_ITEM_INFO_ZZP(                                                                                    \
            ok, si, offset, struct_t, printerr, SOFTOFW_P16TCMI, UINT32, "SOFTOFW_P16TCMI", "SOFTOFW_P16TCM");    \
        EXT_STR_ITEM_INFO_ZZP(                                                                                    \
            ok, si, offset, struct_t, printerr, SOFTOFW_P16TCME, UINT32, "SOFTOFW_P16TCME", "SOFTOFW_P16TCM");    \
        EXT_STR_ITEM_INFO_LIM(ok, si, offset, struct_t, printerr, SOFTOFW_P16TC, UINT32, "SOFTOFW_P16TC", 20);    \
        EXT_STR_ITEM_INFO_ZZP(                                                                                    \
            ok, si, offset, struct_t, printerr, SOFTOFW_P16TCv, UINT32, "SOFTOFW_P16TCv", "SOFTOFW_P16TC");       \
        EXT_STR_ITEM_INFO_LIM(ok, si, offset, struct_t, printerr, SOFTOFW_P16E1, UINT32, "SOFTOFW_P16E1", 65535); \
        EXT_STR_ITEM_INFO_LIM(ok, si, offset, struct_t, printerr, SOFTOFW_P16E2, UINT32, "SOFTOFW_P16E2", 65535); \
        EXT_STR_ITEM_INFO_LIM(ok, si, offset, struct_t, printerr, SOFTOFW_P17TFM, UINT32, "SOFTOFW_P17TFM", 2);   \
        EXT_STR_ITEM_INFO_ZZP(                                                                                    \
            ok, si, offset, struct_t, printerr, SOFTOFW_P17TFMI, UINT32, "SOFTOFW_P17TFMI", "SOFTOFW_P17TFM");    \
        EXT_STR_ITEM_INFO_ZZP(                                                                                    \
            ok, si, offset, struct_t, printerr, SOFTOFW_P17TFME, UINT32, "SOFTOFW_P17TFME", "SOFTOFW_P17TFM");    \
        EXT_STR_ITEM_INFO_LIM(ok, si, offset, struct_t, printerr, SOFTOFW_P17TF, UINT32, "SOFTOFW_P17TF", 20);    \
        EXT_STR_ITEM_INFO_ZZP(                                                                                    \
            ok, si, offset, struct_t, printerr, SOFTOFW_P17TFv, UINT32, "SOFTOFW_P17TFv", "SOFTOFW_P17TF");       \
        EXT_STR_ITEM_INFO_LIM(ok, si, offset, struct_t, printerr, SOFTOFW_P17TCM, UINT32, "SOFTOFW_P17TCM", 2);   \
        EXT_STR_ITEM_INFO_ZZP(                                                                                    \
            ok, si, offset, struct_t, printerr, SOFTOFW_P17TCMI, UINT32, "SOFTOFW_P17TCMI", "SOFTOFW_P17TCM");    \
        EXT_STR_ITEM_INFO_ZZP(                                                                                    \
            ok, si, offset, struct_t, printerr, SOFTOFW_P17TCME, UINT32, "SOFTOFW_P17TCME", "SOFTOFW_P17TCM");    \
        EXT_STR_ITEM_INFO_LIM(ok, si, offset, struct_t, printerr, SOFTOFW_P17TC, UINT32, "SOFTOFW_P17TC", 20);    \
        EXT_STR_ITEM_INFO_ZZP(                                                                                    \
            ok, si, offset, struct_t, printerr, SOFTOFW_P17TCv, UINT32, "SOFTOFW_P17TCv", "SOFTOFW_P17TC");       \
        EXT_STR_ITEM_INFO_LIM(ok, si, offset, struct_t, printerr, SOFTOFW_P17E1, UINT32, "SOFTOFW_P17E1", 65535); \
        EXT_STR_ITEM_INFO_LIM(ok, si, offset, struct_t, printerr, SOFTOFW_P17E2, UINT32, "SOFTOFW_P17E2", 65535); \
        EXT_STR_ITEM_INFO_LIM(ok, si, offset, struct_t, printerr, SOFTOFW_P18TFM, UINT32, "SOFTOFW_P18TFM", 2);   \
        EXT_STR_ITEM_INFO_ZZP(                                                                                    \
            ok, si, offset, struct_t, printerr, SOFTOFW_P18TFMI, UINT32, "SOFTOFW_P18TFMI", "SOFTOFW_P18TFM");    \
        EXT_STR_ITEM_INFO_ZZP(                                                                                    \
            ok, si, offset, struct_t, printerr, SOFTOFW_P18TFME, UINT32, "SOFTOFW_P18TFME", "SOFTOFW_P18TFM");    \
        EXT_STR_ITEM_INFO_LIM(ok, si, offset, struct_t, printerr, SOFTOFW_P18TF, UINT32, "SOFTOFW_P18TF", 20);    \
        EXT_STR_ITEM_INFO_ZZP(                                                                                    \
            ok, si, offset, struct_t, printerr, SOFTOFW_P18TFv, UINT32, "SOFTOFW_P18TFv", "SOFTOFW_P18TF");       \
        EXT_STR_ITEM_INFO_LIM(ok, si, offset, struct_t, printerr, SOFTOFW_P18TCM, UINT32, "SOFTOFW_P18TCM", 2);   \
        EXT_STR_ITEM_INFO_ZZP(                                                                                    \
            ok, si, offset, struct_t, printerr, SOFTOFW_P18TCMI, UINT32, "SOFTOFW_P18TCMI", "SOFTOFW_P18TCM");    \
        EXT_STR_ITEM_INFO_ZZP(                                                                                    \
            ok, si, offset, struct_t, printerr, SOFTOFW_P18TCME, UINT32, "SOFTOFW_P18TCME", "SOFTOFW_P18TCM");    \
        EXT_STR_ITEM_INFO_LIM(ok, si, offset, struct_t, printerr, SOFTOFW_P18TC, UINT32, "SOFTOFW_P18TC", 20);    \
        EXT_STR_ITEM_INFO_ZZP(                                                                                    \
            ok, si, offset, struct_t, printerr, SOFTOFW_P18TCv, UINT32, "SOFTOFW_P18TCv", "SOFTOFW_P18TC");       \
        EXT_STR_ITEM_INFO_LIM(ok, si, offset, struct_t, printerr, SOFTOFW_P18E1, UINT32, "SOFTOFW_P18E1", 65535); \
        EXT_STR_ITEM_INFO_LIM(ok, si, offset, struct_t, printerr, SOFTOFW_P18E2, UINT32, "SOFTOFW_P18E2", 65535); \
        EXT_STR_ITEM_INFO_LIM(ok, si, offset, struct_t, printerr, SOFTOFW_P19TFM, UINT32, "SOFTOFW_P19TFM", 2);   \
        EXT_STR_ITEM_INFO_ZZP(                                                                                    \
            ok, si, offset, struct_t, printerr, SOFTOFW_P19TFMI, UINT32, "SOFTOFW_P19TFMI", "SOFTOFW_P19TFM");    \
        EXT_STR_ITEM_INFO_ZZP(                                                                                    \
            ok, si, offset, struct_t, printerr, SOFTOFW_P19TFME, UINT32, "SOFTOFW_P19TFME", "SOFTOFW_P19TFM");    \
        EXT_STR_ITEM_INFO_LIM(ok, si, offset, struct_t, printerr, SOFTOFW_P19TF, UINT32, "SOFTOFW_P19TF", 20);    \
        EXT_STR_ITEM_INFO_ZZP(                                                                                    \
            ok, si, offset, struct_t, printerr, SOFTOFW_P19TFv, UINT32, "SOFTOFW_P19TFv", "SOFTOFW_P19TF");       \
        EXT_STR_ITEM_INFO_LIM(ok, si, offset, struct_t, printerr, SOFTOFW_P19TCM, UINT32, "SOFTOFW_P19TCM", 2);   \
        EXT_STR_ITEM_INFO_ZZP(                                                                                    \
            ok, si, offset, struct_t, printerr, SOFTOFW_P19TCMI, UINT32, "SOFTOFW_P19TCMI", "SOFTOFW_P19TCM");    \
        EXT_STR_ITEM_INFO_ZZP(                                                                                    \
            ok, si, offset, struct_t, printerr, SOFTOFW_P19TCME, UINT32, "SOFTOFW_P19TCME", "SOFTOFW_P19TCM");    \
        EXT_STR_ITEM_INFO_LIM(ok, si, offset, struct_t, printerr, SOFTOFW_P19TC, UINT32, "SOFTOFW_P19TC", 20);    \
        EXT_STR_ITEM_INFO_ZZP(                                                                                    \
            ok, si, offset, struct_t, printerr, SOFTOFW_P19TCv, UINT32, "SOFTOFW_P19TCv", "SOFTOFW_P19TC");       \
        EXT_STR_ITEM_INFO_LIM(ok, si, offset, struct_t, printerr, SOFTOFW_P19E1, UINT32, "SOFTOFW_P19E1", 65535); \
        EXT_STR_ITEM_INFO_LIM(ok, si, offset, struct_t, printerr, SOFTOFW_P19E2, UINT32, "SOFTOFW_P19E2", 65535); \
        EXT_STR_ITEM_INFO_LIM(ok, si, offset, struct_t, printerr, SOFTOFW_P20TFM, UINT32, "SOFTOFW_P20TFM", 2);   \
        EXT_STR_ITEM_INFO_ZZP(                                                                                    \
            ok, si, offset, struct_t, printerr, SOFTOFW_P20TFMI, UINT32, "SOFTOFW_P20TFMI", "SOFTOFW_P20TFM");    \
        EXT_STR_ITEM_INFO_ZZP(                                                                                    \
            ok, si, offset, struct_t, printerr, SOFTOFW_P20TFME, UINT32, "SOFTOFW_P20TFME", "SOFTOFW_P20TFM");    \
        EXT_STR_ITEM_INFO_LIM(ok, si, offset, struct_t, printerr, SOFTOFW_P20TF, UINT32, "SOFTOFW_P20TF", 20);    \
        EXT_STR_ITEM_INFO_ZZP(                                                                                    \
            ok, si, offset, struct_t, printerr, SOFTOFW_P20TFv, UINT32, "SOFTOFW_P20TFv", "SOFTOFW_P20TF");       \
        EXT_STR_ITEM_INFO_LIM(ok, si, offset, struct_t, printerr, SOFTOFW_P20TCM, UINT32, "SOFTOFW_P20TCM", 2);   \
        EXT_STR_ITEM_INFO_ZZP(                                                                                    \
            ok, si, offset, struct_t, printerr, SOFTOFW_P20TCMI, UINT32, "SOFTOFW_P20TCMI", "SOFTOFW_P20TCM");    \
        EXT_STR_ITEM_INFO_ZZP(                                                                                    \
            ok, si, offset, struct_t, printerr, SOFTOFW_P20TCME, UINT32, "SOFTOFW_P20TCME", "SOFTOFW_P20TCM");    \
        EXT_STR_ITEM_INFO_LIM(ok, si, offset, struct_t, printerr, SOFTOFW_P20TC, UINT32, "SOFTOFW_P20TC", 20);    \
        EXT_STR_ITEM_INFO_ZZP(                                                                                    \
            ok, si, offset, struct_t, printerr, SOFTOFW_P20TCv, UINT32, "SOFTOFW_P20TCv", "SOFTOFW_P20TC");       \
        EXT_STR_ITEM_INFO_LIM(ok, si, offset, struct_t, printerr, SOFTOFW_P20E1, UINT32, "SOFTOFW_P20E1", 65535); \
        EXT_STR_ITEM_INFO_LIM(ok, si, offset, struct_t, printerr, SOFTOFW_P20E2, UINT32, "SOFTOFW_P20E2", 65535); \
        EXT_STR_ITEM_INFO_LIM(ok, si, offset, struct_t, printerr, SOFTOFW_P21TFM, UINT32, "SOFTOFW_P21TFM", 2);   \
        EXT_STR_ITEM_INFO_ZZP(                                                                                    \
            ok, si, offset, struct_t, printerr, SOFTOFW_P21TFMI, UINT32, "SOFTOFW_P21TFMI", "SOFTOFW_P21TFM");    \
        EXT_STR_ITEM_INFO_ZZP(                                                                                    \
            ok, si, offset, struct_t, printerr, SOFTOFW_P21TFME, UINT32, "SOFTOFW_P21TFME", "SOFTOFW_P21TFM");    \
        EXT_STR_ITEM_INFO_LIM(ok, si, offset, struct_t, printerr, SOFTOFW_P21TF, UINT32, "SOFTOFW_P21TF", 20);    \
        EXT_STR_ITEM_INFO_ZZP(                                                                                    \
            ok, si, offset, struct_t, printerr, SOFTOFW_P21TFv, UINT32, "SOFTOFW_P21TFv", "SOFTOFW_P21TF");       \
        EXT_STR_ITEM_INFO_LIM(ok, si, offset, struct_t, printerr, SOFTOFW_P21TCM, UINT32, "SOFTOFW_P21TCM", 2);   \
        EXT_STR_ITEM_INFO_ZZP(                                                                                    \
            ok, si, offset, struct_t, printerr, SOFTOFW_P21TCMI, UINT32, "SOFTOFW_P21TCMI", "SOFTOFW_P21TCM");    \
        EXT_STR_ITEM_INFO_ZZP(                                                                                    \
            ok, si, offset, struct_t, printerr, SOFTOFW_P21TCME, UINT32, "SOFTOFW_P21TCME", "SOFTOFW_P21TCM");    \
        EXT_STR_ITEM_INFO_LIM(ok, si, offset, struct_t, printerr, SOFTOFW_P21TC, UINT32, "SOFTOFW_P21TC", 20);    \
        EXT_STR_ITEM_INFO_ZZP(                                                                                    \
            ok, si, offset, struct_t, printerr, SOFTOFW_P21TCv, UINT32, "SOFTOFW_P21TCv", "SOFTOFW_P21TC");       \
        EXT_STR_ITEM_INFO_LIM(ok, si, offset, struct_t, printerr, SOFTOFW_P21E1, UINT32, "SOFTOFW_P21E1", 65535); \
        EXT_STR_ITEM_INFO_LIM(ok, si, offset, struct_t, printerr, SOFTOFW_P21E2, UINT32, "SOFTOFW_P21E2", 65535); \
        EXT_STR_ITEM_INFO_LIM(ok, si, offset, struct_t, printerr, SOFTOFW_P22TFM, UINT32, "SOFTOFW_P22TFM", 2);   \
        EXT_STR_ITEM_INFO_ZZP(                                                                                    \
            ok, si, offset, struct_t, printerr, SOFTOFW_P22TFMI, UINT32, "SOFTOFW_P22TFMI", "SOFTOFW_P22TFM");    \
        EXT_STR_ITEM_INFO_ZZP(                                                                                    \
            ok, si, offset, struct_t, printerr, SOFTOFW_P22TFME, UINT32, "SOFTOFW_P22TFME", "SOFTOFW_P22TFM");    \
        EXT_STR_ITEM_INFO_LIM(ok, si, offset, struct_t, printerr, SOFTOFW_P22TF, UINT32, "SOFTOFW_P22TF", 20);    \
        EXT_STR_ITEM_INFO_ZZP(                                                                                    \
            ok, si, offset, struct_t, printerr, SOFTOFW_P22TFv, UINT32, "SOFTOFW_P22TFv", "SOFTOFW_P22TF");       \
        EXT_STR_ITEM_INFO_LIM(ok, si, offset, struct_t, printerr, SOFTOFW_P22TCM, UINT32, "SOFTOFW_P22TCM", 2);   \
        EXT_STR_ITEM_INFO_ZZP(                                                                                    \
            ok, si, offset, struct_t, printerr, SOFTOFW_P22TCMI, UINT32, "SOFTOFW_P22TCMI", "SOFTOFW_P22TCM");    \
        EXT_STR_ITEM_INFO_ZZP(                                                                                    \
            ok, si, offset, struct_t, printerr, SOFTOFW_P22TCME, UINT32, "SOFTOFW_P22TCME", "SOFTOFW_P22TCM");    \
        EXT_STR_ITEM_INFO_LIM(ok, si, offset, struct_t, printerr, SOFTOFW_P22TC, UINT32, "SOFTOFW_P22TC", 20);    \
        EXT_STR_ITEM_INFO_ZZP(                                                                                    \
            ok, si, offset, struct_t, printerr, SOFTOFW_P22TCv, UINT32, "SOFTOFW_P22TCv", "SOFTOFW_P22TC");       \
        EXT_STR_ITEM_INFO_LIM(ok, si, offset, struct_t, printerr, SOFTOFW_P22E1, UINT32, "SOFTOFW_P22E1", 65535); \
        EXT_STR_ITEM_INFO_LIM(ok, si, offset, struct_t, printerr, SOFTOFW_P22E2, UINT32, "SOFTOFW_P22E2", 65535); \
        EXT_STR_ITEM_INFO_LIM(ok, si, offset, struct_t, printerr, SOFTOFW_P23TFM, UINT32, "SOFTOFW_P23TFM", 2);   \
        EXT_STR_ITEM_INFO_ZZP(                                                                                    \
            ok, si, offset, struct_t, printerr, SOFTOFW_P23TFMI, UINT32, "SOFTOFW_P23TFMI", "SOFTOFW_P23TFM");    \
        EXT_STR_ITEM_INFO_ZZP(                                                                                    \
            ok, si, offset, struct_t, printerr, SOFTOFW_P23TFME, UINT32, "SOFTOFW_P23TFME", "SOFTOFW_P23TFM");    \
        EXT_STR_ITEM_INFO_LIM(ok, si, offset, struct_t, printerr, SOFTOFW_P23TF, UINT32, "SOFTOFW_P23TF", 20);    \
        EXT_STR_ITEM_INFO_ZZP(                                                                                    \
            ok, si, offset, struct_t, printerr, SOFTOFW_P23TFv, UINT32, "SOFTOFW_P23TFv", "SOFTOFW_P23TF");       \
        EXT_STR_ITEM_INFO_LIM(ok, si, offset, struct_t, printerr, SOFTOFW_P23TCM, UINT32, "SOFTOFW_P23TCM", 2);   \
        EXT_STR_ITEM_INFO_ZZP(                                                                                    \
            ok, si, offset, struct_t, printerr, SOFTOFW_P23TCMI, UINT32, "SOFTOFW_P23TCMI", "SOFTOFW_P23TCM");    \
        EXT_STR_ITEM_INFO_ZZP(                                                                                    \
            ok, si, offset, struct_t, printerr, SOFTOFW_P23TCME, UINT32, "SOFTOFW_P23TCME", "SOFTOFW_P23TCM");    \
        EXT_STR_ITEM_INFO_LIM(ok, si, offset, struct_t, printerr, SOFTOFW_P23TC, UINT32, "SOFTOFW_P23TC", 20);    \
        EXT_STR_ITEM_INFO_ZZP(                                                                                    \
            ok, si, offset, struct_t, printerr, SOFTOFW_P23TCv, UINT32, "SOFTOFW_P23TCv", "SOFTOFW_P23TC");       \
        EXT_STR_ITEM_INFO_LIM(ok, si, offset, struct_t, printerr, SOFTOFW_P23E1, UINT32, "SOFTOFW_P23E1", 65535); \
        EXT_STR_ITEM_INFO_LIM(ok, si, offset, struct_t, printerr, SOFTOFW_P23E2, UINT32, "SOFTOFW_P23E2", 65535); \
        EXT_STR_ITEM_INFO_LIM(ok, si, offset, struct_t, printerr, SOFTOFW_P24TFM, UINT32, "SOFTOFW_P24TFM", 2);   \
        EXT_STR_ITEM_INFO_ZZP(                                                                                    \
            ok, si, offset, struct_t, printerr, SOFTOFW_P24TFMI, UINT32, "SOFTOFW_P24TFMI", "SOFTOFW_P24TFM");    \
        EXT_STR_ITEM_INFO_ZZP(                                                                                    \
            ok, si, offset, struct_t, printerr, SOFTOFW_P24TFME, UINT32, "SOFTOFW_P24TFME", "SOFTOFW_P24TFM");    \
        EXT_STR_ITEM_INFO_LIM(ok, si, offset, struct_t, printerr, SOFTOFW_P24TF, UINT32, "SOFTOFW_P24TF", 20);    \
        EXT_STR_ITEM_INFO_ZZP(                                                                                    \
            ok, si, offset, struct_t, printerr, SOFTOFW_P24TFv, UINT32, "SOFTOFW_P24TFv", "SOFTOFW_P24TF");       \
        EXT_STR_ITEM_INFO_LIM(ok, si, offset, struct_t, printerr, SOFTOFW_P24TCM, UINT32, "SOFTOFW_P24TCM", 2);   \
        EXT_STR_ITEM_INFO_ZZP(                                                                                    \
            ok, si, offset, struct_t, printerr, SOFTOFW_P24TCMI, UINT32, "SOFTOFW_P24TCMI", "SOFTOFW_P24TCM");    \
        EXT_STR_ITEM_INFO_ZZP(                                                                                    \
            ok, si, offset, struct_t, printerr, SOFTOFW_P24TCME, UINT32, "SOFTOFW_P24TCME", "SOFTOFW_P24TCM");    \
        EXT_STR_ITEM_INFO_LIM(ok, si, offset, struct_t, printerr, SOFTOFW_P24TC, UINT32, "SOFTOFW_P24TC", 20);    \
        EXT_STR_ITEM_INFO_ZZP(                                                                                    \
            ok, si, offset, struct_t, printerr, SOFTOFW_P24TCv, UINT32, "SOFTOFW_P24TCv", "SOFTOFW_P24TC");       \
        EXT_STR_ITEM_INFO_LIM(ok, si, offset, struct_t, printerr, SOFTOFW_P24E1, UINT32, "SOFTOFW_P24E1", 65535); \
        EXT_STR_ITEM_INFO_LIM(ok, si, offset, struct_t, printerr, SOFTOFW_P24E2, UINT32, "SOFTOFW_P24E2", 65535); \
        EXT_STR_ITEM_INFO_LIM(ok, si, offset, struct_t, printerr, SOFTOFW_P25TFM, UINT32, "SOFTOFW_P25TFM", 2);   \
        EXT_STR_ITEM_INFO_ZZP(                                                                                    \
            ok, si, offset, struct_t, printerr, SOFTOFW_P25TFMI, UINT32, "SOFTOFW_P25TFMI", "SOFTOFW_P25TFM");    \
        EXT_STR_ITEM_INFO_ZZP(                                                                                    \
            ok, si, offset, struct_t, printerr, SOFTOFW_P25TFME, UINT32, "SOFTOFW_P25TFME", "SOFTOFW_P25TFM");    \
        EXT_STR_ITEM_INFO_LIM(ok, si, offset, struct_t, printerr, SOFTOFW_P25TF, UINT32, "SOFTOFW_P25TF", 20);    \
        EXT_STR_ITEM_INFO_ZZP(                                                                                    \
            ok, si, offset, struct_t, printerr, SOFTOFW_P25TFv, UINT32, "SOFTOFW_P25TFv", "SOFTOFW_P25TF");       \
        EXT_STR_ITEM_INFO_LIM(ok, si, offset, struct_t, printerr, SOFTOFW_P25TCM, UINT32, "SOFTOFW_P25TCM", 2);   \
        EXT_STR_ITEM_INFO_ZZP(                                                                                    \
            ok, si, offset, struct_t, printerr, SOFTOFW_P25TCMI, UINT32, "SOFTOFW_P25TCMI", "SOFTOFW_P25TCM");    \
        EXT_STR_ITEM_INFO_ZZP(                                                                                    \
            ok, si, offset, struct_t, printerr, SOFTOFW_P25TCME, UINT32, "SOFTOFW_P25TCME", "SOFTOFW_P25TCM");    \
        EXT_STR_ITEM_INFO_LIM(ok, si, offset, struct_t, printerr, SOFTOFW_P25TC, UINT32, "SOFTOFW_P25TC", 20);    \
        EXT_STR_ITEM_INFO_ZZP(                                                                                    \
            ok, si, offset, struct_t, printerr, SOFTOFW_P25TCv, UINT32, "SOFTOFW_P25TCv", "SOFTOFW_P25TC");       \
        EXT_STR_ITEM_INFO_LIM(ok, si, offset, struct_t, printerr, SOFTOFW_P25E1, UINT32, "SOFTOFW_P25E1", 65535); \
        EXT_STR_ITEM_INFO_LIM(ok, si, offset, struct_t, printerr, SOFTOFW_P25E2, UINT32, "SOFTOFW_P25E2", 65535); \
        EXT_STR_ITEM_INFO_LIM(ok, si, offset, struct_t, printerr, SOFTOFW_P26TFM, UINT32, "SOFTOFW_P26TFM", 2);   \
        EXT_STR_ITEM_INFO_ZZP(                                                                                    \
            ok, si, offset, struct_t, printerr, SOFTOFW_P26TFMI, UINT32, "SOFTOFW_P26TFMI", "SOFTOFW_P26TFM");    \
        EXT_STR_ITEM_INFO_ZZP(                                                                                    \
            ok, si, offset, struct_t, printerr, SOFTOFW_P26TFME, UINT32, "SOFTOFW_P26TFME", "SOFTOFW_P26TFM");    \
        EXT_STR_ITEM_INFO_LIM(ok, si, offset, struct_t, printerr, SOFTOFW_P26TF, UINT32, "SOFTOFW_P26TF", 20);    \
        EXT_STR_ITEM_INFO_ZZP(                                                                                    \
            ok, si, offset, struct_t, printerr, SOFTOFW_P26TFv, UINT32, "SOFTOFW_P26TFv", "SOFTOFW_P26TF");       \
        EXT_STR_ITEM_INFO_LIM(ok, si, offset, struct_t, printerr, SOFTOFW_P26TCM, UINT32, "SOFTOFW_P26TCM", 2);   \
        EXT_STR_ITEM_INFO_ZZP(                                                                                    \
            ok, si, offset, struct_t, printerr, SOFTOFW_P26TCMI, UINT32, "SOFTOFW_P26TCMI", "SOFTOFW_P26TCM");    \
        EXT_STR_ITEM_INFO_ZZP(                                                                                    \
            ok, si, offset, struct_t, printerr, SOFTOFW_P26TCME, UINT32, "SOFTOFW_P26TCME", "SOFTOFW_P26TCM");    \
        EXT_STR_ITEM_INFO_LIM(ok, si, offset, struct_t, printerr, SOFTOFW_P26TC, UINT32, "SOFTOFW_P26TC", 20);    \
        EXT_STR_ITEM_INFO_ZZP(                                                                                    \
            ok, si, offset, struct_t, printerr, SOFTOFW_P26TCv, UINT32, "SOFTOFW_P26TCv", "SOFTOFW_P26TC");       \
        EXT_STR_ITEM_INFO_LIM(ok, si, offset, struct_t, printerr, SOFTOFW_P26E1, UINT32, "SOFTOFW_P26E1", 65535); \
        EXT_STR_ITEM_INFO_LIM(ok, si, offset, struct_t, printerr, SOFTOFW_P26E2, UINT32, "SOFTOFW_P26E2", 65535); \
        EXT_STR_ITEM_INFO_LIM(ok, si, offset, struct_t, printerr, SOFTOFW_P27TFM, UINT32, "SOFTOFW_P27TFM", 2);   \
        EXT_STR_ITEM_INFO_ZZP(                                                                                    \
            ok, si, offset, struct_t, printerr, SOFTOFW_P27TFMI, UINT32, "SOFTOFW_P27TFMI", "SOFTOFW_P27TFM");    \
        EXT_STR_ITEM_INFO_ZZP(                                                                                    \
            ok, si, offset, struct_t, printerr, SOFTOFW_P27TFME, UINT32, "SOFTOFW_P27TFME", "SOFTOFW_P27TFM");    \
        EXT_STR_ITEM_INFO_LIM(ok, si, offset, struct_t, printerr, SOFTOFW_P27TF, UINT32, "SOFTOFW_P27TF", 20);    \
        EXT_STR_ITEM_INFO_ZZP(                                                                                    \
            ok, si, offset, struct_t, printerr, SOFTOFW_P27TFv, UINT32, "SOFTOFW_P27TFv", "SOFTOFW_P27TF");       \
        EXT_STR_ITEM_INFO_LIM(ok, si, offset, struct_t, printerr, SOFTOFW_P27TCM, UINT32, "SOFTOFW_P27TCM", 2);   \
        EXT_STR_ITEM_INFO_ZZP(                                                                                    \
            ok, si, offset, struct_t, printerr, SOFTOFW_P27TCMI, UINT32, "SOFTOFW_P27TCMI", "SOFTOFW_P27TCM");    \
        EXT_STR_ITEM_INFO_ZZP(                                                                                    \
            ok, si, offset, struct_t, printerr, SOFTOFW_P27TCME, UINT32, "SOFTOFW_P27TCME", "SOFTOFW_P27TCM");    \
        EXT_STR_ITEM_INFO_LIM(ok, si, offset, struct_t, printerr, SOFTOFW_P27TC, UINT32, "SOFTOFW_P27TC", 20);    \
        EXT_STR_ITEM_INFO_ZZP(                                                                                    \
            ok, si, offset, struct_t, printerr, SOFTOFW_P27TCv, UINT32, "SOFTOFW_P27TCv", "SOFTOFW_P27TC");       \
        EXT_STR_ITEM_INFO_LIM(ok, si, offset, struct_t, printerr, SOFTOFW_P27E1, UINT32, "SOFTOFW_P27E1", 65535); \
        EXT_STR_ITEM_INFO_LIM(ok, si, offset, struct_t, printerr, SOFTOFW_P27E2, UINT32, "SOFTOFW_P27E2", 65535); \
        EXT_STR_ITEM_INFO_LIM(ok, si, offset, struct_t, printerr, SOFTOFW_P28TFM, UINT32, "SOFTOFW_P28TFM", 2);   \
        EXT_STR_ITEM_INFO_ZZP(                                                                                    \
            ok, si, offset, struct_t, printerr, SOFTOFW_P28TFMI, UINT32, "SOFTOFW_P28TFMI", "SOFTOFW_P28TFM");    \
        EXT_STR_ITEM_INFO_ZZP(                                                                                    \
            ok, si, offset, struct_t, printerr, SOFTOFW_P28TFME, UINT32, "SOFTOFW_P28TFME", "SOFTOFW_P28TFM");    \
        EXT_STR_ITEM_INFO_LIM(ok, si, offset, struct_t, printerr, SOFTOFW_P28TF, UINT32, "SOFTOFW_P28TF", 20);    \
        EXT_STR_ITEM_INFO_ZZP(                                                                                    \
            ok, si, offset, struct_t, printerr, SOFTOFW_P28TFv, UINT32, "SOFTOFW_P28TFv", "SOFTOFW_P28TF");       \
        EXT_STR_ITEM_INFO_LIM(ok, si, offset, struct_t, printerr, SOFTOFW_P28TCM, UINT32, "SOFTOFW_P28TCM", 2);   \
        EXT_STR_ITEM_INFO_ZZP(                                                                                    \
            ok, si, offset, struct_t, printerr, SOFTOFW_P28TCMI, UINT32, "SOFTOFW_P28TCMI", "SOFTOFW_P28TCM");    \
        EXT_STR_ITEM_INFO_ZZP(                                                                                    \
            ok, si, offset, struct_t, printerr, SOFTOFW_P28TCME, UINT32, "SOFTOFW_P28TCME", "SOFTOFW_P28TCM");    \
        EXT_STR_ITEM_INFO_LIM(ok, si, offset, struct_t, printerr, SOFTOFW_P28TC, UINT32, "SOFTOFW_P28TC", 20);    \
        EXT_STR_ITEM_INFO_ZZP(                                                                                    \
            ok, si, offset, struct_t, printerr, SOFTOFW_P28TCv, UINT32, "SOFTOFW_P28TCv", "SOFTOFW_P28TC");       \
        EXT_STR_ITEM_INFO_LIM(ok, si, offset, struct_t, printerr, SOFTOFW_P28E1, UINT32, "SOFTOFW_P28E1", 65535); \
        EXT_STR_ITEM_INFO_LIM(ok, si, offset, struct_t, printerr, SOFTOFW_P28E2, UINT32, "SOFTOFW_P28E2", 65535); \
                                                                                                                  \
    } while (0);

/********************************************************
 *
 * For internal use by the network data reader:
 * (version checks, etc)
 */

#endif /*__GUARD_H101_SOFTOFW_EXT_H101_H__*/

/*******************************************************/
