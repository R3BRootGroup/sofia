#ifndef R3BSofSci_H
#define R3BSofSci_H

#include "R3BDetector.h"
#include "TLorentzVector.h"

class TClonesArray;
class R3BSofSciPoint;
class FairVolume;
class TGeoRotation;

class R3BSofSci : public R3BDetector
{
  public:
    R3BSofSci();
    /** Standard constructor.
     *@param geoFile name of the ROOT geometry file
     *@param trans   position
     *@param rot     rotation
     */
    R3BSofSci(const TString& geoFile, const TGeoTranslation& trans, const TGeoRotation& rot = TGeoRotation());

    /** Standard constructor.
     *@param geoFile name of the ROOT geometry file
     *@param combi   position + rotation
     */
    R3BSofSci(const TString& geoFile, const TGeoCombiTrans& combi = TGeoCombiTrans());

    /** Destructor **/
    ~R3BSofSci();

    /** Virtual method ProcessHits
     **
     ** Defines the action to be taken when a step is inside the
     ** active volume. Creates a R3BSofSciPoint and adds it
     ** to the collection.
     *@param vol  Pointer to the active volume
     **/
    virtual Bool_t ProcessHits(FairVolume* vol = 0);

    /** Virtual method BeginEvent
     **
     ** Actions at the begin of the event
     **/
    virtual void BeginEvent();

    /** Virtual method EndOfEvent
     **
     ** If verbosity level is set, print hit collection at the
     ** end of the event and resets it afterwards.
     **/
    virtual void EndOfEvent();

    /** Virtual method Register
     **
     ** Registers the hit collection in the ROOT manager.
     **/
    virtual void Register();

    /** Accessor to the hit collection **/
    virtual TClonesArray* GetCollection(Int_t iColl) const;

    /** Virtual method Print
     **
     ** Screen output of hit collection.
     **/
    virtual void Print(Option_t* option = "") const;

    /** Virtual method Reset
     **
     ** Clears the hit collection
     **/
    virtual void Reset();

    /** Virtual method CopyClones
     **
     ** Copies the hit collection with a given track index offset
     *@param cl1     Origin
     *@param cl2     Target
     *@param offset  Index offset
     **/
    virtual void CopyClones(TClonesArray* cl1, TClonesArray* cl2, Int_t offset);

    virtual Bool_t CheckIfSensitive(std::string name);

    virtual void Initialize();
    virtual void SetSpecialPhysicsCuts() {}

  private:
    /** Track information to be stored until the track leaves the
        active volume. **/
    Int_t fTrackID;  //!  track index
    Int_t fTrackPID; //!  particle identification
    Int_t fDetCopyID;
    Int_t fVolumeID;                //!  volume id
    Int_t fUniqueID;                //!  particle unique id (e.g. if Delta electron, fUniqueID=9)
    TLorentzVector fPosIn, fPosOut; //!  position
    TLorentzVector fMomIn, fMomOut; //!  momentum
    Double32_t fTime;               //!  time
    Double32_t fLength;             //!  length
    Double32_t fELoss;              //!  energy loss
    Int_t fPosIndex;                //!
    Int_t fNSteps;                  //!  Number of steps in the active volume
    Bool_t kGeoSaved;               //!
    TList* flGeoPar;                //!

    TClonesArray* fSofSCICollection; //!  The point collection

    /** Private method AddHit
     **
     ** Adds a SofSCIPoint to the HitCollection
     **/
    R3BSofSciPoint* AddPoint(Int_t trackID,
                             Int_t detID,
                             Int_t detCopyID,
                             TVector3 posIn,
                             TVector3 pos_out,
                             TVector3 momIn,
                             TVector3 momOut,
                             Double_t time,
                             Double_t length,
                             Double_t eLoss);

    /** Private method ResetParameters
     **
     ** Resets the private members for the track parameters
     **/
    void ResetParameters();

    ClassDef(R3BSofSci, 1);
};

inline void R3BSofSci::ResetParameters()
{
    fTrackID = fVolumeID = fDetCopyID = fTrackPID = fUniqueID = 0;
    fPosIn.SetXYZM(0.0, 0.0, 0.0, 0.0);
    fPosOut.SetXYZM(0.0, 0.0, 0.0, 0.0);
    fMomIn.SetXYZM(0.0, 0.0, 0.0, 0.0);
    fMomOut.SetXYZM(0.0, 0.0, 0.0, 0.0);
    fTime = fLength = fELoss = 0;
    fPosIndex = 0;
    fNSteps = 0;
};

#endif
