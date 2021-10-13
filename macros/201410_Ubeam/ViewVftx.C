#include <TCanvas.h>
#include <TH1.h>
#include <string.h>


#define SOF_COMREF 1
#define SOF_SCI    1
#define SOF_TOFW   0
#define SOF_TRIM   0
#define SOF_TWIM   0

#if SOF_COMREF
// see sofsource/R3BSofComRefReader.cxx
#define NUM_SOFCOMREF_SIGNALS 2 
#endif

#if SOF_SCI
// see sofsource/R3BSofSciReader.cxx
#define NUM_SOFSCI_DETECTORS 2
#define NUM_SOFSCI_CHANNELS  2
#endif

#if SOF_TOFW
// see sofsource/R3BSofToFWReader.cxx
#define NUM_SOFTOFW_DETECTORS 28
#define NUM_SOFTOFW_CHANNELS  2
#endif

#if SOF_TRIM
// see sofsource/R3BSofTrimMadcVftxReader.cxx
#define NUM_SOFTRIM_PLANES   1
#define NUM_SOFTRIM_SECTIONS 3
#define NUM_SOFTRIM_ANODES   6
#endif

#if SOF_TWIM
// see sofsource/R3BSofTwimMadcVftxReader.cxx
#define NUM_SOFTWIM_PLANES   2
#define NUM_SOFTWIM_SECTIONS 2
#define NUM_SOFTWIM_ANODES   16
#endif


