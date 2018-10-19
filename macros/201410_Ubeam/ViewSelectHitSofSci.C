#include <TCanvas.h>
#include <TH1.h>
#include <string.h>


void ViewSelectHitSofSci(UInt_t FirstRun=1355){
  
  TString runNumber=Form ("%03d", FirstRun);
  
  // --- ---------------- --- //
  // --- ROOT ENVIRONMENT --- //
  // --- ---------------- --- //
  gROOT->SetStyle("Plain");
  gStyle->SetOptStat(1);
  gStyle->SetOptFit(0);
  
  // --- ---------- --- //
  // --- INPUT DATA --- //
  // --- ---------- --- //
  TString filename;
  if(FirstRun==1331){
    filename = "../SofMacrosOutput/201410_Ubeam/tcal_Fr207.root";
  }
  else if (FirstRun==1355){
    filename = "../SofMacrosOutput/201410_Ubeam/tcal_Bi194.root";
  }
  else if(FirstRun==1365){
    filename = "../SofMacrosOutput/201410_Ubeam/tcal_Tl187.root";
  }
  else{
    filename = "../SofMacrosOutput/201410_Ubeam/tcal_run"+runNumber+".root";
  }
  TFile * file = TFile::Open(filename);
  TTree * t = (TTree*)file->Get("evt");
  t->ls();
  
  // --- --------- --- //
  // --- TCAL DATA --- //
  // --- --------- --- //
  TClonesArray * tcalSofSci = new TClonesArray("R3BSofSciTcalData",5);
  TBranch * b_tcalSofSci = t->GetBranch("SofSciTcal");
  b_tcalSofSci->SetAddress(&tcalSofSci);
  b_tcalSofSci->ls();
  Int_t nHits_tcalSofSci=0;
  R3BSofSciTcalData** evt_tcalSofSci;
  

  Double_t DeltaT[2];
  Double_t MeanT[2];
  Double_t TofRaw;
  
  // --- -------------------- --- //
  // --- FINE TIME HISTOGRAMS --- //
  // --- -------------------- --- //
  char name[100];

  // RAW POSITIONS
  TH1D * h1_tcalSofSci_RawPos[2];
  for(UShort_t det=0; det<2; det++){
    sprintf(name,"SofSci_Det%i_DeltaTns",det+1);
    h1_tcalSofSci_RawPos[det] = new TH1D(name,name,25000,15.,40.);
    h1_tcalSofSci_RawPos[det]->GetXaxis()->SetTitle("Tleft - Tright [ns]");
  }
  TH2D * h2_tcalSofSci_RawPos_S2vsCC = new TH2D("DeltaTns_S2vsCC","DeltaTns_S2vsCC",3000,19,22,2000,35,37);

  // RAW ToF
  TH1D * h1_TofRaw = new TH1D("SofSci_TofRaw","SofSci_TofRaw",15000,1704,1719);

  // RAW TOF vs RAW POS 
  TH2D * h2_TofRaw_vs_PosRaw[2];
  h2_TofRaw_vs_PosRaw[0] = new TH2D("S2_TofRaw_vs_DeltaT","S2_TofRaw_vs_DeltaT",2000,35,37,3000,1704,1719);
  h2_TofRaw_vs_PosRaw[1] = new TH2D("CaveC_TofRaw_vs_DeltaT","CaveC_TofRaw_vs_DeltaT",3000,19,22,3000,1704,1719);


  // --- -------------------- --- //
  // --- LOOP OVER THE EVENTS --- //
  // --- -------------------- --- //
  Long64_t nevents = t->GetEntries();
  cout << "Number of entries : " << nevents << endl;
  for(int Event=0; Event<nevents; Event++){
    if((Event%10000)==0) cout << "Event " << Event << endl;
    t->GetEvent(Event);

    UInt_t iDet;
    UInt_t iCh;
    int signal;
    Double_t iTns;
    Double_t Tns[6];
    for(UShort_t sig=0; sig<6; sig++) Tns[sig] = -1;
    nHits_tcalSofSci = tcalSofSci->GetEntries();
    if(nHits_tcalSofSci==6){  // SELECT MULT=1 
      evt_tcalSofSci = new R3BSofSciTcalData*[nHits_tcalSofSci];
      for(Int_t hit=0; hit<nHits_tcalSofSci; hit++) {
	evt_tcalSofSci[hit] = (R3BSofSciTcalData*)tcalSofSci->At(hit);
	iDet = evt_tcalSofSci[hit]->GetDetector();
	iCh = evt_tcalSofSci[hit]->GetPmt();
	signal = (iDet-1)*3 + iCh-1;
	if( ((iDet==1)||(iDet==2)) && ((1<=iCh)&&(iCh<=3)) ){
	  iTns = evt_tcalSofSci[hit]->GetRawTimeNs();
	  Tns[signal] = iTns;
	}
	else{
	  cout << "ERROR : iDet=" << iDet << ", iCh=" << iCh << endl;
	  continue;
	}
      }
      if ((Tns[0]!=-1) && (Tns[1]!=-1) && (Tns[2]!=-1) && (Tns[3]!=-1) && (Tns[4]!=-1) && (Tns[5]!=-1)){
	DeltaT[0] = Tns[2]-Tns[1]; h1_tcalSofSci_RawPos[0]->Fill(DeltaT[0]);
	DeltaT[1] = Tns[5]-Tns[4]; h1_tcalSofSci_RawPos[1]->Fill(DeltaT[1]);
	h2_tcalSofSci_RawPos_S2vsCC->Fill(DeltaT[1],DeltaT[0]);

	MeanT[0]  = 0.5*(Tns[2]+Tns[1]);
	MeanT[1]  = 0.5*(Tns[5]+Tns[4]);
	TofRaw = MeanT[1] - MeanT[0] + Tns[0] - Tns[3];
	h1_TofRaw->Fill(TofRaw);
	h2_TofRaw_vs_PosRaw[0]->Fill(DeltaT[0],TofRaw);
	h2_TofRaw_vs_PosRaw[1]->Fill(DeltaT[1],TofRaw);

      }
    }// end of if nHits=6
  }// end of for(Event)


  char nameCan[100];
  
  TCanvas * can_SofSci_RawPos = new TCanvas("RawPos_SofSci","RawPos_SofSci",0,0,1200,400);
  can_SofSci_RawPos->Divide(3,1);
  can_SofSci_RawPos->cd(1); gPad->SetGridx(); gPad->SetGridy(); h1_tcalSofSci_RawPos[0]->SetLineColor(kBlue+2); h1_tcalSofSci_RawPos[0]->Draw();
  can_SofSci_RawPos->cd(2); gPad->SetGridx(); gPad->SetGridy(); h1_tcalSofSci_RawPos[1]->SetLineColor(kBlue+2); h1_tcalSofSci_RawPos[1]->Draw();
  can_SofSci_RawPos->cd(3); gPad->SetGridx(); gPad->SetGridy(); h2_tcalSofSci_RawPos_S2vsCC->Draw("COL");
  
  TCanvas * can_SofSci_TofRaw = new TCanvas("RawTof_SofSci","RawTof_SofSci",0,0,1200,1200);
  can_SofSci_TofRaw->Divide(2,2);
  can_SofSci_TofRaw->cd(1); gPad->SetGridx(); gPad->SetGridy(); h1_TofRaw->SetLineColor(kBlue+2); h1_TofRaw->Draw();
  can_SofSci_TofRaw->cd(3); gPad->SetGridx(); gPad->SetGridy(); h2_TofRaw_vs_PosRaw[0]->Draw("COL");
  can_SofSci_TofRaw->cd(4); gPad->SetGridx(); gPad->SetGridy(); h2_TofRaw_vs_PosRaw[1]->Draw("COL");
  
  TH1D * h1_TofRaw_Integral = new TH1D("TofRaw_IntegralPerBin","TofRaw_IntegralPerBin",h1_TofRaw->GetNbinsX(),h1_TofRaw->GetBinLowEdge(1),h1_TofRaw->GetBinLowEdge(h1_TofRaw->GetNbinsX())+h1_TofRaw->GetBinWidth(h1_TofRaw->GetNbinsX()));
  for(UShort_t bin=1; bin<=h1_TofRaw->GetNbinsX(); bin++){
    h1_TofRaw_Integral->SetBinContent(bin,h1_TofRaw->Integral(1,bin));
  }
  can_SofSci_TofRaw->cd(2); gPad->SetGridx(); gPad->SetGridy(); h1_TofRaw_Integral->SetLineColor(kBlue+2); h1_TofRaw_Integral->Draw();
  Double_t Amplitude = h1_TofRaw_Integral->GetMaximum()-h1_TofRaw_Integral->GetMinimum();
  Double_t CutMin = 0.003*Amplitude;
  Double_t CutMax = h1_TofRaw_Integral->GetMaximum()-0.003*Amplitude;
  cout << "Amplitude = " << Amplitude << " : CutMin at " << CutMin << ", CutMax at " << CutMax << endl;
  Double_t TofMin = h1_TofRaw_Integral->GetBinCenter(h1_TofRaw_Integral->FindFirstBinAbove(CutMin));
  Double_t TofMax = h1_TofRaw_Integral->GetBinCenter(h1_TofRaw_Integral->FindFirstBinAbove(CutMax));
  cout << "TofMin = " << TofMin << endl;
  cout << "TofMax = " << TofMax << endl;

}
