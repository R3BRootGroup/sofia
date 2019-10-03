#include "FairLogger.h"

#include "FairRootManager.h"
#include "R3BSofTwimReader.h"
#include "R3BSofTwimMappedData.h"

extern "C" {
#include "ext_data_client.h"
#include "ext_h101_softwim.h"
}

#include <iostream>

using namespace std;

R3BSofTwimReader::R3BSofTwimReader(EXT_STR_h101_SOFTWIM* data, UInt_t offset)
  : R3BReader("R3BSofTwimReader")
  , fData(data)
  , fOffset(offset)
  , fLogger(FairLogger::GetLogger())
  , fArray(new TClonesArray("R3BSofTwimMappedData"))
{
}

R3BSofTwimReader::~R3BSofTwimReader()
{}

Bool_t R3BSofTwimReader::Init(ext_data_struct_info *a_struct_info)
{
  int ok;
  LOG(INFO) << "R3BSofTwimReader::Init";
  EXT_STR_h101_SOFTWIM_ITEMS_INFO(ok, *a_struct_info, fOffset,EXT_STR_h101_SOFTWIM, 0);
  if (!ok) {
    perror("ext_data_struct_info_item");
    LOG(ERROR) << "R3BSofTwimReader::Failed to setup structure information." ;
    return kFALSE;
  }

  // Register output array in tree
  FairRootManager::Instance()->Register("SofTwimMappedData","SofTwim", fArray, kTRUE);

  // clear struct_writer's output struct. Seems ucesb doesn't do that
  // for channels that are unknown to the current ucesb config.
  EXT_STR_h101_SOFTWIM_onion* data = (EXT_STR_h101_SOFTWIM_onion*)fData;
  for(int s=0;s<NUM_SOFTWIM_SECTIONS;s++){
    data->SOFTWIM_S[s].EM=0;
    data->SOFTWIM_S[s].TM=0;
    data->SOFTWIM_S[s].TREFM=0;
  }
  for(int mult=0; mult<NUM_SOFTWIM_ANODES; mult++) multPerAnode[mult] = 0;

  return kTRUE;
}

Bool_t R3BSofTwimReader::Read()
{
  // Convert plain raw data to multi-dimensional array
  EXT_STR_h101_SOFTWIM_onion* data = (EXT_STR_h101_SOFTWIM_onion*)fData;

  // loop over all planes and sections
  for(UShort_t s=0;s<NUM_SOFTWIM_SECTIONS;s++){
    ReadData(data,s);
  }
  return kTRUE;
}

void R3BSofTwimReader::Reset()
{
    // Reset the output array
    fArray->Clear();
}

Bool_t R3BSofTwimReader::ReadData(EXT_STR_h101_SOFTWIM_onion* data, UShort_t section)
{

  // --- ----------------- --- //
  // --- TWIM MAPPED DATA --- //
  // --- ----------------- --- //

  // --- TREF --- //
  // return the number of Tref in the section with data
  // 0<=nTref<=1
  UShort_t nTref = data->SOFTWIM_S[section].TREFM;
  if (nTref>1)
    LOG(ERROR) << "R3BSofTwimReader::ReadData ERROR ! ONLY ONE Tref signal PER SECTION BUT HERE MULTIPLE SIGNALS IS FOUND!" ;
  uint32_t curTref=0;
  uint32_t nextTref; 
  if (nTref==1){
    nextTref = data->SOFTWIM_S[section].TREFME[0];
    multPerAnode[16] = nextTref-curTref;
    if(multPerAnode[16]!=data->SOFTWIM_S[section].TREF)
      LOG(ERROR) << "R3BSofTwimReader::ReadData ERROR ! multiplicity of Tref not consistent!" ;
    for(int hit=curTref; hit<nextTref;hit++)
      R3BSofTwimMappedData * mapped = new ((*fArray)[fArray->GetEntriesFast()])R3BSofTwimMappedData(section+1,17,data->SOFTWIM_S[section].TREFv[hit],0);
  }


  // --- ANODES --- //
  // return the number of anodes in the section with data
  // 0<=nAnodesEnergy<=16
  // 0<=nAnodesTime<=16
  UShort_t nAnodesEnergy = data->SOFTWIM_S[section].EM;
  UShort_t nAnodesTime = data->SOFTWIM_S[section].TM;

  // mail from R. Schneider from June 16, 2016: "if you have several hits of one channel in the window of interest, you get an energy and a timing for each hit."
  // --> multiplicity per anode (for anode 1 to anode 16) should also be the same in energy and time
  // mail from R. Schneider from May 21st 2019 : "the hits from one channel are kept in the chronological order."
  // --> for one anode with multi-hit, the first hit in energy correspond to the first hit in time 
  if (nAnodesEnergy!=nAnodesTime)
    LOG(ERROR) << "R3BSofTwimReader::ReadData ERROR ! NOT THE SAME NUMBER OF ANODES HITTED IN ENERGY AND TIME" ;

  // ENERGY AND TIME ARE SORTED 
  uint32_t curAnodeTimeStart=0;
  uint32_t curAnodeEnergyStart=0;
  for(UShort_t a=0; a<nAnodesTime; a++){
    // EMI and TMI give the 1-based anode number
    UShort_t idAnodeTime = data->SOFTWIM_S[section].TMI[a];
    UShort_t idAnodeEnergy = data->SOFTWIM_S[section].EMI[a];
    if(idAnodeEnergy!=idAnodeTime)
      LOG(ERROR) << "R3BSofTwimReader::ReadData ERROR ! MISMATCH FOR ANODE ID IN ENERGY #" << idAnodeEnergy<< " AND TIME #" << idAnodeTime ;
    uint32_t nextAnodeTimeStart = data->SOFTWIM_S[section].TME[a];
    uint32_t nextAnodeEnergyStart = data->SOFTWIM_S[section].EME[a];
    multPerAnode[idAnodeTime-1] = nextAnodeTimeStart-curAnodeTimeStart;
    if(multPerAnode[idAnodeTime-1]!=(nextAnodeEnergyStart-curAnodeEnergyStart))
      LOG(ERROR) << "R3BSofTwimReader::ReadData ERROR ! MISMATCH FOR MULTIPLICITY PER ANODE IN ENERGY AND TIME";
    for(int hit=curAnodeTimeStart; hit<nextAnodeTimeStart;hit++){
      R3BSofTwimMappedData * mapped = new ((*fArray)[fArray->GetEntriesFast()])R3BSofTwimMappedData(section+1,idAnodeEnergy,data->SOFTWIM_S[section].Tv[hit],data->SOFTWIM_S[section].Ev[hit]);
    }
    curAnodeEnergyStart=nextAnodeEnergyStart;
    curAnodeTimeStart=nextAnodeTimeStart;
  }//end of loop over the anodes from 1 to 16
  
  return kTRUE;
}

ClassImp(R3BSofTwimReader)
