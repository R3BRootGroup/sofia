#include "FairLogger.h"

#include "FairRootManager.h"
#include "R3BSofMwpcMappedData.h"
#include "R3BSofMwpcReader.h"

extern "C" {
#include "ext_data_client.h"
#include "ext_h101_sofmwpc.h"
}

#define NUM_SOFMWPC_DETECTORS   4
#define NUM_SOFMWPC_PLANES_MAX  3
#include <iostream>

using namespace std;

R3BSofMwpcReader::R3BSofMwpcReader(EXT_STR_h101_SOFMWPC* data, UInt_t offset)
    : R3BReader("R3BSofMwpcReader")
    , fData(data)
    , fOffset(offset)
    , fLogger(FairLogger::GetLogger())
    , fArrayMwpc0(new TClonesArray("R3BSofMwpcMappedData")) // class name
    , fArrayMwpc1(new TClonesArray("R3BSofMwpcMappedData")) // class name
    , fArrayMwpc2(new TClonesArray("R3BSofMwpcMappedData")) // class name
    , fArrayMwpc3(new TClonesArray("R3BSofMwpcMappedData")) // class name
    , fNumEntries(0)
{
}

R3BSofMwpcReader::~R3BSofMwpcReader() {}

Bool_t R3BSofMwpcReader::Init(ext_data_struct_info* a_struct_info)
{
    int ok;
    LOG(INFO) << "R3BSofMwpcReader::Init";
    EXT_STR_h101_SOFMWPC_ITEMS_INFO(ok, *a_struct_info, fOffset, EXT_STR_h101_SOFMWPC, 0);
    if (!ok)
    {
        perror("ext_data_struct_info_item");
        LOG(ERROR) << "R3BSofMwpcReader::Failed to setup structure information.";
        return kFALSE;
    }

    // Register output array in tree
    FairRootManager::Instance()->Register("Mwpc0MappedData", "MWPC0", fArrayMwpc0, kTRUE);
    fArrayMwpc0->Clear();
    FairRootManager::Instance()->Register("Mwpc1MappedData", "MWPC0", fArrayMwpc1, kTRUE);
    fArrayMwpc1->Clear();
    FairRootManager::Instance()->Register("Mwpc2MappedData", "MWPC0", fArrayMwpc2, kTRUE);
    fArrayMwpc2->Clear();
    FairRootManager::Instance()->Register("Mwpc3MappedData", "MWPC0", fArrayMwpc3, kTRUE);
    fArrayMwpc3->Clear();

    // clear struct_writer's output struct. Seems ucesb doesn't do that
    // for channels that are unknown to the current ucesb config.
    // no multi-hit capabiity
    EXT_STR_h101_SOFMWPC_onion* data = (EXT_STR_h101_SOFMWPC_onion*)fData;
    for (int d = 0; d < NUM_SOFMWPC_DETECTORS; d++)
      for(int p=0; p < NUM_SOFMWPC_PLANES_MAX; p++)
	data->SOFMWPC[d].Plane[p].Q = 0;
    return kTRUE;
}

Bool_t R3BSofMwpcReader::Read()
{
    // Convert plain raw data to multi-dimensional array
    EXT_STR_h101_SOFMWPC_onion* data = (EXT_STR_h101_SOFMWPC_onion*)fData;

    // loop over all detectors
    for (int d = 0; d < NUM_SOFMWPC_DETECTORS; d++){
      for (int p = 0; p < NUM_SOFMWPC_PLANES_MAX; p++){
	  uint32_t numberOfPadsPerPlane = data->SOFMWPC[d].Plane[p].Q;
	  for (int mult = 0; mult < numberOfPadsPerPlane; mult++){
	    uint16_t pad  = data->SOFMWPC[d].Plane[p].QI[mult];
	    uint16_t qval = data->SOFMWPC[d].Plane[p].Qv[mult];
	    R3BSofMwpcMappedData* mapped;
	    switch(d){
	    case 1: 
	      mapped = new ((*fArrayMwpc0)[fArrayMwpc0->GetEntriesFast()])
                R3BSofMwpcMappedData(p+1,pad,qval);
	      break;
	    case 2: 
	      mapped = new ((*fArrayMwpc1)[fArrayMwpc1->GetEntriesFast()])
                R3BSofMwpcMappedData(p+1,pad,qval);
	      break;
	    case 3: 
	      mapped = new ((*fArrayMwpc2)[fArrayMwpc2->GetEntriesFast()])
                R3BSofMwpcMappedData(p+1,pad,qval);
	      break;
	    case 4: 
	      mapped = new ((*fArrayMwpc3)[fArrayMwpc3->GetEntriesFast()])
                R3BSofMwpcMappedData(p+1,pad,qval);
	      break;
	    default:
	      break;
	    }
	  }
      }
    }
    return kTRUE;
}

void R3BSofMwpcReader::Reset()
{
    // Reset the output array
    fArrayMwpc0->Clear();
    fArrayMwpc1->Clear();
    fArrayMwpc2->Clear();
    fArrayMwpc3->Clear();
    fNumEntries = 0;
}

ClassImp(R3BSofMwpcReader)
