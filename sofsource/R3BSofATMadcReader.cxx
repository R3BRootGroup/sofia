#include "FairLogger.h"

#include "TClonesArray.h"
#include "FairRootManager.h"
#include "R3BSofATMadcReader.h"
#include "R3BSofATMadcMappedData.h"

extern "C" {
#include "ext_data_client.h"
#include "ext_h101_sofat_madc.h"
}

#define NUM_SOFAT_CHANNELS  5
#include <iostream>


using namespace std;

R3BSofATMadcReader::R3BSofATMadcReader(EXT_STR_h101_SOFAT_MADC* data, UInt_t offset)
  : R3BReader("R3BSofATMadcReader")
  , fData(data)
  , fOffset(offset)
  , fLogger(FairLogger::GetLogger())
  , fArray(new TClonesArray("R3BSofATMadcMappedData"))
{
}

R3BSofATMadcReader::~R3BSofATMadcReader()
{}

Bool_t R3BSofATMadcReader::Init(ext_data_struct_info *a_struct_info)
{
  int ok;
  
  EXT_STR_h101_SOFAT_MADC_ITEMS_INFO(ok, *a_struct_info, fOffset,EXT_STR_h101_SOFAT_MADC, 0);
  
  if (!ok) {
    perror("ext_data_struct_info_item");
    fLogger->Error(MESSAGE_ORIGIN,"Failed to setup structure information.");
    return kFALSE;
  }
  
  // Register output array in tree
  FairRootManager::Instance()->Register("SofAt","MappedDim", fArray, kTRUE);
  
  EXT_STR_h101_SOFAT_MADC_onion* data = (EXT_STR_h101_SOFAT_MADC_onion*)fData;
  for (int ch=0;ch<NUM_SOFAT_CHANNELS;ch++)
    data->SOFAT_E[ch]=0;
  
  return kTRUE;
}

Bool_t R3BSofATMadcReader::Read()
{
  EXT_STR_h101_SOFAT_MADC_onion* data = (EXT_STR_h101_SOFAT_MADC_onion*)fData;
  
  R3BSofATMadcMappedData * mapped = new ((*fArray)[fArray->GetEntriesFast()])R3BSofATMadcMappedData(NUM_SOFAT_CHANNELS);
  

  // loop over all the anodes
  for (int ch=0; ch<NUM_SOFAT_CHANNELS; ch++)
    mapped->SetEnergy(ch,data->SOFAT_E[ch]);

  return kTRUE;
}

void R3BSofATMadcReader::Reset()
{
    // Reset the output array
    fArray->Clear();
}

ClassImp(R3BSofATMadcReader)

