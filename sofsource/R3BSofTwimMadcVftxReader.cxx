#include "FairLogger.h"

#include "FairRootManager.h"
#include "R3BSofTwimMadcVftxReader.h"
#include "R3BSofMusicMadcVftxMappedData.h"

extern "C" {
#include "ext_data_client.h"
#include "ext_h101_softwim_madcvftx.h"
}

#define NUM_SOFTWIM_MADCVFTX_PLANES   2
#define NUM_SOFTWIM_MADCVFTX_SECTIONS 2
#define NUM_SOFTWIM_MADCVFTX_ANODES   16
#include <iostream>

using namespace std;

R3BSofTwimMadcVftxReader::R3BSofTwimMadcVftxReader(EXT_STR_h101_SOFTWIM_MADCVFTX* data, UInt_t offset)
  : R3BReader("R3BSofTwimMadcVftxReader")
  , fData(data)
  , fOffset(offset)
  , fLogger(FairLogger::GetLogger())
  , fArray(new TClonesArray("R3BSofMusicMadcVftxMappedData"))
{
}

R3BSofTwimMadcVftxReader::~R3BSofTwimMadcVftxReader()
{}

Bool_t R3BSofTwimMadcVftxReader::Init(ext_data_struct_info *a_struct_info)
{
  int ok;
  EXT_STR_h101_SOFTWIM_MADCVFTX_ITEMS_INFO(ok, *a_struct_info, fOffset,EXT_STR_h101_SOFTWIM_MADCVFTX, 0);
  if (!ok) {
    perror("ext_data_struct_info_item");
    fLogger->Error(MESSAGE_ORIGIN,"Failed to setup structure information.");
    return kFALSE;
  }

  // Register output array in tree
  FairRootManager::Instance()->Register("SofTwimMadcVftx","MappedDim", fArray, kTRUE);

  // clear struct_writer's output struct. Seems ucesb doesn't do that
  // for channels that are unknown to the current ucesb config.
  EXT_STR_h101_SOFTWIM_MADCVFTX_onion* data = (EXT_STR_h101_SOFTWIM_MADCVFTX_onion*)fData;
  for (int p=0;p<NUM_SOFTWIM_MADCVFTX_PLANES;p++)
    for(int s=0;s<NUM_SOFTWIM_MADCVFTX_SECTIONS;s++)
      data->SOFTWIM_P[p].S[s].TFM=0;
    
  return kTRUE;
}

Bool_t R3BSofTwimMadcVftxReader::Read()
{
  // Convert plain raw data to multi-dimensional array
  EXT_STR_h101_SOFTWIM_MADCVFTX_onion* data = (EXT_STR_h101_SOFTWIM_MADCVFTX_onion*)fData;

  /*
    REMINDER OF THE DATA STRUCTURE

    typedef struct EXT_STR_h101_SOFTWIM_MADCVFTX_onion_t
    {
    struct {
    struct {
    uint32_t TFM;
    uint32_t TFMI[16 // TFM //];
    uint32_t TFME[16 // TFM //];
    uint32_t TF;
    uint32_t TFv[160 // TF //];
    uint32_t TCM;
    uint32_t TCMI[16 // TCM //];
    uint32_t TCME[16 // TCM //];
    uint32_t TC;
    uint32_t TCv[160 // TC //];
    uint32_t E[16];
    } S[2];
    } SOFTWIM_P[2];
    } EXT_STR_h101_SOFTWIM_MADCVFTX_onion;
  */
  
  
  // loop over all planes and sections
  for (int p=0;p<NUM_SOFTWIM_MADCVFTX_PLANES;p++){
    for(int s=0;s<NUM_SOFTWIM_MADCVFTX_SECTIONS;s++){
      uint32_t NumberOfAnodesWithHits = data->SOFTWIM_P[p].S[s].TFM; // could also be data->SOFTWIM_P[d].S[s].TCM;
      uint32_t TotalNumberOfHits = data->SOFTWIM_P[p].S[s].TF;       // could also be data->SOFTWIM_MADCVFTX_P[d].TC
      Bool_t   FLAG_S;
      
      FLAG_S = kFALSE;
      if((NumberOfAnodesWithHits==TotalNumberOfHits) && (TotalNumberOfHits==6)) FLAG_S=kTRUE;  // no double hits
      
      // loop over channels with hits
      uint32_t curChannelStart=0;
      for (int amult=0;amult<NumberOfAnodesWithHits;amult++) {
	uint32_t aval=data->SOFTWIM_P[p].S[s].TFMI[amult];
	uint32_t nextChannelStart=data->SOFTWIM_P[p].S[s].TFME[amult];
	// put the mapped items {det,pmt,finetime, coarsetime} one after the other in the fArray
	for (int hit=curChannelStart;hit<nextChannelStart;hit++){
	  new ((*fArray)[fArray->GetEntriesFast()])R3BSofMusicMadcVftxMappedData(p+1,s+1,aval,data->SOFTWIM_P[p].S[s].TCv[hit],data->SOFTWIM_P[p].S[s].TFv[hit],data->SOFTWIM_P[p].S[s].E[amult],FLAG_S);
	}
	curChannelStart=nextChannelStart;
      }
    }// end of loop over the sections
  }// end of loop over the planes
  return kTRUE;
}

void R3BSofTwimMadcVftxReader::Reset()
{
    // Reset the output array
    fArray->Clear();
}

ClassImp(R3BSofTwimMadcVftxReader)
