#include <TCanvas.h>
#include <TH1.h>


#define SOF_COMREF 1
#define SOF_SCI    1
#define SOF_TOFW   1

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
// see sofsource/R3BSofSciToFW.cxx
#define NUM_SOFTOFW_DETECTORS 28
#define NUM_SOFTOFW_CHANNELS  2
#endif


void ViewFineTime(){


  // --- ---------------- --- //
  // --- ROOT ENVIRONMENT --- //
  // --- ---------------- --- //
  gROOT->SetStyle("Plain");
  gStyle->SetOptStat(1);
  gStyle->SetOptFit(0);

  // --- ---------- --- //
  // --- INPUT DATA --- //
  // --- ---------- --- //
  char inputFile[250] = "../../../../SofMacrosOutput/201410_Ubeam/SofDataVftx_run1322.root";  //change root file
  TFile * file = TFile::Open(inputFile);
  TTree * t = (TTree*)file->Get("evt");
  t->ls();
  //t->Print();

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


  // --- -------------------- --- //
  // --- FINE TIME HISTOGRAMS --- //
  // --- -------------------- --- //
  char name[100];
#if SOF_COMREF
  TH1I * h1_mappedSofComfRef_ft[NUM_SOFCOMREF_SIGNALS];
  h1_mappedSofComfRef_ft[0] = new TH1I("SofComRef_FineTime_S2","SofComRef_FineTime_S2",1000,0,1000);
  h1_mappedSofComfRef_ft[1] = new TH1I("SofComRef_FineTime_CaveC","SofComRef_FineTime_CaveC",1000,0,1000);
#endif
#if SOF_SCI
  TH1I * h1_mappedSofSci_ft[NUM_SOFSCI_DETECTORS][NUM_SOFSCI_CHANNELS];
  for(UShort_t det=0; det<NUM_SOFSCI_DETECTORS; det++){
    for(UShort_t pmt=0; pmt<NUM_SOFSCI_CHANNELS; pmt++){
      sprintf(name,"SofSci_Det%i_Pmt%i_FineTime",det+1,pmt+1);
      h1_mappedSofSci_ft[det][pmt] = new TH1I(name,name,1000,0,1000);
    }
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
      }
    }
#endif
#if SOF_SCI
    nHits_mappedSofSci = mappedSofSci->GetEntries();
    if(nHits_mappedSofSci!=0){
      evt_mappedSofSci = new R3BSofSciMappedData*[nHits_mappedSofSci];
      for(Int_t hit=0; hit<nHits_mappedSofSci; hit++) {
	evt_mappedSofSci[hit] = (R3BSofSciMappedData*)mappedSofSci->At(hit);
	h1_mappedSofSci_ft[evt_mappedSofSci[hit]->GetDetector()-1][evt_mappedSofSci[hit]->GetPmt()-1]->Fill(evt_mappedSofSci[hit]->GetTimeFine());
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
    
  }// end of for(Event)

#if SOF_COMREF
  TCanvas * can_SofComRef_ft = new TCanvas("FineTime_SofComRef","FineTime_SofComRef",0,0,1000,500);
  can_SofComRef_ft->Divide(2,1);
  can_SofComRef_ft->cd(1); gPad->SetGridx(); gPad->SetGridy(); h1_mappedSofComfRef_ft[0]->SetLineColor(kBlue+2); h1_mappedSofComfRef_ft[0]->Draw();
  can_SofComRef_ft->cd(2); gPad->SetGridx(); gPad->SetGridy(); h1_mappedSofComfRef_ft[1]->SetLineColor(kBlue+2); h1_mappedSofComfRef_ft[1]->Draw();
#endif
#if SOF_SCI
  TCanvas * can_SofSci_ft = new TCanvas("FineTime_SofSci","FineTime_SofSci",0,0,1000,1000);
  can_SofSci_ft->Divide(2,2);
  can_SofSci_ft->cd(1); gPad->SetGridx(); gPad->SetGridy(); h1_mappedSofSci_ft[0][0]->SetLineColor(kBlue+2); h1_mappedSofSci_ft[0][0]->Draw();
  can_SofSci_ft->cd(2); gPad->SetGridx(); gPad->SetGridy(); h1_mappedSofSci_ft[0][1]->SetLineColor(kBlue+2); h1_mappedSofSci_ft[0][1]->Draw();
  can_SofSci_ft->cd(3); gPad->SetGridx(); gPad->SetGridy(); h1_mappedSofSci_ft[1][0]->SetLineColor(kBlue+2); h1_mappedSofSci_ft[1][0]->Draw();
  can_SofSci_ft->cd(4); gPad->SetGridx(); gPad->SetGridy(); h1_mappedSofSci_ft[1][1]->SetLineColor(kBlue+2); h1_mappedSofSci_ft[1][1]->Draw();
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

}
