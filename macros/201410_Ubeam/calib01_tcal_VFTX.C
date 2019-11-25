#define SCI_MAPPED    1
#define TOFW_MAPPED   1

#define SCI_TCALPAR   1
#define TOFW_TCALPAR  1

#define SAVE_TCALPAR_ROOT 0


struct EXT_STR_h101_t
{
  EXT_STR_h101_unpack_t unpack;
#if SCI_MAPPED
  EXT_STR_h101_SOFSCI_onion_t sci;
#endif
#if TOFW_MAPPED
  EXT_STR_h101_SOFTOFW_onion_t tofw;
#endif
};

void calib01_tcal_VFTX(Int_t First=1320)
{
  TString runNumber=Form ("%03d", First);
  TStopwatch timer;
  timer.Start();

  const Int_t nev = -1; /* number of events to read, -1 - until CTRL+C */

  /* Create source using ucesb for input ------------------ */
     
  //TString filename = " --stream=lxir123 ";
  TString filename = "/media/audrey/MANGUE/SOFIA/ANALYSE/SOFIA2/data/lmd_timestitched/run*.lmd";
  TString outputFileName = "../SofMacrosOutput/201410_Ubeam/calib_output_tcal_VFTX_All.root";
  TString outputFileNamePar = "../SofMacrosOutput/201410_Ubeam/Parameters/tcal_VFTX_All";
  TString ntuple_options = "UNPACK:EVENTNO,UNPACK:TRIGGER,RAW";
  TString ucesb_dir = getenv("UCESB_DIR");
  TString ucesb_path = ucesb_dir + "/../upexps/sofia2014oct/sofia2014oct";
  
  EXT_STR_h101 ucesb_struct;
  R3BUcesbSource* source = new R3BUcesbSource(filename, ntuple_options,ucesb_path, &ucesb_struct, sizeof(ucesb_struct));
  source->SetMaxEvents(nev);
  
  source->AddReader(new R3BUnpackReader((EXT_STR_h101_unpack_t *)&ucesb_struct,offsetof(EXT_STR_h101, unpack)));
#if SCI_MAPPED
  source->AddReader(new R3BSofSciReader((EXT_STR_h101_SOFSCI_t *)&ucesb_struct.sci,offsetof(EXT_STR_h101, sci)));
#endif
#if TOFW_MAPPED
  source->AddReader(new R3BSofToFWReader((EXT_STR_h101_SOFTOFW_t *)&ucesb_struct.tofw,offsetof(EXT_STR_h101, tofw)));
#endif
    
  const Int_t refresh = 1000;  /* refresh rate for saving */

  /* Create online run ------------------------------------ */
  FairRunOnline* run = new FairRunOnline(source);
  run->SetOutputFile(outputFileName);

#if SCI_TCALPAR || TOFW_TCALPAR 
  R3BSofTcalContFact needToConstructTcalContFact;
#endif

#if SCI_TCALPAR
  /* Calibrate SofSci+ComRef ---------------------------------------- */
  R3BSofSciMapped2TcalPar* sciTcalibrator = new R3BSofSciMapped2TcalPar("R3BSofSciMapped2TcalPar");
  sciTcalibrator->SetNumDetectors(2);
  sciTcalibrator->SetNumSections(1);
  sciTcalibrator->SetNumChannels(3);
  sciTcalibrator->SetNumSignals(2,1,3);
  sciTcalibrator->SetMinStatistics(1000000);
  sciTcalibrator->SetMinStatistics(250000);
  run->AddTask(sciTcalibrator);
#endif

#if TOFW_TCALPAR
  /* Calibrate time-of-flight wall  ---------------------------------------- */
  R3BSofToFWMapped2TcalPar* tofwTcalibrator = new R3BSofToFWMapped2TcalPar("R3BSofToFWMapped2TcalPar");
  tofwTcalibrator->SetNumDetectors(28);
  tofwTcalibrator->SetNumSections(1);
  tofwTcalibrator->SetNumChannels(2);
  tofwTcalibrator->SetNumSignals(28,1,2);
  tofwTcalibrator->SetMinStatistics(100000);
  run->AddTask(tofwTcalibrator);
#endif

  /* Runtime data base ------------------------------------ */
  FairRuntimeDb* rtdb = run->GetRuntimeDb();
#if SCI_TCALPAR || TOFW_TCALPAR 
  //Choose Root or Ascii file	
#if SAVE_TCALPAR_ROOT
  // Root file with the Calibration Parameters
  Bool_t kParameterMerged = kTRUE;
  FairParRootFileIo* parOut = new FairParRootFileIo(kParameterMerged);
  outputFileNamePar = outputFileNamePar+ ".root";
  parOut->open(outputFileNamePar);
  rtdb->setFirstInput(parOut);
  rtdb->setOutput(parOut);
#else
  // Ascii file with the Calibration Parameters
  FairParAsciiFileIo* parOut = new FairParAsciiFileIo();
  outputFileNamePar = outputFileNamePar+ ".par";
  parOut->open(outputFileNamePar,"out");
  rtdb->setOutput(parOut);
#endif
#endif
  
  
  /* Initialize ------------------------------------------- */
  run->Init();
  FairLogger::GetLogger()->SetLogScreenLevel("INFO");
  //FairLogger::GetLogger()->SetLogScreenLevel("WARNING");
  /* ------------------------------------------------------ */
  
  /* Run -------------------------------------------------- */
  run->Run((nev < 0) ? nev : 0, (nev < 0) ? 0 : nev);
  if (rtdb->getCurrentRun()) cout << "have run" << endl;
  else cout << "have no run" << endl;
  rtdb->saveOutput();
  /* ------------------------------------------------------ */

  
  timer.Stop();
  Double_t rtime = timer.RealTime();
  Double_t ctime = timer.CpuTime();
  cout << endl << endl;
  cout << "Macro finished succesfully." << endl;
  cout << "Output file is " << outputFileName << endl;
  cout << "Real time " << rtime << " s, CPU time " << ctime << " s"
       << endl << endl;


  // --- -------------------------------------------------------- --- //
  // --- DRAW TCAL HISTOGRAMS CREATED R3BSofXXXMapped2TcalPar.cxx --- //
  // --- -------------------------------------------------------- --- //

  TFile * file = TFile::Open(outputFileName);
  char name[100];
  int signal, nsig,ndet,nsec,nch;

#if SCI_TCALPAR
  TCanvas * can_SofSci = new TCanvas("SofSci_TcalPar","SofSci_TcalPar",0,0,1000,1000);
  can_SofSci->Divide(3,2);
  TH1F * h1_sofsci_ftns[6];
  for(int det=0; det<sciTcalibrator->GetNumDetectors(); det++){
    for(int ch=0; ch<sciTcalibrator->GetNumChannels(); ch++){
      signal = det*sciTcalibrator->GetNumChannels()+ch;
      sprintf(name,"TimeFineNs_Sci%i_Ch%i_Sig%i",det+1,ch+1,signal);
      h1_sofsci_ftns[signal] = (TH1F*)file->Get(name);
      can_SofSci->cd(signal+1);gPad->SetGridx(); gPad->SetGridy();
      h1_sofsci_ftns[signal]->SetLineWidth(2);
      h1_sofsci_ftns[signal]->Draw();
    }
  }
#endif
#if TOFW_TCALPAR
  const char * namePmt[2] = {"Down","Up"};
  TCanvas * can_SofToFW[2];
  TH1F * h1_softofw_ftns[56];
  for(int pmt=0; pmt<tofwTcalibrator->GetNumChannels(); pmt++){
    sprintf(name,"SofToFW_TcalPar_Pmt%s",namePmt[pmt]);
    can_SofToFW[pmt] = new TCanvas(name,name,0,0,1000,1000);
    can_SofToFW[pmt]->Divide(7,4);
    for(int plastic=0; plastic<tofwTcalibrator->GetNumDetectors(); plastic++){
      signal = plastic*tofwTcalibrator->GetNumChannels()+pmt;
      sprintf(name,"TimeFineNs_ToFW_P%i_Pmt%i_Sig%i",plastic+1,pmt+1,signal);
      h1_softofw_ftns[signal] = (TH1F*)file->Get(name);
      if(!h1_softofw_ftns[signal]) continue;
      can_SofToFW[pmt]->cd(plastic+1);gPad->SetGridx(); gPad->SetGridy();
      h1_softofw_ftns[signal]->SetLineWidth(2);
      h1_softofw_ftns[signal]->Draw();
    }
  }
#endif

}
