#define COMREF_MAPPED 1
#define SCI_MAPPED    1
#define TOFW_MAPPED   1
#define TRIM_MAPPED   1
#define TWIM_MAPPED   1

#define SCI_TCALPAR   1
#define TOFW_TCALPAR  1
#define TRIM_TCALPAR  1
#define TWIM_TCALPAR  1

#define SAVE_TCALPAR_ROOT 1


struct EXT_STR_h101_t
{
  EXT_STR_h101_unpack_t unpack;
#if COMREF_MAPPED
  EXT_STR_h101_SOFCOMREF_onion_t comref;
#endif
#if SCI_MAPPED
  EXT_STR_h101_SOFSCI_onion_t sci;
#endif
#if TOFW_MAPPED
  EXT_STR_h101_SOFTOFW_onion_t tofw;
#endif
#if TRIM_MAPPED
  EXT_STR_h101_SOFTRIM_MADCVFTX_onion_t trim;
#endif
#if TWIM_MAPPED
  EXT_STR_h101_SOFTWIM_MADCVFTX_onion_t twim;
#endif
};

void calib_tcal_VFTX(Int_t First=1320)
{
  TString runNumber=Form ("%03d", First);
  TStopwatch timer;
  timer.Start();

  const Int_t nev = -1; /* number of events to read, -1 - until CTRL+C */

  /* Create source using ucesb for input ------------------ */
     
  //TString filename = " --stream=lxir123 ";
  //TString filename = "/media/audrey/COURGE/SOFIA/ANALYSE/SOFIA2/data/lmd_timestitched/run13*.lmd";
  TString filename = "/media/audrey/MANGUE/SOFIA/ANALYSE/SOFIA2/data/lmd_timestitched/run1*.lmd";
  TString outputFileName = "../SofMacrosOutput/201410_Ubeam/calib_tcal_VFTX_run13xx.root";
  TString ntuple_options = "UNPACK:EVENTNO,UNPACK:TRIGGER,RAW";
  TString ucesb_dir = getenv("UCESB_DIR");
  TString ucesb_path = ucesb_dir + "/../upexps/sofia2014oct/sofia2014oct";
  
  EXT_STR_h101 ucesb_struct;
  R3BUcesbSource* source = new R3BUcesbSource(filename, ntuple_options,ucesb_path, &ucesb_struct, sizeof(ucesb_struct));
  source->SetMaxEvents(nev);
  
  source->AddReader(new R3BUnpackReader((EXT_STR_h101_unpack_t *)&ucesb_struct,offsetof(EXT_STR_h101, unpack)));
#if COMREF_MAPPED
  source->AddReader(new R3BSofComRefReader((EXT_STR_h101_SOFCOMREF_t *)&ucesb_struct.comref,offsetof(EXT_STR_h101, comref)));
#endif
#if SCI_MAPPED
  source->AddReader(new R3BSofSciReader((EXT_STR_h101_SOFSCI_t *)&ucesb_struct.sci,offsetof(EXT_STR_h101, sci)));
#endif
#if TOFW_MAPPED
  source->AddReader(new R3BSofToFWReader((EXT_STR_h101_SOFTOFW_t *)&ucesb_struct.tofw,offsetof(EXT_STR_h101, tofw)));
#endif
#if TRIM_MAPPED
  source->AddReader(new R3BSofTrimMadcVftxReader((EXT_STR_h101_SOFTRIM_MADCVFTX_t *)&ucesb_struct.trim,offsetof(EXT_STR_h101, trim)));
#endif
#if TWIM_MAPPED
  source->AddReader(new R3BSofTwimMadcVftxReader((EXT_STR_h101_SOFTWIM_MADCVFTX_t *)&ucesb_struct.twim,offsetof(EXT_STR_h101, twim)));
#endif
    
  const Int_t refresh = 1000;  /* refresh rate for saving */

  /* Create online run ------------------------------------ */
  FairRunOnline* run = new FairRunOnline(source);
  run->SetOutputFile(outputFileName);

#if SCI_TCALPAR || TOFW_TCALPAR || TRIM_TCALPAR || TWIM_TCALPAR
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
  run->AddTask(sciTcalibrator);
#endif

#if TOFW_TCALPAR
  /* Calibrate time-of-flight wall  ---------------------------------------- */
  R3BSofToFWMapped2TcalPar* tofwTcalibrator = new R3BSofToFWMapped2TcalPar("R3BSofToFWMapped2TcalPar");
  tofwTcalibrator->SetNumDetectors(28);
  tofwTcalibrator->SetNumSections(1);
  tofwTcalibrator->SetNumChannels(2);
  tofwTcalibrator->SetNumSignals(28,1,2);
  tofwTcalibrator->SetMinStatistics(1000000);
  run->AddTask(tofwTcalibrator);
#endif

#if TRIM_TCALPAR
  /* Calibrate time-of-flight wall  ---------------------------------------- */
  R3BSofTrimMapped2TcalPar* trimTcalibrator = new R3BSofTrimMapped2TcalPar("R3BSofTrimMapped2TcalPar");
  trimTcalibrator->SetNumDetectors(1);
  trimTcalibrator->SetNumSections(3);
  trimTcalibrator->SetNumChannels(6);
  trimTcalibrator->SetNumSignals(1,3,6);
  trimTcalibrator->SetMinStatistics(1000000);
  run->AddTask(trimTcalibrator);
#endif

#if TWIM_TCALPAR
  /* Calibrate time-of-flight wall  ---------------------------------------- */
  R3BSofTwimMapped2TcalPar* twimTcalibrator = new R3BSofTwimMapped2TcalPar("R3BSofTwimMapped2TcalPar");
  twimTcalibrator->SetNumDetectors(2);
  twimTcalibrator->SetNumSections(2);
  twimTcalibrator->SetNumChannels(16);
  twimTcalibrator->SetNumSignals(2,2,16);
  twimTcalibrator->SetMinStatistics(1000000);
  run->AddTask(twimTcalibrator);
#endif

  /* Runtime data base ------------------------------------ */
  FairRuntimeDb* rtdb = run->GetRuntimeDb();
#if SCI_TCALPAR || TOFW_TCALPAR || TRIM_TCALPAR || TWIM_TCALPAR
  //Choose Root or Ascii file	
#if SAVE_TCALPAR_ROOT
  // Root file with the Calibration Parameters
  Bool_t kParameterMerged = kTRUE;
  FairParRootFileIo* parOut = new FairParRootFileIo(kParameterMerged);
  TString parFileName = "../SofMacrosOutput/201410_Ubeam/Parameters/tcal_VFTX_run1xxx.root";  // name of parameter file	
  parOut->open(parFileName);
  rtdb->setFirstInput(parOut);
  rtdb->setOutput(parOut);
#else
  // Ascii file with the Calibration Parameters
  FairParAsciiFileIo* parIo1 = new FairParAsciiFileIo();
  parIo1->open("../SofMacrosOutput/201410_Ubeam/Parameters/tcal_VFTX_run1xxx.par","out");
  rtdb->setOutput(parIo1);
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
#if TRIM_TCALPAR
  TCanvas * can_SofTrim = new TCanvas("SofTrim_TcalPar","SofTrim_TcalPar",0,0,1000,1000);
  can_SofTrim->Divide(6,3);
  TH1F * h1_softrim_ftns[18];
  for(int section=0; section<trimTcalibrator->GetNumSections(); section++){
    for(int anode=0; anode<trimTcalibrator->GetNumChannels(); anode++){
      signal = section*trimTcalibrator->GetNumChannels() + anode;
      sprintf(name,"TimeFineNs_Trim_P1_S%i_A%i_Sig%i",section+1,anode+1,signal);
      h1_softrim_ftns[signal]=(TH1F*)file->Get(name);
      can_SofTrim->cd(signal+1); gPad->SetGridx(); gPad->SetGridy();
      h1_softrim_ftns[signal]->SetLineWidth(2);
      h1_softrim_ftns[signal]->Draw();
    }
  }
#endif
#if TWIM_TCALPAR
  const char * nameTwimPlane[2] = {"Right","Left"};
  const char * nameTwimSection[2] = {"Down","Up"};
  TCanvas * can_SofTwim[4];
  TH1F * h1_softwim_ftns[18];
  for(int plane=0; plane<twimTcalibrator->GetNumDetectors(); plane++){
    for(int section=0; section<twimTcalibrator->GetNumSections(); section++){
      sprintf(name,"SofTwim_%s_%s_TcalPar",nameTwimPlane[plane],nameTwimSection[section]);
      can_SofTwim[plane*2+section] = new TCanvas(name,name,0,0,1000,1000);
      can_SofTwim[plane*2+section]->Divide(4,4);
      for(int anode=0; anode<twimTcalibrator->GetNumChannels(); anode++){
	signal = 
	  plane*twimTcalibrator->GetNumSections()*twimTcalibrator->GetNumChannels() + 
	  section*twimTcalibrator->GetNumChannels() + 
	  anode;
	sprintf(name,"TimeFineNs_Twim_P%i_S%i_A%i_Sig%i",plane+1,section+1,anode+1,signal);
	h1_softwim_ftns[signal]=(TH1F*)file->Get(name);
	can_SofTwim[plane*2+section]->cd(anode+1); gPad->SetGridx(); gPad->SetGridy();
	h1_softwim_ftns[signal]->SetLineWidth(2);
	h1_softwim_ftns[signal]->Draw();
      }
    }
  }
#endif


}
