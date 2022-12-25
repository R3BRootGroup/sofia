#ifndef R3BSofTrim_H
#define R3BSofTrim_H 1

#include "R3BDetector.h"
#include "TLorentzVector.h"

class TClonesArray;
class R3BSofTrimPoint;
class FairVolume;

class R3BSofTrim : public R3BDetector
{
  public:
    R3BSofTrim();
    /** Standard constructor.
     *@param geoFile name of the ROOT geometry file
     *@param trans   position
     *@param rot     rotation
     */
    R3BSofTrim(const TString& geoFile, const TGeoTranslation& trans, const TGeoRotation& rot = TGeoRotation());

    /** Standard constructor.
     *@param geoFile name of the ROOT geometry file
     *@param combi   position + rotation
     */
    R3BSofTrim(const TString& geoFile, const TGeoCombiTrans& combi = TGeoCombiTrans());

    /** Destructor **/
    ~R3BSofTrim() override;

    /**  method ProcessHits
     **
     ** Defines the action to be taken when a step is inside the
     ** active volume. Creates a R3BSofTrimPoint and adds it
     ** to the collection.
     *@param vol  Pointer to the active volume
     **/
    Bool_t ProcessHits(FairVolume* vol = 0) override;

    /**  method EndOfEvent
     **
     ** If verbosity level is set, print hit collection at the
     ** end of the event and resets it afterwards.
     **/
    void EndOfEvent() override;

    /**  method Register
     **
     ** Registers the hit collection in the ROOT manager.
     **/
    void Register() override;

    /** Accessor to the hit collection **/
    TClonesArray* GetCollection(Int_t iColl) const override;

    /**  method Print
     **
     ** Screen output of hit collection.
     **/
    void Print(Option_t* option = "") const override;

    /**  method Reset
     **
     ** Clears the hit collection
     **/
    void Reset() override;

    Bool_t CheckIfSensitive(std::string name) override;

    void Initialize() override;

  private:
    /** Track information to be stored until the track leaves the
        active volume. **/
    Int_t fTrackID;  //!  track index
    Int_t fTrackPID; //!  particle identification
    Int_t fVolumeID; //!  volume id
    Int_t fDetCopyID;
    Double_t fZ;
    Double_t fA;
    Int_t fUniqueID;                //!  particle unique id (e.g. if Delta electron, fUniqueID=9)
    TLorentzVector fPosIn, fPosOut; //!  position
    TLorentzVector fMomIn, fMomOut; //!  momentum
    Double32_t fTime;               //!  time
    Double32_t fLength;             //!  length
    Double32_t fELoss;              //!  energy loss
    Int_t fNSteps;                  //!  Number of steps in the active volume

    TClonesArray* fSofTRIMCollection; //!  The point collection

    /** Private method AddHit
     **
     ** Adds a SofTRIMPoint to the HitCollection
     **/
    R3BSofTrimPoint* AddPoint(Int_t trackID,
                              Int_t detID,
                              Int_t detCopyID,
                              Double_t zf,
                              Double_t af,
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

  public:
    ClassDefOverride(R3BSofTrim, 2);
};

inline void R3BSofTrim::ResetParameters()
{
    fTrackID = fVolumeID = fDetCopyID = fTrackPID = fUniqueID = 0;
    fPosIn.SetXYZM(0.0, 0.0, 0.0, 0.0);
    fPosOut.SetXYZM(0.0, 0.0, 0.0, 0.0);
    fMomIn.SetXYZM(0.0, 0.0, 0.0, 0.0);
    fMomOut.SetXYZM(0.0, 0.0, 0.0, 0.0);
    fTime = fLength = fZ = fA = fELoss = 0.;
    fNSteps = 0;
};

#endif
