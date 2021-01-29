#ifndef R3BSofAtMappedData_H
#define R3BSofAtMappedData_H
#include "TObject.h"

class R3BSofAtMappedData : public TObject
{

  public:
    /** Default constructor **/
    R3BSofAtMappedData();

    /** Constructor with arguments
     *@param secID    Section ID
     *@param energy   Energy deposit [channels]
     **/
    R3BSofAtMappedData(UShort_t anodeID, UShort_t energy, UShort_t time, Bool_t pu, Bool_t ov);

    /** Destructor **/
    virtual ~R3BSofAtMappedData() {}

    /** Accessors **/
    inline const UShort_t& GetAnodeID() const { return fAnodeID; }
    inline const UShort_t& GetEnergy() const { return fEnergy; }
    inline const UShort_t& GetTime() const { return fTime; }
    inline const Bool_t& GetPileupStatus() const { return fPileup; }
    inline const Bool_t& GetOverflowStatus() const { return fOverflow; }

    /** Modifiers **/
    void SetAnodeID(UShort_t id) { fAnodeID = id; };
    void SetEnergy(UShort_t energy) { fEnergy = energy; };
    void SetTime(UShort_t time) { fTime = time; };
    void SetPileup(Bool_t pu) { fPileup = pu; }
    void SetOverflow(Bool_t ov) { fOverflow = ov; }

  protected:
    UShort_t fAnodeID;
    UShort_t fEnergy;
    UShort_t fTime;
    Bool_t fPileup;
    Bool_t fOverflow;

    ClassDef(R3BSofAtMappedData, 1)
};

#endif
