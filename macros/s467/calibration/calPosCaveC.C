typedef struct EXT_STR_h101_t
{
  EXT_STR_h101_unpack_t unpack;
  EXT_STR_h101_SOFSCI_onion_t sci;
  EXT_STR_h101_SOFMWPC_onion_t mwpc;
}EXT_STR_h101;

void calPosCaveC()
{
  TStopwatch timer;
  timer.Start();

  const Int_t nev = -1; /* number of events to read, -1 - until CTRL+C */

  // *********************************** //
  // PLEASE COMPLETE THE FOLLOWING LINES //
  // *********************************** //
  const Int_t expId = 467; // select experiment: 444 or 467
  // *********************************** //

  // SofSci detector config, lmd input file -------------------------------
  TString ntuple_options = "RAW";
  TString ucesb_dir = getenv("UCESB_DIR");
  UShort_t NumSofSci ;
  TString filename, upexps_dir, ucesb_path;
  if (expId==444){
    NumSofSci = 1; // s444: PRIMARY BEAM EXP, 1 SofSci at CAVE C ONLY

    //filename = "--stream=lxir123:7803";
    filename = "/lustre/land/202002_s444/lustre/r3b/202002_s444/main004*_0001.lmd";

    upexps_dir = ucesb_dir + "/../upexps/";                      // for local computers
    // upexps_dir = "/u/land/fake_cvmfs/upexps";                 // for lxlandana computers
    // upexps_dir = "/u/land/lynx.landexp/202002_s444/upexps/";  // for lxg computers
    ucesb_path = upexps_dir + "/202002_s444/202002_s444 --allow-errors --input-buffer=100Mi";
  }
  else if(expId==467){
    NumSofSci = 4; // s467: SECONDARY BEAM EXP, 2 at S2, 1 at S8, 1 at CAVE C

    //filename = "--stream=lxir123:7803";
    //filename = "/lustre/land/202002_s467/lustre/r3b/202002_s467/main0341_0001.lmd";
    filename = "/media/audrey/COURGE/SOFIA/ANALYSE/SOFIA3/data/202002_s467/main0238_0001.lmd";

    upexps_dir = ucesb_dir + "/../upexps/";                      // for local computers
    // upexps_dir = "/u/land/fake_cvmfs/upexps";                 // for lxlandana computers
    // upexps_dir = "/u/land/lynx.landexp/202002_s467/upexps/";  // for lxg computers
    ucesb_path = upexps_dir + "/202002_s467/202002_s467 --allow-errors --input-buffer=100Mi";
  }
  else{
    std::cout << "Experiment was not selected" << std::endl;
    gApplication->Terminate();
  }
  ucesb_path.ReplaceAll("//","/");

  // Output file ----------------------------------------------------
  TString outputFileName = "calsofsci_PosCaveC_data.root";

  // Definition of reader --------------------------------------------
  EXT_STR_h101 ucesb_struct;
  R3BUcesbSource* source = new R3BUcesbSource(filename, ntuple_options,ucesb_path, &ucesb_struct, sizeof(ucesb_struct));
  source->AddReader(new R3BUnpackReader((EXT_STR_h101_unpack_t *)&ucesb_struct,offsetof(EXT_STR_h101, unpack)));
  source->AddReader(new R3BSofSciReader((EXT_STR_h101_SOFSCI_t *)&ucesb_struct.sci,offsetof(EXT_STR_h101, sci),NumSofSci));
  source->AddReader(new R3BSofMwpcReader((EXT_STR_h101_SOFMWPC_t*)&ucesb_struct.mwpc, offsetof(EXT_STR_h101, mwpc)));

  // --- ----------------- --- //
  // --- Create online run --- //
  // --- ----------------- --- //
  FairRunOnline* run = new FairRunOnline(source);
  run->SetRunId(1);
  run->SetOutputFile(outputFileName);

  // --- ----------------- --- //
  // --- Runtime data base --- //
  // --- ----------------- --- //
  FairRuntimeDb* rtdb = run->GetRuntimeDb();
 
  // --- ------------------------------------------- --- //
  // --- Input parameters :                          --- //
  // ---  ascii file with the calibration parameters --- //
  // --- ------------------------------------------- --- //
  FairParAsciiFileIo* parIo1 = new FairParAsciiFileIo();//Ascii
  parIo1->open("../parameters/CalibParam.par","in");
  rtdb->setFirstInput(parIo1);
  rtdb->print();

  // --- ----------------- --- //
  // --- Add analysis task --- //
  // --- ----------------- --- //

  // === Mapped 2 Tcal for SofSci === //
  R3BSofSciMapped2Tcal* SofSciMap2Tcal = new R3BSofSciMapped2Tcal();
  run->AddTask(SofSciMap2Tcal);

  // === Tcal 2 SingleTcal for SofSci === //
  R3BSofSciTcal2SingleTcal* SofSciTcal2STcal = new R3BSofSciTcal2SingleTcal();
  run->AddTask(SofSciTcal2STcal);

  // === Mapped 2 Cal Mwpc0 === //
  R3BSofMwpc0Mapped2Cal* MW0Map2Cal = new R3BSofMwpc0Mapped2Cal();
  run->AddTask(MW0Map2Cal);

  // === Cal 2 Hit Mwpc0 === //
  R3BSofMwpc0Cal2Hit* MW0Cal2Hit = new R3BSofMwpc0Cal2Hit();
  run->AddTask(MW0Cal2Hit);

  // === SingleTcal2CalPosCaveC for SofSci === //
  R3BSofSciSingleTcal2CalPosCaveCPar* calcc = new R3BSofSciSingleTcal2CalPosCaveCPar("R3BSofSciSingleTcal2CalPosCaveCPar");
  calcc->SetNumDets(NumSofSci); 
  calcc->SetFitMin(1.0); 
  calcc->SetFitMax(2.0); 
  calcc->SetMinStatistics(10000);
  run->AddTask(calcc);

  // --- ---------- --- //
  // --- Initialize --- //
  // --- ---------- --- //
  run->Init();
  FairLogger::GetLogger()->SetLogScreenLevel("INFO");
  //FairLogger::GetLogger()->SetLogScreenLevel("WARNING");

  // --- ------------------------------------------- --- //
  // --- output parameters :                         --- //
  // ---  ascii file with the calibration parameters --- //
  // --- ------------------------------------------- --- //
  FairParAsciiFileIo* parOut = new FairParAsciiFileIo();
  TString outputFileNamePar = "out_sofsci_calposCaveC.par";
  parOut->open(outputFileNamePar,"out");
  rtdb->setOutput(parOut);
  rtdb->print();

  // --- --- --- //
  // --- Run --- //
  // --- --- --- //
  run->Run((nev < 0) ? nev : 0, (nev < 0) ? 0 : nev);
  rtdb->saveOutput();

  // --- ------- --- //
  // --- Cleanup --- //
  // --- ------- --- //
  timer.Stop();
  Double_t rtime = timer.RealTime();
  Double_t ctime = timer.CpuTime();
  cout << endl << endl;
  cout << "Macro finished succesfully." << endl;
  cout << "Output file is " << outputFileName << endl;
  cout << "Real time " << rtime << " s, CPU time " << ctime << " s"
       << endl << endl;

}
