#ifndef R3BSofTrimMappedData_H
#define R3BSofTrimMappedData_H
#include "TObject.h"

class R3BSofTrimMappedData : public TObject
{

  public:
    /** Default constructor **/
    R3BSofTrimMappedData();

    /** Constructor with arguments
    secID   is 1-based
    anodeID is 1-based
    time    is the RawTime needs to be subtracted later from Tref to get the drift time
    energy  is the energy loss
    pu      pile-up bit
    ov      overflow bit
    **/

    R3BSofTrimMappedData(UShort_t secID, UShort_t anodeID, UShort_t time, UShort_t energy, Bool_t pu, Bool_t ov);

    /** Destructor **/
    virtual ~R3BSofTrimMappedData() {}

    /** Accessors **/
    inline const UShort_t& GetSecID() const { return fSecID; }
    inline const UShort_t& GetAnodeID() const { return fAnodeID; }
    inline const UShort_t& GetTime() const { return fTime; }
    inline const UShort_t& GetEnergy() const { return fEnergy; }
    inline const Bool_t& GetPileupStatus() const { return fPileup; }
    inline const Bool_t& GetOverflowStatus() const { return fOverflow; }

    /** Modifiers **/
    void SetSecID(UShort_t id) { fSecID = id; };
    void SetAnodeID(UShort_t id) { fAnodeID = id; };
    void SetDTime(UShort_t time) { fTime = time; };
    void SetEnergy(UShort_t energy) { fEnergy = energy; };
    void SetPileup(Bool_t pu) { fPileup = pu; }
    void SetOverflow(Bool_t ov) { fOverflow = ov; }

  protected:
    UShort_t fSecID;
    UShort_t fAnodeID;
    UShort_t fTime;
    UShort_t fEnergy;
    Bool_t fPileup;
    Bool_t fOverflow;

    ClassDef(R3BSofTrimMappedData, 1)
};

#endif
