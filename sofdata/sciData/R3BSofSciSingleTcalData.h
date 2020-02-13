#ifndef R3BSOFSCISINGLETCALITEM_H
#define R3BSOFSCISINGLETCALITEM_H

#include "TObject.h"
#include "FairLogger.h"

#include "detectors_cfg.h"


class R3BSofSciSingleTcalData : public TObject
{
  public:
    // Default Constructor
    R3BSofSciSingleTcalData();

    // Destructor
    virtual ~R3BSofSciSingleTcalData() {}

    // Getters
    inline const Double_t& GetRawTimeNs(UShort_t det) const { return fRawTimeNs[det-1]; }
    inline const Double_t& GetRawPosNs(UShort_t det)  const { return fRawPosNs[det-1]; }
#ifdef NUMBER_OF_SOFSCI_TOF
    inline const Double_t& GetRawTofNs(UShort_t rank) const { return fRawTofNs[rank];}
#endif

    // Modifiers
    void SetRawTimeNs(UShort_t det, Double_t time)  {fRawTimeNs[det-1]=time;}
    void SetRawPosNs(UShort_t det, Double_t pos)    {fRawPosNs[det-1]=pos;}
    void SetMultPerDet(UShort_t det, UShort_t m)    {fMultPerDet[det-1]=m;}
#ifdef NUMBER_OF_SOFSCI_TOF
    void SetRawTofNs(UShort_t rank, Double_t tof)   {fRawTofNs[rank]=tof;} 
    void SetMultPerToF(UShort_t rank, UShort_t m)    {fMultPerDet[rank]=m;} 
#endif

  private:
    Double_t fRawTimeNs[NUMBER_OF_SOFSCI_DETECTORS];           // 0.5*(Tleft + Tright)
    Double_t fRawPosNs[NUMBER_OF_SOFSCI_DETECTORS];            // Tleft - Tright
    UShort_t fMultPerDet[NUMBER_OF_SOFSCI_DETECTORS];        // number of hits with the proper pos 
#ifdef NUMBER_OF_SOFSCI_TOF
    Double_t fRawTofNs[NUMBER_OF_SOFSCI_TOF];
    UShort_t fMultPerTof[NUMBER_OF_SOFSCI_TOF];              // number of hits with the proper tof 
#endif
  public:
    ClassDef(R3BSofSciSingleTcalData, 2)
};

#endif
