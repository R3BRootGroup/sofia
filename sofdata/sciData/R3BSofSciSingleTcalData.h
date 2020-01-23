#ifndef R3BSOFSCISINGLETCALITEM_H
#define R3BSOFSCISINGLETCALITEM_H

#include "TObject.h"
#include "FairLogger.h"

#define NUMBER_OF_DETECTORS 1

class R3BSofSciSingleTcalData : public TObject
{
  public:
    // Default Constructor
    R3BSofSciSingleTcalData();

    // Standard Constructor
    R3BSofSciSingleTcalData(Double_t t, Double_t p, Double_t m);

    // Destructor
    virtual ~R3BSofSciSingleTcalData() {}

    // Getters
    inline const Double_t& GetRawTimeNs(UShort_t det)       const { return fRawTimeNs[det-1]; }
    inline const Double_t& GetRawPosNs(UShort_t det)        const { return fRawPosNs[det-1]; }
//#if NUMBER_OF_DETECTORS==2
    inline const Double_t& GetRawTofNs()                    const { return fRawTofNs;}
//#endif
    //#if NUMBER_OF_DETECTORS>1
    //inline const Double_t& GetRawTofNs(rank)        const { return fRawTofNs[rank];}
    //#endif

    // Modifiers
    void SetRawTimeNs(UShort_t det, Double_t time)  {fRawTimeNs[det-1]=time;}
    void SetRawPosNs(UShort_t det, Double_t pos)    {fRawPosNs[det-1]=pos;}
//#if NUMBER_OF_DETECTORS==2
    void SetRawTofNs(Double_t tof)                  {fRawTofNs=tof;}
//#endif
    void SetMultPerDet(UShort_t det, UShort_t m)    {fMultPerDet[det-1]=m;}

  private:
    Double_t fRawTimeNs[NUMBER_OF_DETECTORS];           // 0.5*(Tleft + Tright)
    Double_t fRawPosNs[NUMBER_OF_DETECTORS];            // Tleft - Tright
//#if NUMBER_OF_DETECTORS==2
    Double_t fRawTofNs;
//#endif
    //#if NUMBER_OF_DETECTORS>1
    //Double_t fRawTofNs[Fact(n)/(Fact(2)*Fact(n-2))]
    //#endif
    UShort_t fMultPerDet[NUMBER_OF_DETECTORS];        // number of hits with the proper pos (and ToF is NUMBER_OF_DETECTORS==2)
  public:
    ClassDef(R3BSofSciSingleTcalData, 2)
};

#endif
