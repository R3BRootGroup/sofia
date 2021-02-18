// ----------------------------------------------------------------------
// -----                     R3BSofMwpc3Cal2Hit                     -----
// -----             Created 14/10/19  by G. García Jiménez         -----
// -----             by modifying J.L classes for MWPC0             -----
// ----------------------------------------------------------------------

#ifndef R3BSOFMWPC3CAL2HIT_H
#define R3BSOFMWPC3CAL2HIT_H

#include "FairTask.h"
#include "R3BSofMwpcCalData.h"
#include "R3BSofMwpcHitData.h"
#include "R3BSofTofWHitData.h"
#include "TH1F.h"
#include <TRandom.h>

using namespace std;

#define Mw3PadsX 288
#define Mw3PadsY 120

// bool compare(pair<int,int> p1, pair<int,int> p2) {return p1.second<p2.second;}

class TClonesArray;

class R3BSofMwpc3Cal2Hit : public FairTask
{

  public:
    /* Default Constructor */
    R3BSofMwpc3Cal2Hit();

    /** Standard constructor **/
    R3BSofMwpc3Cal2Hit(const char* name, Int_t iVerbose = 1);

    /** Destructor **/
    virtual ~R3BSofMwpc3Cal2Hit();

    /** Virtual method Exec **/
    virtual void Exec(Option_t* option);

    /** Virtual method Reset **/
    virtual void Reset();

    // Fair specific
    /** Virtual method Init **/
    virtual InitStatus Init();

    /** Virtual method ReInit **/
    virtual InitStatus ReInit();

    /** Virtual method Finish **/
    virtual void Finish();

    void SetOnline(Bool_t option) { fOnline = option; }

  private:
    Double_t fSizeX; // Detector size in X and Y
    Double_t fSizeY; // Detector size in X and Y
    Double_t fwx;    // Pad width in X
    Double_t fwy;    // Pad width in Y
    Int_t fx[Mw3PadsX], fy[Mw3PadsY];
    vector<int> fQX;
    vector<int> fQY;
    vector<int> fPadX;
    vector<int> fPadY;
    vector<pair<int, int>> fPairX;
    vector<pair<int, int>> fPairY;
    vector<vector<pair<int, int>>> fClusterX;
    vector<vector<pair<int, int>>> fClusterY;

    double fThresholdX;
    double fThresholdY;
    Bool_t fOnline;          // Don't store data for online
    Bool_t fTofWallMatching; // boolean to know if position matching with the tofwall is required

    TClonesArray* fMwpcCalDataCA;    /**< Array with Cal input data. >*/
    TClonesArray* fMwpcHitDataCA;    /**< Array with Hit output data. >*/
    TClonesArray* fTofWallHitDataCA; /**< Array with Hit output data. >*/

    /** Private method AddHitData **/

    // Adds a SofMwpcHitData to the MwpcHitCollection
    R3BSofMwpcHitData* AddHitData(Double_t x, Double_t y);

    /** **/
    void TofWallMatching(vector<double>, vector<double>, vector<double>, vector<double>);
    /** **/
    double GetChargeMax(vector<pair<int, int>> p1);
    /** **/
    vector<pair<int, int>> FindCluster(vector<pair<int, int>>);
    /** **/
    void ReconstructHitWithTofWallMatching();
    /** Private method to obtain the position X **/
    Double_t GetPositionX(Double_t qmax, Int_t padmax, Double_t qleft, Double_t qright);
    /** Private method to obtain the position Y **/
    Double_t GetPositionY(Double_t qmax, Int_t padmax, Double_t qdown, Double_t qup);

  public:
    // Class definition
    ClassDef(R3BSofMwpc3Cal2Hit, 1)
};

#endif
