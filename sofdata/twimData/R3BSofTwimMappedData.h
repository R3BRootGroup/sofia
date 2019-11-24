#ifndef R3BSofTwimMappedData_H
#define R3BSofTwimMappedData_H
#include "TObject.h"

class R3BSofTwimMappedData : public TObject
{

  public:
    /** Default constructor **/
    R3BSofTwimMappedData();

    /** Constructor with arguments
     *@param secID    Section ID
     *@param anodeID  Anode ID
     *@param dtime    Time [channels]
     *@param eLoss    Energy deposit [channels]
     **/
    R3BSofTwimMappedData(UShort_t secID, UShort_t anodeID, UShort_t time, UShort_t energy);

    /** Destructor **/
    virtual ~R3BSofTwimMappedData() {}

    /** Accessors **/
    inline const UShort_t& GetSecID() const { return fSecID; }
    inline const UShort_t& GetAnodeID() const { return fAnodeID; }
    inline const UShort_t& GetTime() const { return fTime; }
    inline const UShort_t& GetEnergy() const { return fEnergy; }

    /** Modifiers **/
    void SetSecID(UShort_t id) { fSecID = id; };
    void SetAnodeID(UShort_t id) { fAnodeID = id; };
    void SetDTime(UShort_t time) { fTime = time; };
    void SetEnergy(UShort_t energy) { fEnergy = energy; };

  protected:
    UShort_t fSecID;
    UShort_t fAnodeID;
    UShort_t fTime;
    UShort_t fEnergy;

    ClassDef(R3BSofTwimMappedData, 1)
};

#endif
