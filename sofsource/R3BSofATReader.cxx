#include "FairLogger.h"

#include "TClonesArray.h"
#include "FairRootManager.h"
#include "R3BSofATReader.h"
#include "R3BSofATMappedData.h"

extern "C" {
#include "ext_data_client.h"
#include "ext_h101_sofat.h"
}

#define NUM_SOFAT_CHANNELS  5
#include <iostream>


using namespace std;

R3BSofATReader::R3BSofATReader(EXT_STR_h101_SOFAT* data, UInt_t offset)
  : R3BReader("R3BSofATReader")
  , fData(data)
  , fOffset(offset)
  , fLogger(FairLogger::GetLogger())
  , fArray(new TClonesArray("R3BSofATMappedData"))
{
}

R3BSofATReader::~R3BSofATReader()
{}

Bool_t R3BSofATReader::Init(ext_data_struct_info *a_struct_info)
{
  int ok;
  
  EXT_STR_h101_SOFAT_ITEMS_INFO(ok, *a_struct_info, fOffset,EXT_STR_h101_SOFAT, 0);
  
  if (!ok) {
    perror("ext_data_struct_info_item");
    fLogger->Error(MESSAGE_ORIGIN,"Failed to setup structure information.");
    return kFALSE;
  }
  
  // Register output array in tree
  FairRootManager::Instance()->Register("SofAt","MappedDim", fArray, kTRUE);
  
  EXT_STR_h101_SOFAT_onion* data = (EXT_STR_h101_SOFAT_onion*)fData;
  for (int ch=0;ch<NUM_SOFAT_CHANNELS;ch++)
    data->SOFAT_E[ch]=0;
  
  return kTRUE;
}

Bool_t R3BSofATReader::Read()
{
  EXT_STR_h101_SOFAT_onion* data = (EXT_STR_h101_SOFAT_onion*)fData;
  
  R3BSofATMappedData * mapped = new ((*fArray)[fArray->GetEntriesFast()])R3BSofATMappedData(NUM_SOFAT_CHANNELS);
  

  // loop over all the anodes
  for (int ch=0; ch<NUM_SOFAT_CHANNELS; ch++)
    mapped->SetEnergy(ch,data->SOFAT_E[ch]);

  return kTRUE;
}

void R3BSofATReader::Reset()
{
    // Reset the output array
    fArray->Clear();
}

ClassImp(R3BSofATReader)

