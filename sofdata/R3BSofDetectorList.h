// -------------------------------------------------------------------------
// -----                  R3BSofDetectorList.header file                  -----
// -----                  Created 06/04/17 by H. Alvarez                  -----
// -------------------------------------------------------------------------

/** Unique identifier for all sofia detector systems **/

#ifndef R3BSOFDETECTORLIST_H
#define R3BSOFDETECTORLIST_H 1

enum DetectorId
{
    kSOFAT,
    kSOFMWPC,
    kSOFTWIM,
    kSOFTRIM,
    kSOFTOF,
    kLAST
};

/** Unique identifier for all R3B Point and Hit types **/

enum fDetectorType
{

    kSofATPoint,
    kSofMWPCPoint,
    kSofTWIMPoint,
    kSofTRIMPoint,
    kSofTOFPoint
};

enum SensorSide
{
    kTOP,
    kBOTTOM
};

#endif
