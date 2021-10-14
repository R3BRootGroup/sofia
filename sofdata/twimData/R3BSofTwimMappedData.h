#ifndef R3BSofTwimMappedData_H
#define R3BSofTwimMappedData_H
#include "TObject.h"

class R3BSofTwimMappedData : public TObject
{

  public:
    /** Default constructor **/
    R3BSofTwimMappedData();

    /** Constructor with arguments
     *@param secID    Section ID attention 0-based !
     *@param anodeID  Anode ID attention 0-based !
     *@param dtime    Time [channels]
     *@param eLoss    Energy deposit [channels]
     **/
    R3BSofTwimMappedData(Int_t secID, Int_t anodeID, Int_t time, Int_t energy, Bool_t pu, Bool_t ov);

    /** Destructor **/
    virtual ~R3BSofTwimMappedData() {}

    /** Accessors **/
    inline const Int_t& GetSecID() const { return fSecID; }
    inline const Int_t& GetAnodeID() const { return fAnodeID; }
    inline const Int_t& GetTime() const { return fTime; }
    inline const Int_t& GetEnergy() const { return fEnergy; }
    inline const Bool_t& GetPileupStatus() const { return fPileup; }
    inline const Bool_t& GetOverflowStatus() const { return fOverflow; }

    /** Modifiers **/
    void SetSecID(Int_t id) { fSecID = id; };
    void SetAnodeID(Int_t id) { fAnodeID = id; };
    void SetDTime(Int_t time) { fTime = time; };
    void SetEnergy(Int_t energy) { fEnergy = energy; };
    void SetPileup(Bool_t pu) { fPileup = pu; }
    void SetOverflow(Bool_t ov) { fOverflow = ov; }

  protected:
    Int_t fSecID;
    Int_t fAnodeID;
    Int_t fTime;
    Int_t fEnergy;
    Bool_t fPileup;
    Bool_t fOverflow;

  public:
    ClassDef(R3BSofTwimMappedData, 1)
};

#endif