void ViewVftx(){


  // --- ---------------- --- //
  // --- ROOT ENVIRONMENT --- //
  // --- ---------------- --- //
  gROOT->SetStyle("Plain");
  gStyle->SetOptStat(1);
  gStyle->SetOptFit(0);

  // --- ---------- --- //
  // --- INPUT DATA --- //
  // --- ---------- --- //
  char inputFile[250] = "../SofMacrosOutput/201410_Ubeam/SofDataVftx_Bi194.root";
  TFile * file = TFile::Open(inputFile);
  TTree * t = (TTree*)file->Get("evt");
  t->ls();
  t->Print();

  // --- ----------- --- //
  // --- MAPPED DATA --- //
  // --- ----------- --- //
#if SOF_COMREF
  // COMMON REFERENCE SIGNALS AT S2 AND CAVE C
  TClonesArray * mappedSofComRef = new TClonesArray("R3BSofComRefMappedData",5);
  TBranch * b_mappedSofComRef = t->GetBranch("SofComRef");
  b_mappedSofComRef->SetAddress(&mappedSofComRef);
  b_mappedSofComRef->ls();
  Int_t nHits_mappedSofComRef=0; // number of hits per event
  R3BSofComRefMappedData** evt_mappedSofComRef; // array with [nHits] dimension
#endif
#if SOF_SCI
  // SCINTILLATORS AT S2 AND CAVE C
  TClonesArray * mappedSofSci = new TClonesArray("R3BSofSciMappedData",5);
  TBranch * b_mappedSofSci = t->GetBranch("SofSci");
  b_mappedSofSci->SetAddress(&mappedSofSci);
  b_mappedSofSci->ls();
  Int_t nHits_mappedSofSci=0;
  R3BSofSciMappedData** evt_mappedSofSci;
#endif
#if SOF_TOFW
  // TIME-OF-FLIGHT WALL
  TClonesArray * mappedSofToFW = new TClonesArray("R3BSofToFWMappedData",5);
  TBranch * b_mappedSofToFW = t->GetBranch("SofToFW");
  b_mappedSofToFW->SetAddress(&mappedSofToFW);
  b_mappedSofToFW->ls();
  Int_t nHits_mappedSofToFW=0;
  R3BSofToFWMappedData** evt_mappedSofToFW;
#endif
#if SOF_TRIM
  // TRIPLE MUSIC WHEN READ OUT BY VFTX+MADC
  TClonesArray * mappedSofTrim = new TClonesArray("R3BSofMusicMadcVftxMappedData",5);
  TBranch * b_mappedSofTrim = t->GetBranch("SofTrimMadcVftx");
  b_mappedSofTrim->SetAddress(&mappedSofTrim);
  b_mappedSofTrim->ls();
  Int_t nHits_mappedSofTrim=0;
  R3BSofMusicMadcVftxMappedData** evt_mappedSofTrim;
#endif
#if SOF_TWIM
  // TWIIN MUSIC WHEN READ OUT BY VFTX+MADC
  TClonesArray * mappedSofTwim = new TClonesArray("R3BSofMusicMadcVftxMappedData",5);
  TBranch * b_mappedSofTwim = t->GetBranch("SofTwimMadcVftx");
  b_mappedSofTwim->SetAddress(&mappedSofTwim);
  b_mappedSofTwim->ls();
  Int_t nHits_mappedSofTwim=0;
  R3BSofMusicMadcVftxMappedData** evt_mappedSofTwim;
#endif


  // --- -------------------- --- //
  // --- FINE TIME HISTOGRAMS --- //
  // --- -------------------- --- //
  char name[100];
  Int_t ctDet1Pmt1, ctDet1Pmt2;
  Int_t ctDet2Pmt1, ctDet2Pmt2;
#if SOF_COMREF
  TH1I * h1_mappedSofComfRef_ft[NUM_SOFCOMREF_SIGNALS];
  h1_mappedSofComfRef_ft[0] = new TH1I("SofComRef_FineTime_S2","SofComRef_FineTime_S2",1000,0,1000);
  h1_mappedSofComfRef_ft[1] = new TH1I("SofComRef_FineTime_CaveC","SofComRef_FineTime_CaveC",1000,0,1000);
  TH1I * h1_mappedSofComfRef_ct[NUM_SOFCOMREF_SIGNALS];
  h1_mappedSofComfRef_ct[0] = new TH1I("SofComRef_CoarseTime_S2","SofComRef_CoarseTime_S2",9000,0,9000);
  h1_mappedSofComfRef_ct[1] = new TH1I("SofComRef_CoarseTime_CaveC","SofComRef_CoarseTime_CaveC",9000,0,9000);
  TH1I * h1_mappedSofComRef_DELTAct = new TH1I("SofComRef_DELTAct","SofComRef_DELTAct",2000,-1000,1000);
#endif
#if SOF_SCI
  TH1I * h1_mappedSofSci_ft[NUM_SOFSCI_DETECTORS][NUM_SOFSCI_CHANNELS];
  TH1I * h1_mappedSofSci_ct[NUM_SOFSCI_DETECTORS][NUM_SOFSCI_CHANNELS];
  TH1I * h1_mappedSofSci_DELTAct[NUM_SOFSCI_DETECTORS];
  for(UShort_t det=0; det<NUM_SOFSCI_DETECTORS; det++){
    for(UShort_t pmt=0; pmt<NUM_SOFSCI_CHANNELS; pmt++){
      sprintf(name,"SofSci_Det%i_Pmt%i_FineTime",det+1,pmt+1);
      h1_mappedSofSci_ft[det][pmt] = new TH1I(name,name,1000,0,1000);
      sprintf(name,"SofSci_Det%i_Pmt%i_CoarseTime",det+1,pmt+1);
      h1_mappedSofSci_ct[det][pmt] = new TH1I(name,name,9000,0,9000);
    }
    sprintf(name,"SofSci_DELTAct_Det%i",det+1);
    h1_mappedSofSci_DELTAct[det] = new TH1I(name,name,1000,-500,500);
    h1_mappedSofSci_DELTAct[det]->SetLineWidth(2);
  }
#endif
#if SOF_TOFW
  TH1I * h1_mappedSofToFW_ft[NUM_SOFTOFW_DETECTORS][NUM_SOFTOFW_CHANNELS];
  for(UShort_t det=0; det<NUM_SOFTOFW_DETECTORS; det++){
    for(UShort_t pmt=0; pmt<NUM_SOFTOFW_CHANNELS; pmt++){
      sprintf(name,"SofToFW_Det%i_Pmt%i_FineTime",det+1,pmt+1);
      h1_mappedSofToFW_ft[det][pmt] = new TH1I(name,name,1000,0,1000);
    }
  }
#endif
#if SOF_TRIM
  TH1I * h1_mappedSofTrim_ft[NUM_SOFTRIM_PLANES][NUM_SOFTRIM_SECTIONS][NUM_SOFTRIM_ANODES];
  for(UShort_t plane=0; plane<NUM_SOFTRIM_PLANES; plane++){
    for(UShort_t section=0; section<NUM_SOFTRIM_SECTIONS; section++){
      for(UShort_t anode=0; anode<NUM_SOFTRIM_ANODES; anode++){
	sprintf(name,"SofTrim_Plane%i_Section%i_Anode%i_FineTime",plane+1,section+1,anode+1);
	h1_mappedSofTrim_ft[plane][section][anode] = new TH1I(name,name,1000,0,1000);
      }
    }}
  
#endif
#if SOF_TWIM
  TH1I * h1_mappedSofTwim_ft[NUM_SOFTWIM_PLANES][NUM_SOFTWIM_SECTIONS][NUM_SOFTWIM_ANODES];
  for(UShort_t plane=0; plane<NUM_SOFTWIM_PLANES; plane++){
    for(UShort_t section=0; section<NUM_SOFTWIM_SECTIONS; section++){
      for(UShort_t anode=0; anode<NUM_SOFTWIM_ANODES; anode++){
	sprintf(name,"SofTwim_Plane%i_Section%i_Anode%i_FineTime",plane+1,section+1,anode+1);
	h1_mappedSofTwim_ft[plane][section][anode] = new TH1I(name,name,1000,0,1000);
      }
    }
  }
#endif
  
  // --- -------------------- --- //
  // --- LOOP OVER THE EVENTS --- //
  // --- -------------------- --- //
  Long64_t nevents = t->GetEntries();
  cout << "Number of entries : " << nevents << endl;
  for(int Event=0; Event<nevents; Event++){
    if((Event%10000)==0) cout << "Event " << Event << endl;
    t->GetEvent(Event);
    
#if SOF_COMREF
    nHits_mappedSofComRef = mappedSofComRef->GetEntries();
    if(nHits_mappedSofComRef!=0){
      evt_mappedSofComRef = new R3BSofComRefMappedData*[nHits_mappedSofComRef];
      for(Int_t hit=0; hit<nHits_mappedSofComRef; hit++) {
	evt_mappedSofComRef[hit] = (R3BSofComRefMappedData*)mappedSofComRef->At(hit);
	h1_mappedSofComfRef_ft[evt_mappedSofComRef[hit]->GetDetector()-1]->Fill(evt_mappedSofComRef[hit]->GetTimeFine());
	h1_mappedSofComfRef_ct[evt_mappedSofComRef[hit]->GetDetector()-1]->Fill(evt_mappedSofComRef[hit]->GetTimeCoarse());
      }
      if(nHits_mappedSofComRef==2)      h1_mappedSofComRef_DELTAct->Fill(evt_mappedSofComRef[0]->GetTimeCoarse()-evt_mappedSofComRef[1]->GetTimeCoarse());
    }
#endif
#if SOF_SCI
    nHits_mappedSofSci = mappedSofSci->GetEntries();
    if(nHits_mappedSofSci!=0){
      evt_mappedSofSci = new R3BSofSciMappedData*[nHits_mappedSofSci];
      ctDet1Pmt1=-1; //S2 / right
      ctDet1Pmt2=-1; //S2 / left
      ctDet2Pmt1=-1; //CC / right
      ctDet2Pmt2=-1; //CC / left
      for(Int_t hit=0; hit<nHits_mappedSofSci; hit++) {
	evt_mappedSofSci[hit] = (R3BSofSciMappedData*)mappedSofSci->At(hit);
	h1_mappedSofSci_ft[evt_mappedSofSci[hit]->GetDetector()-1][evt_mappedSofSci[hit]->GetPmt()-1]->Fill(evt_mappedSofSci[hit]->GetTimeFine());
	h1_mappedSofSci_ct[evt_mappedSofSci[hit]->GetDetector()-1][evt_mappedSofSci[hit]->GetPmt()-1]->Fill(evt_mappedSofSci[hit]->GetTimeCoarse());
	if((evt_mappedSofSci[hit]->GetDetector()==1)&&(evt_mappedSofSci[hit]->GetPmt()==1)) ctDet1Pmt1 = evt_mappedSofSci[hit]->GetTimeCoarse();
	if((evt_mappedSofSci[hit]->GetDetector()==1)&&(evt_mappedSofSci[hit]->GetPmt()==2)) ctDet1Pmt2 = evt_mappedSofSci[hit]->GetTimeCoarse();
	if((evt_mappedSofSci[hit]->GetDetector()==2)&&(evt_mappedSofSci[hit]->GetPmt()==1)) ctDet2Pmt1 = evt_mappedSofSci[hit]->GetTimeCoarse();
	if((evt_mappedSofSci[hit]->GetDetector()==2)&&(evt_mappedSofSci[hit]->GetPmt()==2)) ctDet2Pmt2 = evt_mappedSofSci[hit]->GetTimeCoarse();
      }
      if((nHits_mappedSofSci==4)&&(ctDet1Pmt1!=-1)&&(ctDet1Pmt2!=-1)&&(ctDet2Pmt1!=-1)&&(ctDet2Pmt2!=-1)){
	h1_mappedSofSci_DELTAct[0]->Fill(ctDet1Pmt1-ctDet1Pmt2);
	h1_mappedSofSci_DELTAct[1]->Fill(ctDet2Pmt1-ctDet2Pmt2);
      }
    }
#endif
#if SOF_TOFW
    nHits_mappedSofToFW = mappedSofToFW->GetEntries();
    if(nHits_mappedSofToFW!=0){
      evt_mappedSofToFW = new R3BSofToFWMappedData*[nHits_mappedSofToFW];
      for(Int_t hit=0; hit<nHits_mappedSofToFW; hit++) {
	evt_mappedSofToFW[hit] = (R3BSofToFWMappedData*)mappedSofToFW->At(hit);
	h1_mappedSofToFW_ft[evt_mappedSofToFW[hit]->GetDetector()-1][evt_mappedSofToFW[hit]->GetPmt()-1]->Fill(evt_mappedSofToFW[hit]->GetTimeFine());
      }
    }
#endif
#if SOF_TRIM
    nHits_mappedSofTrim = mappedSofTrim->GetEntries();
    if(nHits_mappedSofTrim!=0){
      evt_mappedSofTrim = new R3BSofMusicMadcVftxMappedData*[nHits_mappedSofTrim];
      for(Int_t hit=0; hit<nHits_mappedSofTrim; hit++) {
	evt_mappedSofTrim[hit] = (R3BSofMusicMadcVftxMappedData*)mappedSofTrim->At(hit);
	h1_mappedSofTrim_ft[evt_mappedSofTrim[hit]->GetPlane()-1][evt_mappedSofTrim[hit]->GetSection()-1][evt_mappedSofTrim[hit]->GetAnode()-1]->Fill(evt_mappedSofTrim[hit]->GetTimeFine());
      }
    }
#endif
#if SOF_TWIM
    nHits_mappedSofTwim = mappedSofTwim->GetEntries();
    if(nHits_mappedSofTwim!=0){
      evt_mappedSofTwim = new R3BSofMusicMadcVftxMappedData*[nHits_mappedSofTwim];
      for(Int_t hit=0; hit<nHits_mappedSofTwim; hit++) {
	evt_mappedSofTwim[hit] = (R3BSofMusicMadcVftxMappedData*)mappedSofTwim->At(hit);
	h1_mappedSofTwim_ft[evt_mappedSofTwim[hit]->GetPlane()-1][evt_mappedSofTwim[hit]->GetSection()-1][evt_mappedSofTwim[hit]->GetAnode()-1]->Fill(evt_mappedSofTwim[hit]->GetTimeFine());
      }
    }
#endif
    
  }// end of for(Event)


  char nameCan[100];
  
#if SOF_COMREF
  TCanvas * can_SofComRef_ft = new TCanvas("FineTime_SofComRef","FineTime_SofComRef",0,0,1000,500);
  can_SofComRef_ft->Divide(2,1);
  can_SofComRef_ft->cd(1); gPad->SetGridx(); gPad->SetGridy(); h1_mappedSofComfRef_ft[0]->SetLineColor(kBlue+2); h1_mappedSofComfRef_ft[0]->Draw();
  can_SofComRef_ft->cd(2); gPad->SetGridx(); gPad->SetGridy(); h1_mappedSofComfRef_ft[1]->SetLineColor(kBlue+2); h1_mappedSofComfRef_ft[1]->Draw();
  TCanvas * can_SofComRef_ct = new TCanvas("CoarseTime_SofComRef","CoarseTime_SofComRef",0,0,1200,400);
  can_SofComRef_ct->Divide(3,1);
  can_SofComRef_ct->cd(1); gPad->SetGridx(); gPad->SetGridy(); h1_mappedSofComfRef_ct[0]->SetLineColor(kBlue+2); h1_mappedSofComfRef_ct[0]->Draw();
  can_SofComRef_ct->cd(2); gPad->SetGridx(); gPad->SetGridy(); h1_mappedSofComfRef_ct[1]->SetLineColor(kBlue+2); h1_mappedSofComfRef_ct[1]->Draw();
  can_SofComRef_ct->cd(3); gPad->SetGridx(); gPad->SetGridy(); h1_mappedSofComRef_DELTAct->SetLineColor(kBlue+2); h1_mappedSofComRef_DELTAct->Draw();
#endif
#if SOF_SCI
  TCanvas * can_SofSci_ft = new TCanvas("FineTime_SofSci","FineTime_SofSci",0,0,1000,1000);
  can_SofSci_ft->Divide(2,2);
  can_SofSci_ft->cd(1); gPad->SetGridx(); gPad->SetGridy(); h1_mappedSofSci_ft[0][0]->SetLineColor(kBlue+2); h1_mappedSofSci_ft[0][0]->Draw();
  can_SofSci_ft->cd(2); gPad->SetGridx(); gPad->SetGridy(); h1_mappedSofSci_ft[0][1]->SetLineColor(kBlue+2); h1_mappedSofSci_ft[0][1]->Draw();
  can_SofSci_ft->cd(3); gPad->SetGridx(); gPad->SetGridy(); h1_mappedSofSci_ft[1][0]->SetLineColor(kBlue+2); h1_mappedSofSci_ft[1][0]->Draw();
  can_SofSci_ft->cd(4); gPad->SetGridx(); gPad->SetGridy(); h1_mappedSofSci_ft[1][1]->SetLineColor(kBlue+2); h1_mappedSofSci_ft[1][1]->Draw();
  TCanvas * can_SofSci_ct = new TCanvas("CoarseTime_SofSci","CoarseTime_SofSci",0,0,1000,1000);
  can_SofSci_ct->Divide(2,2);
  can_SofSci_ct->cd(1); gPad->SetGridx(); gPad->SetGridy(); h1_mappedSofSci_ct[0][0]->SetLineColor(kBlue+2); h1_mappedSofSci_ct[0][0]->Draw();
  can_SofSci_ct->cd(2); gPad->SetGridx(); gPad->SetGridy(); h1_mappedSofSci_ct[0][1]->SetLineColor(kBlue+2); h1_mappedSofSci_ct[0][1]->Draw();
  can_SofSci_ct->cd(3); gPad->SetGridx(); gPad->SetGridy(); h1_mappedSofSci_ct[1][0]->SetLineColor(kBlue+2); h1_mappedSofSci_ct[1][0]->Draw();
  can_SofSci_ct->cd(4); gPad->SetGridx(); gPad->SetGridy(); h1_mappedSofSci_ct[1][1]->SetLineColor(kBlue+2); h1_mappedSofSci_ct[1][1]->Draw();
  TCanvas * can_SofSci_DeltaCt = new TCanvas("DeltaCt_SofSci","DeltaCt_SofSci",0,0,1000,500);
  can_SofSci_DeltaCt->Divide(2,1);
  can_SofSci_DeltaCt->cd(1); gPad->SetGridx(); gPad->SetGridy(); h1_mappedSofSci_DELTAct[0]->SetLineColor(kBlue+2);h1_mappedSofSci_DELTAct[0]->Draw();
  can_SofSci_DeltaCt->cd(2); gPad->SetGridx(); gPad->SetGridy(); h1_mappedSofSci_DELTAct[1]->SetLineColor(kBlue+2);h1_mappedSofSci_DELTAct[1]->Draw();
#endif
#if SOF_TOFW
  TCanvas * can_ToFW_ft[2];
  can_ToFW_ft[0] = new TCanvas("FineTime_ToFW_Pmt1Down","FineTime_ToFW_Pmt1Down",0,0,1500,1400);  can_ToFW_ft[0]->Divide(7,4);
  can_ToFW_ft[1] = new TCanvas("FineTime_ToFW_Pmt2Up","FineTime_ToFW_Pmt2Up",0,0,1500,1400);      can_ToFW_ft[1]->Divide(7,4);
  for(UShort_t det=0; det<NUM_SOFTOFW_DETECTORS; det++){
    can_ToFW_ft[0]->cd(det+1); gPad->SetGridx(); gPad->SetGridy(); h1_mappedSofToFW_ft[det][0]->SetLineColor(kBlue+2); h1_mappedSofToFW_ft[det][0]->Draw();
    can_ToFW_ft[1]->cd(det+1); gPad->SetGridx(); gPad->SetGridy(); h1_mappedSofToFW_ft[det][1]->SetLineColor(kBlue+2); h1_mappedSofToFW_ft[det][1]->Draw();
  }
#endif
#if SOF_TRIM
  TCanvas * can_Trim_ft = new TCanvas("FineTime_TripleMUSIC","FineTime_TripleMUSIC",0,0,1300,1000);
  can_Trim_ft->Divide(6,3);
  for(UShort_t plane=0; plane<NUM_SOFTRIM_PLANES; plane++){
    for(UShort_t section=0; section<NUM_SOFTRIM_SECTIONS; section++){
      for(UShort_t anode=0; anode<NUM_SOFTRIM_ANODES; anode++){
	can_Trim_ft->cd(plane*NUM_SOFTRIM_SECTIONS*NUM_SOFTRIM_ANODES + section*NUM_SOFTRIM_ANODES + anode + 1 ); gPad->SetGridx(); gPad->SetGridy(); 
	h1_mappedSofTrim_ft[plane][section][anode]->SetLineColor(kBlue+2); 
	h1_mappedSofTrim_ft[plane][section][anode]->Draw();
      }
    }
  }
#endif
#if SOF_TWIM
  const char * namePlane[NUM_SOFTWIM_PLANES] = {"Right","Left"};
  const char * nameSection[NUM_SOFTWIM_SECTIONS] = {"Down","Up"};
  TCanvas * can_Twim_ft[4];
  for(UShort_t plane=0; plane<NUM_SOFTWIM_PLANES; plane++){
    for(UShort_t section=0; section<NUM_SOFTWIM_SECTIONS; section++){
      sprintf(nameCan,"FineTime_TwinMUSIC_%s_%s",namePlane[plane],nameSection[section]);
      can_Twim_ft[plane*NUM_SOFTWIM_SECTIONS + section] = new TCanvas(nameCan,nameCan,0,0,1300,1000);
      can_Twim_ft[plane*NUM_SOFTWIM_SECTIONS + section]->Divide(4,4);
      for(UShort_t anode=0; anode<NUM_SOFTWIM_ANODES; anode++){
	can_Twim_ft[plane*NUM_SOFTWIM_SECTIONS + section]->cd(anode+1);
	gPad->SetGridx(); gPad->SetGridy(); 
	h1_mappedSofTwim_ft[plane][section][anode]->SetLineColor(kBlue+2); 
	h1_mappedSofTwim_ft[plane][section][anode]->Draw();
      }
    }
  }
#endif
  
}
