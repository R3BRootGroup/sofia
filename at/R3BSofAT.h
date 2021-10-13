#ifndef R3BSOFAT_H
#define R3BSOFAT_H

#include "R3BDetector.h"
#include "TLorentzVector.h"
#include <map>

class TClonesArray;
class R3BSofATPoint;
class FairVolume;
class TGeoRotation;

class R3BSofAT : public R3BDetector
{
  public:
    /** Default constructor */
    R3BSofAT();

    /** Standard constructor.
     *@param geoFile name of the ROOT geometry file
     *@param trans   position
     *@param rot     rotation
     */
    R3BSofAT(const TString& geoFile, const TGeoTranslation& trans, const TGeoRotation& rot = TGeoRotation());

    /** Standard constructor.
     *@param geoFile name of the ROOT geometry file
     *@param combi   position + rotation
     */
    R3BSofAT(const TString& geoFile, const TGeoCombiTrans& combi = TGeoCombiTrans());

    /** Destructor **/
    ~R3BSofAT();

    /** Virtual method ProcessHits
     **
     ** Defines the action to be taken when a step is inside the
     ** active volume. Creates a R3BSofATPoint and adds it
     ** to the collection.
     *@param vol  Pointer to the active volume
     **/
    virtual Bool_t ProcessHits(FairVolume* vol = 0);

    /** Virtual method BeginEvent
     **
     ** Actions at the begin of the event.
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

    /** Public method SetNonUniformity
     **
     ** Defines the fNonUniformity parameter in % deviation from the central value
     *@param nonU  Double parameter setting the maximum non-uniformity allowed
     **/

    virtual void Initialize();
    virtual void SetSpecialPhysicsCuts() {}

    //  void SaveGeoParams();

  private:
    /** Track information to be stored until the track leaves the
        active volume. **/
    Int_t fTrackID;                 //!  track index
    Int_t fTrackPID;                //!  particle identification
    Int_t fVolumeID;                //!  volume id
    Int_t fParentTrackID;           //!  parent track index
    Int_t fUniqueID;                //!  particle unique id (e.g. if Delta electron, fUniqueID=9)
    TLorentzVector fPosIn, fPosOut; //!  position
    TLorentzVector fMomIn, fMomOut; //!  momentum
    Double32_t fTime;               //!  time
    Double32_t fLength;             //!  length
    Double32_t fELoss;              //!  energy loss
    Double32_t fNf;                 //!  fast CsI(Tl) amplitude
    Double32_t fNs;                 //!  slow CsI(Tl) amplitude
    Int_t fPosIndex;                //!
    Int_t fNSteps;                  //!  Number of steps in the active volume
    Double32_t fEinc;               //!  Total incident energy
    Bool_t kGeoSaved;               //!
    TList* flGeoPar;                //!

    TClonesArray* fSofATCollection; //!  The point collection

    /** Private method AddHit
     **
     ** Adds a SofATPoint to the HitCollection
     **/
    R3BSofATPoint* AddPoint(Int_t trackID,
                            Int_t detID,
                            Int_t volid,
                            Int_t copy,
                            Int_t ident,
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

    TGeoRotation* createMatrix(Double_t phi, Double_t theta, Double_t psi);

    ClassDef(R3BSofAT, 2);
};

inline void R3BSofAT::ResetParameters()
{
    fTrackID = fVolumeID = fParentTrackID = fTrackPID = fUniqueID = 0;
    fPosIn.SetXYZM(0.0, 0.0, 0.0, 0.0);
    fPosOut.SetXYZM(0.0, 0.0, 0.0, 0.0);
    fMomIn.SetXYZM(0.0, 0.0, 0.0, 0.0);
    fMomOut.SetXYZM(0.0, 0.0, 0.0, 0.0);
    fTime = fLength = fELoss = fNf = fNs = fEinc = 0;
    fPosIndex = 0;
    fNSteps = 0;
};

#endif
