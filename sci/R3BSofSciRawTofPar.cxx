#include "R3BSofSciRawTofPar.h"

#include "FairLogger.h"
#include "FairParamList.h"

#include "TArrayF.h"
#include "TMath.h"
#include "TString.h"

#include <iostream>

#define MAX_RAWTOFPAR 2*NUMBER_OF_SOFSCI_TOF 

using std::cout;
using std::endl;

// ---- Standard Constructor ---------------------------------------------------
R3BSofSciRawTofPar::R3BSofSciRawTofPar(const char* name, const char* title, const char* context)
  : FairParGenericSet(name, title, context)
  , fFirstStaSci(1)
  , fFirstStoSci(NUMBER_OF_SOFSCI_DETECTORS) //last SofSci at cave C
  , fNumSignals(0)
  , fNumParsPerSignal(0)
{
  fAllSignalsRawTofParams = new TArrayF(MAX_RAWTOFPAR);
}

// ----  Destructor ------------------------------------------------------------
R3BSofSciRawTofPar::~R3BSofSciRawTofPar()
{
    clear();
    if (fAllSignalsRawTofParams)
    {
        delete fAllSignalsRawTofParams;
        fAllSignalsRawTofParams == NULL;
    }
}

// ----  Method clear ----------------------------------------------------------
void R3BSofSciRawTofPar::clear()
{
    status = kFALSE;
    resetInputVersions();
}

// ----  Method putParams ------------------------------------------------------
void R3BSofSciRawTofPar::putParams(FairParamList* list)
{
    LOG(INFO) << "R3BSofSciRawTofPar::putParams() called";
    if (!list)
    {
        return;
    }

    Int_t array_size = fNumSignals * fNumParsPerSignal;
    LOG(INFO) << "Array Size: " << array_size;

    fAllSignalsRawTofParams->Set(array_size);

    list->add("RawTofPar", *fAllSignalsRawTofParams);
    list->add("selectionFirstStart",fFirstStaSci);
    list->add("selectionFirstStop",fFirstStoSci);
    list->add("nSignalsRawTofPar", fNumSignals);
    list->add("nRawTofParsPerSignal", fNumParsPerSignal);
}

// ----  Method getParams ------------------------------------------------------
Bool_t R3BSofSciRawTofPar::getParams(FairParamList* list)
{
    LOG(INFO) << "R3BSofSciRawTofPar::getParams() called";
    if (!list)
    {
        return kFALSE;
    }
    if (!list->fill("selectionFirstStart"),&fFirstStaSci)
    {
      return kFALSE;
    }
    if (!list->fill("selectionFirstStop"),&fFirstStoSci)
    {
      return kFALSE;
    }
    if (!list->fill("nSignalsRawTofPar", &fNumSignals))
    {
        return kFALSE;
    }
    if (!list->fill("nRawTofParsPerSignal", &fNumParsPerSignal))
    {
        return kFALSE;
    }

    Int_t array_size = fNumSignals * fNumParsPerSignal;
    LOG(INFO) << "Array Size: " << array_size;
    fAllSignalsRawTofParams->Set(array_size);

    if (!(list->fill("RawTofPar", fAllSignalsRawTofParams)))
    {
        LOG(INFO) << "---Could not initialize fAllSignalsRawTofParams";
        return kFALSE;
    }

    return kTRUE;
}

// ----  Method printParams ----------------------------------------------------
void R3BSofSciRawTofPar::printParams()
{
    LOG(INFO) << "R3BSofSciRawTofPar: SofSciRawTof Parameters: ";
    Int_t array_size = fNumSignals * fNumParsPerSignal;

    cout << "--- --------------------------------------------" << endl;
    cout << "--- Single Tcal RawTof Parameters :  " << endl;
    cout << "--- --------------------------------------------" << endl;
    for (Int_t param=0; param < array_size; param++)
      {
	cout << "LIMIT " << param << " = " << fAllSignalsRawTofParams->GetAt(param) << endl;
      }
}
