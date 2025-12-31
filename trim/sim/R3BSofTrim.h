/******************************************************************************
 *   Copyright (C) 2017 GSI Helmholtzzentrum für Schwerionenforschung GmbH    *
 *   Copyright (C) 2017-2026 Members of R3B Collaboration                     *
 *                                                                            *
 *             This software is distributed under the terms of the            *
 *                 GNU General Public Licence (GPL) version 3,                *
 *                    copied verbatim in the file "LICENSE".                  *
 *                                                                            *
 * In applying this license GSI does not waive the privileges and immunities  *
 * granted to it by virtue of its status as an Intergovernmental Organization *
 * or submit itself to any jurisdiction.                                      *
 ******************************************************************************/

#pragma once

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
    bool ProcessHits(FairVolume* vol = 0) override;

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
    TClonesArray* GetCollection(int iColl) const override;

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

    bool CheckIfSensitive(std::string name) override;

    void Initialize() override;

  private:
    /** Track information to be stored until the track leaves the
        active volume. **/
    int fTrackID;  //!  track index
    int fTrackPID; //!  particle identification
    int fVolumeID; //!  volume id
    int fDetCopyID;
    double fZ;
    double fA;
    int fUniqueID;                  //!  particle unique id (e.g. if Delta electron, fUniqueID=9)
    TLorentzVector fPosIn, fPosOut; //!  position
    TLorentzVector fMomIn, fMomOut; //!  momentum
    double fTime;                   //!  time
    double fLength;                 //!  length
    double fELoss;                  //!  energy loss
    int fNSteps;                    //!  Number of steps in the active volume

    TClonesArray* fSofTRIMCollection; //!  The point collection

    /** Private method AddHit
     **
     ** Adds a SofTRIMPoint to the HitCollection
     **/
    R3BSofTrimPoint* AddPoint(int trackID,
                              int detID,
                              int detCopyID,
                              double zf,
                              double af,
                              TVector3 posIn,
                              TVector3 pos_out,
                              TVector3 momIn,
                              TVector3 momOut,
                              double time,
                              double length,
                              double eLoss);

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
