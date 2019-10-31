void eventDisplay()
{
    FairRunAna* fRun = new FairRunAna();

    FairRuntimeDb* rtdb = fRun->GetRuntimeDb();
    FairParRootFileIo* parIo1 = new FairParRootFileIo();
    parIo1->open("par.root");
    rtdb->setFirstInput(parIo1);
    rtdb->print();

    fRun->SetSource(new FairFileSource("sim.root"));
    fRun->SetSink(new FairRootFileSink("test.root"));

    FairEventManager* fMan = new FairEventManager();
    FairMCTracks* Track = new FairMCTracks("Monte-Carlo Tracks");
    FairMCPointDraw* LandPoints = new FairMCPointDraw("NeulandPoints", kOrange, kFullSquare);

    fMan->AddTask(Track);

    fMan->AddTask(LandPoints);

    fMan->Init();
}
