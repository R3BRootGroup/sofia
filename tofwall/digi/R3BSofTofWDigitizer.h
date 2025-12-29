/******************************************************************************
 *   Copyright (C) 2019 GSI Helmholtzzentrum für Schwerionenforschung GmbH    *
 *   Copyright (C) 2019-2026 Members of R3B Collaboration                     *
 *                                                                            *
 *             This software is distributed under the terms of the            *
 *                 GNU General Public Licence (GPL) version 3,                *
 *                    copied verbatim in the file "LICENSE".                  *
 *                                                                            *
 * In applying this license GSI does not waive the privileges and immunities  *
 * granted to it by virtue of its status as an Intergovernmental Organization *
 * or submit itself to any jurisdiction.                                      *
 ******************************************************************************/

// ----------------------------------------------------------------
// -----          R3BSofTofWDigitizer source file             -----
// -----         Created 03/11/19  by JL Rodriguez            -----
// ----------------------------------------------------------------

#pragma once

#include "R3BSofTofWHitData.h"

#include <FairTask.h>

#include <TRandom3.h>
#include <TRotation.h>
#include <TVector3.h>
#include <map>
#include <string>

class TClonesArray;
class R3BTGeoPar;

class R3BSofTofWDigitizer : public FairTask
{

  public:
    /** Default constructor **/
    R3BSofTofWDigitizer();

    /** Standard constructor **/
    R3BSofTofWDigitizer(const TString& name, int iVerbose = 1);

    /** Destructor **/
    ~R3BSofTofWDigitizer();

    /** Method Init **/
    InitStatus Init() override;

    /** Method ReInit **/
    InitStatus ReInit() override;

    /** Method Exec **/
    void Exec(Option_t*) override;

    // Fair specific
    void SetParContainers() override;

    void Reset();

    /** Setters for sigmas **/
    void SetSigma_t(double sigma_t) { fsigma_t = sigma_t; }
    void SetSigma_ELoss(double sigma_ELoss) { fsigma_ELoss = sigma_ELoss; }

  private:
    void SetParameter();

    TClonesArray* fMCTrack = nullptr;
    TClonesArray* fTofPoints = nullptr;
    TClonesArray* fTofHits = nullptr;
    R3BTGeoPar* fTofWGeoPar = nullptr;

    TRandom3* rand;

    double fsigma_y = 1.;    // mm
    double fsigma_t = 0.017; // ns
    double fsigma_ELoss = 0.;

    TVector3 fTrans;
    TRotation fRot;

    /** Private method AddHitData **/
    // Adds a R3BSofTofWHitData to the TofWHitCollection
    R3BSofTofWHitData* AddHitData(int paddle, double x, double y, double time);

  public:
    // Class definition
    ClassDefOverride(R3BSofTofWDigitizer, 2);
};
