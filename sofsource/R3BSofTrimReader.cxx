#include "FairLogger.h"

#include "FairRootManager.h"
#include "R3BSofTrimReader.h"
#include "R3BSofMusicMappedData.h"

extern "C" {
#include "ext_data_client.h"
#include "ext_h101_softrim.h"
}

#include <iostream>

using namespace std;

R3BSofTrimReader::R3BSofTrimReader(EXT_STR_h101_SOFTRIM* data, UInt_t offset)
  : R3BReader("R3BSofTrimReader")
  , fData(data)
  , fOffset(offset)
  , fLogger(FairLogger::GetLogger())
  , fArray(new TClonesArray("R3BSofMusicMappedData"))
{
}

R3BSofTrimReader::~R3BSofTrimReader()
{}

Bool_t R3BSofTrimReader::Init(ext_data_struct_info *a_struct_info)
{
  int ok;
  EXT_STR_h101_SOFTRIM_ITEMS_INFO(ok, *a_struct_info, fOffset,EXT_STR_h101_SOFTRIM, 0);
  if (!ok) {
    perror("ext_data_struct_info_item");
    LOG(ERROR) << "Failed to setup structure information.";
    return kFALSE;
  }

  // Register output array in tree
  FairRootManager::Instance()->Register("SofTrimMappedData","SofTrim", fArray, kTRUE);

  // clear struct_writer's output struct. Seems ucesb doesn't do that
  // for channels that are unknown to the current ucesb config.
  EXT_STR_h101_SOFTRIM_onion* data = (EXT_STR_h101_SOFTRIM_onion*)fData;
  for(int s=0;s<NUM_SOFTRIM_SECTIONS;s++){
    data->SOFTRIM_S[s].EM=0;
    data->SOFTRIM_S[s].TM=0;
    data->SOFTRIM_S[s].TREFM=0;
  }
  for(int mult=0; mult<NUM_SOFTRIM_ANODES; mult++) multPerAnode[mult] = 0;

  return kTRUE;
}

Bool_t R3BSofTrimReader::Read()
{
  // Convert plain raw data to multi-dimensional array
  EXT_STR_h101_SOFTRIM_onion* data = (EXT_STR_h101_SOFTRIM_onion*)fData;

  // loop over all planes and sections
  for(UShort_t s=0;s<NUM_SOFTRIM_SECTIONS;s++){
    ReadData(data,s);
  }
  return kTRUE;
}

void R3BSofTrimReader::Reset()
{
    // Reset the output array
    fArray->Clear();
}

Bool_t R3BSofTrimReader::ReadData(EXT_STR_h101_SOFTRIM_onion* data, UShort_t section)
{

  // --- ----------------- --- //
  // --- TRIM MAPPED DATA --- //
  // --- ----------------- --- //

  // --- TREF --- //
  // return the number of Tref in the section with data
  // 0<=nTref<=1
  UShort_t nTref = data->SOFTRIM_S[section].TREFM;
  if (nTref>1)
    LOG(ERROR) << "R3BSofTrimReader::ReadData ERROR ! ONLY ONE Tref signal PER SECTION BUT HERE MULTIPLE SIGNALS IS FOUND!";
  uint32_t curTref=0;
  uint32_t nextTref; 
  if (nTref==1){
    nextTref = data->SOFTRIM_S[section].TREFME[0];
    multPerAnode[6] = nextTref-curTref;
    if(multPerAnode[6]!=data->SOFTRIM_S[section].TREF)
      LOG(ERROR) << "R3BSofTrimReader::ReadData ERROR ! multiplicity of Tref not consistent!";
    for(int hit=curTref; hit<nextTref;hit++)
      R3BSofMusicMappedData * mapped = new ((*fArray)[fArray->GetEntriesFast()])R3BSofMusicMappedData(section+1,7,data->SOFTRIM_S[section].TREFv[hit],0);
  }


  // --- ANODES --- //
  // return the number of anodes in the section with data
  // 0<=nAnodesEnergy<=6
  // 0<=nAnodesTime<=6
  UShort_t nAnodesEnergy = data->SOFTRIM_S[section].EM;
  UShort_t nAnodesTime = data->SOFTRIM_S[section].TM;

  // mail from R. Schneider from June 16, 2016: "if you have several hits of one channel in the window of interest, you get an energy and a timing for each hit."
  // --> multiplicity per anode (for anode 1 to anode 16) should also be the same in energy and time
  // mail from R. Schneider from May 21st 2019 : "the hits from one channel are kept in the chronological order."
  // --> for one anode with multi-hit, the first hit in energy correspond to the first hit in time 
  if (nAnodesEnergy!=nAnodesTime)
    LOG(ERROR) << "R3BSofTrimReader::ReadData ERROR ! NOT THE SAME NUMBER OF ANODES HITTED IN ENERGY AND TIME" ;

  // ENERGY AND TIME ARE SORTED 
  uint32_t curAnodeTimeStart=0;
  uint32_t curAnodeEnergyStart=0;
  for(UShort_t a=0; a<nAnodesTime; a++){
    // EMI and TMI give the 1-based anode number
    UShort_t idAnodeTime = data->SOFTRIM_S[section].TMI[a];
    UShort_t idAnodeEnergy = data->SOFTRIM_S[section].EMI[a];
    if(idAnodeEnergy!=idAnodeTime)
      LOG(ERROR) << "R3BSofTrimReader::ReadData ERROR ! MISMATCH FOR ANODE ID IN ENERGY #" << idAnodeEnergy<< " AND TIME #" << idAnodeTime ;
    uint32_t nextAnodeTimeStart = data->SOFTRIM_S[section].TME[a];
    uint32_t nextAnodeEnergyStart = data->SOFTRIM_S[section].EME[a];
    multPerAnode[idAnodeTime-1] = nextAnodeTimeStart-curAnodeTimeStart;
    if(multPerAnode[idAnodeTime-1]!=(nextAnodeEnergyStart-curAnodeEnergyStart))
      LOG(ERROR) << "R3BSofTrimReader::ReadData ERROR ! MISMATCH FOR MULTIPLICITY PER ANODE IN ENERGY AND TIME";
    for(int hit=curAnodeTimeStart; hit<nextAnodeTimeStart;hit++){
      R3BSofMusicMappedData * mapped = new ((*fArray)[fArray->GetEntriesFast()])R3BSofMusicMappedData(section+1,idAnodeEnergy,data->SOFTRIM_S[section].Tv[hit],data->SOFTRIM_S[section].Ev[hit]);
    }
    curAnodeEnergyStart=nextAnodeEnergyStart;
    curAnodeTimeStart=nextAnodeTimeStart;
  }//end of loop over the multiplicity on anodes (1 to 6)
  
  return kTRUE;
}

ClassImp(R3BSofTrimReader)
