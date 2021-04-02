#ifndef __R3BSOFTCALPAR_H__
#define __R3BSOFTCALPAR_H__

#include "FairParGenericSet.h" // for FairParGenericSet

#include "TArrayF.h"
#include "TObjArray.h"
#include "TObject.h"
#include <TObjString.h>

class FairParamList;

class R3BSofTcalPar : public FairParGenericSet
{

  public:
    /** Standard constructor **/
    R3BSofTcalPar(const char* name = "SofTcalPar",
                  const char* title = "SofTcal Parameters",
                  const char* context = "SofTcalParContext");

    /** Destructor **/
    virtual ~R3BSofTcalPar();

    /** Method to reset all parameters **/
    virtual void clear();

    /** Method to store all parameters using FairRuntimeDB **/
    virtual void putParams(FairParamList* list);

    /** Method to retrieve all parameters using FairRuntimeDB**/
    Bool_t getParams(FairParamList* list);

    /** Method to print values of parameters to the standard output **/
    void printParams();

    /** Accessor functions **/

		// === number of detectors === // 
    //  = number of plastics in the ToFW = 28
    //  = number of SofSci in the setup 1 for primary, at least 2 for secondary
    const Double_t GetNumDetectors() { return fNumDetectors; }
    void SetNumDetectors(Int_t NumberOfDetectors) { fNumDetectors = NumberOfDetectors; }

		// === number of channels === //
    //   = 2 for Tofw (up, down) , 3 for SofSci (right, left, Tref)
    const Double_t GetNumChannels() { return fNumChannels; }
    void SetNumChannels(Int_t NumberOfChannels) { fNumChannels = NumberOfChannels; }

    // === number of signals for vftx tcal calibration === //
    //   = number of channels*number of detectors
    //void SetNumSignals(Int_t nd, Int_t nc) { fNumSignals = nd * nc ; }
    //void SetNumSignals() { fNumSignals = fNumDetectors * fNumChannels;}
		//const Double_t GetNumSignals() { return fNumSignals; }

		// === number of parameters per signal for vftx tcal calibration === //
    //   = 1000
    void SetNumTcalParsPerSignal(Int_t n) { fNumTcalParsPerSignal = n; }
    const Double_t GetNumTcalParsPerSignal() { return fNumTcalParsPerSignal; }

		// === VFTX tcal parameters === //
    void SetSignalTcalParams(Double_t ft_ns, UInt_t rank) 
    { 
				fAllSignalsTcalParams->AddAt(ft_ns, rank); 
		}
    TArrayF* GetAllSignalsTcalParams() { return fAllSignalsTcalParams; }
    Double_t GetSignalTcalParams(UInt_t rank) 
		{ 
				return (Double_t)fAllSignalsTcalParams->GetAt(rank); 
		}

		// === Clock offset parameters === //
    void SetClockOffset(Double_t offset, UInt_t rank) 
    { 
				fAllClockOffsets->AddAt(offset, rank); 
		}
    TArrayF* GetAllClockOffsets() { return fAllClockOffsets; }
    Double_t GetClockOffset(UInt_t rank) 
		{ 
				return (Double_t)fAllClockOffsets->GetAt(rank); 
		}

    /** Create more Methods if you need them! **/

  private:
		TArrayF* fAllClockOffsets;
    TArrayF* fAllSignalsTcalParams; // Calibration Parameters for all signals of one detector
    Int_t fNumDetectors;            // number of detectors (=2 for Sci, =28 for TofW)
    Int_t fNumChannels;             // number of channels  (=3 for Sci, =2 for TofW)
    Int_t fNumSignals;              // =fNumDetectors * fNumChannels
    Int_t fNumTcalParsPerSignal;
    const R3BSofTcalPar& operator=(const R3BSofTcalPar&); /*< an assignment operator>*/

    R3BSofTcalPar(const R3BSofTcalPar&); // a copy constructor

    ClassDef(R3BSofTcalPar, 1);
};

#endif //__R3BSOFTCALPAR_H__
