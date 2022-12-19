#ifndef R3BSofTofW_H
#define R3BSofTofW_H

#include "R3BDetector.h"
#include "TLorentzVector.h"

class TClonesArray;
class R3BSofTofWPoint;
class FairVolume;
class TGeoRotation;

class R3BSofTofW : public R3BDetector
{
  public:
    /** Default constructor */
    R3BSofTofW();

    /** Standard constructor.
     *@param geoFile name of the ROOT geometry file
     *@param trans   position
     *@param rot     rotation
     */
    R3BSofTofW(const TString& geoFile, const TGeoTranslation& trans, const TGeoRotation& rot = TGeoRotation());

    /** Standard constructor.
     *@param geoFile name of the ROOT geometry file
     *@param combi   position + rotation
     */
    R3BSofTofW(const TString& geoFile, const TGeoCombiTrans& combi = TGeoCombiTrans());

    /** Destructor **/
    ~R3BSofTofW() override;

    /** Virtual method ProcessHits
     **
     ** Defines the action to be taken when a step is inside the
     ** active volume. Creates a R3BSofTofWPoint and adds it
     ** to the collection.
     *@param vol  Pointer to the active volume
     **/
    Bool_t ProcessHits(FairVolume* vol = 0) override;

    /** Virtual method EndOfEvent
     **
     ** If verbosity level is set, print hit collection at the
     ** end of the event and resets it afterwards.
     **/
    void EndOfEvent() override;

    /** Virtual method Register
     **
     ** Registers the hit collection in the ROOT manager.
     **/
    void Register() override;

    /** Gets the produced collections */
    TClonesArray* GetCollection(Int_t iColl) const override;

    /** Virtual method Print
     **
     ** Screen output of hit collection.
     **/
    void Print(Option_t* option = "") const override;

    /**      has to be called after each event to reset the containers      */
    void Reset() override;

    Bool_t CheckIfSensitive(std::string name) override;

    void Initialize() override;

    void SetSpecialPhysicsCuts() override { ; }

  private:
    /** Track information to be stored until the track leaves the
        active volume. **/
    Int_t fTrackID;                 //!  track index
    Int_t fTrackPID;                //!  particle identification
    Int_t fVolumeID;                //!  volume id
    Int_t fDetCopyID;               //!  Det volume id
    Double_t fZ;                    //!  atomic number fragment
    Double_t fA;                    //!  mass number fragment
    TLorentzVector fPosIn, fPosOut; //!  position
    TLorentzVector fMomIn, fMomOut; //!  momentum
    Double32_t fTime;               //!  time
    Double32_t fLength;             //!  length
    Double32_t fELoss;              //!  energy loss
    Int_t fPosIndex;                //!

    TClonesArray* fSofTofWallCollection; //  The point collection

    /** Private method AddPoint
     **
     ** Adds a SofTofWallPoint to the HitCollection
     **/
    R3BSofTofWPoint* AddPoint(Int_t trackID,
                              Int_t detID,
                              Int_t volid,
                              Double_t Z,
                              Double_t A,
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

    R3BSofTofW(const R3BSofTofW&);
    R3BSofTofW& operator=(const R3BSofTofW&);

  public:
    ClassDefOverride(R3BSofTofW, 1);
};

inline void R3BSofTofW::ResetParameters()
{
    fTrackID = fVolumeID = fTrackPID;
    fPosIn.SetXYZM(0.0, 0.0, 0.0, 0.0);
    fPosOut.SetXYZM(0.0, 0.0, 0.0, 0.0);
    fMomIn.SetXYZM(0.0, 0.0, 0.0, 0.0);
    fMomOut.SetXYZM(0.0, 0.0, 0.0, 0.0);
    fTime = fLength = fELoss = fZ = fA = 0;
    fPosIndex = 0;
};

#endif /* R3BSofTofW_H */
