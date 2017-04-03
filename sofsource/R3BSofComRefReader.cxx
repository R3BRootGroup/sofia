#include "FairLogger.h"

#include "TClonesArray.h"
#include "FairRootManager.h"
#include "R3BSofComRefReader.h"
#include "R3BSofComRefMappedData.h"

extern "C" {
#include "ext_data_client.h"
#include "ext_h101_sofcomref.h"
}

#define NUM_SOFCOMREF_SIGNALS 2
#include <iostream>

using namespace std;

R3BSofComRefReader::R3BSofComRefReader(EXT_STR_h101_SOFCOMREF* data, UInt_t offset)
  : R3BReader("R3BSofComRefReader")
  , fData(data)
  , fOffset(offset)
  , fLogger(FairLogger::GetLogger())
  , fArray(new TClonesArray("R3BSofComRefMappedData"))
{
}

R3BSofComRefReader::~R3BSofComRefReader()
{}

Bool_t R3BSofComRefReader::Init(ext_data_struct_info *a_struct_info)
{
  int ok;
  EXT_STR_h101_SOFCOMREF_ITEMS_INFO(ok, *a_struct_info, fOffset,EXT_STR_h101_SOFCOMREF, 0);
  if (!ok) {
    perror("ext_data_struct_info_item");
    fLogger->Error(MESSAGE_ORIGIN,"Failed to setup structure information.");
    return kFALSE;
  }

  // Register output array in tree
  FairRootManager::Instance()->Register("SofComRef","MappedDim", fArray, kTRUE);
  
  // clear struct_writer's output struct. Seems ucesb doesn't do that
  // for channels that are unknown to the current ucesb config.
  EXT_STR_h101_SOFCOMREF_onion* data = (EXT_STR_h101_SOFCOMREF_onion*)fData;
  for (int d=0;d<NUM_SOFCOMREF_SIGNALS;d++)
    data->SOFCOMREF[d].TFM=0;
  
  return kTRUE;
}

Bool_t R3BSofComRefReader::Read()
{
  // Convert plain raw data to multi-dimensional array
  EXT_STR_h101_SOFCOMREF_onion* data = (EXT_STR_h101_SOFCOMREF_onion*)fData;

  // loop over all signals
  for (int d=0;d<NUM_SOFCOMREF_SIGNALS;d++){		
    // no need to loop over channels with hits : one channel per signal
    // put the mapped items {signal,finetime, coarsetime} one after the other in the fArray
    for (int hit=0;hit<data->SOFCOMREF[d].TFME[0];hit++)
      new ((*fArray)[fArray->GetEntriesFast()])R3BSofComRefMappedData(d+1,data->SOFCOMREF[d].TCv[hit],data->SOFCOMREF[d].TFv[hit]);
  }
  return kTRUE;
}

void R3BSofComRefReader::Reset()
{
    // Reset the output array
    fArray->Clear();
}

ClassImp(R3BSofComRefReader)

