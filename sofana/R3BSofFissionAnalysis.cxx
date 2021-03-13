// ----------------------------------------------------------------------
// -----                                                            -----
// -----                     R3BSofFissionAnalysis                  -----
// -----             Created 14/02/21  by J.L. Rodriguez-Sanchez    -----
// ----------------------------------------------------------------------

#include "R3BSofFissionAnalysis.h"
#include "R3BSofTwimHitData.h"
#include "R3BSofMwpcHitData.h"
#include "R3BSofTofWHitData.h"

#include "R3BTGeoPar.h"
#include "R3BSofGladFieldPar.h"

const Double_t c = 29.9792458;

// R3BSofFissionAnalysis: Default Constructor --------------------------
R3BSofFissionAnalysis::R3BSofFissionAnalysis()
    : FairTask("R3BSof fission tracking Analysis", 1)
    , fOffsetAq(0)
    , fFieldCentre(0)
    , fEffLength(0.)
    , fBfield_Glad(0.)
    , fMwpc0HitDataCA(NULL)
    , fTwimHitDataCA(NULL)
    , fMwpc1HitDataCA(NULL)
    , fMwpc2HitDataCA(NULL)
    , fMwpc3HitDataCA(NULL)
    , fTofWHitDataCA(NULL)
    , fTrackingDataCA(NULL)
    , fOnline(kFALSE)
{
}

// R3BSofFissionAnalysis: Standard Constructor --------------------------
R3BSofFissionAnalysis::R3BSofFissionAnalysis(const TString& name, Int_t iVerbose)
    : FairTask(name, iVerbose)
    , fOffsetAq(0)
    , fFieldCentre(0)
    , fEffLength(0.)
    , fBfield_Glad(0.)
    , fMwpc0HitDataCA(NULL)
    , fTwimHitDataCA(NULL)
    , fMwpc1HitDataCA(NULL)
    , fMwpc2HitDataCA(NULL)
    , fMwpc3HitDataCA(NULL)
    , fTofWHitDataCA(NULL)
    , fTrackingDataCA(NULL)
    , fOnline(kFALSE)
{
}

// Virtual R3BSofFissionAnalysis: Destructor
R3BSofFissionAnalysis::~R3BSofFissionAnalysis()
{
    LOG(INFO) << "R3BSofFissionAnalysis: Delete instance";
    if (fMwpc0HitDataCA)
    {
        delete fMwpc0HitDataCA;
    }
    if (fTwimHitDataCA)
    {
        delete fTwimHitDataCA;
    }
    if (fMwpc1HitDataCA)
    {
        delete fMwpc1HitDataCA;
    }
    if (fMwpc2HitDataCA)
    {
        delete fMwpc2HitDataCA;
    }
    if (fMwpc3HitDataCA)
    {
        delete fMwpc3HitDataCA;
    }
    if (fTofWHitDataCA)
    {
        delete fTofWHitDataCA;
    }
    if (fTrackingDataCA)
    {
        delete fTrackingDataCA;
    }
}

// -----   Public method SetParContainers   --------------------------------
void R3BSofFissionAnalysis::SetParContainers()
{
    // Parameter Container
    // Reading softrackingAnaPar from FairRuntimeDb
    FairRuntimeDb* rtdb = FairRuntimeDb::instance();
    
    fGladPar = (R3BSofGladFieldPar*)rtdb->getContainer("GladFieldPar");
    if (!fGladPar)
    {
        LOG(ERROR) << "R3BSofFragmentAnaPar::Init() Couldn't get handle on GladFieldPar container";
    }

    fMw0GeoPar = (R3BTGeoPar*)rtdb->getContainer("Mwpc0GeoPar");
    if (!fMw0GeoPar)
    {
        LOG(ERROR) << "R3BSofFissionAnalysis::SetParContainers() : Could not get access to Mwpc0GeoPar container.";
        return;
    }
    else
        LOG(INFO) << "R3BSofFissionAnalysis::SetParContainers() : Container Mwpc0GeoPar found.";

    fTargetGeoPar = (R3BTGeoPar*)rtdb->getContainer("TargetGeoPar");
    if (!fTargetGeoPar)
    {
            LOG(WARNING)
                << "R3BSofFissionAnalysis::SetParContainers() : Could not get access to TargetGeoPar container.";
        return;
    }
    LOG(INFO) << "R3BSofFissionAnalysis::SetParContainers() : Container TargetGeoPar found.";

    fMw1GeoPar = (R3BTGeoPar*)rtdb->getContainer("Mwpc1GeoPar");
    if (!fMw1GeoPar)
    {
        LOG(ERROR) << "R3BSofFissionAnalysis::SetParContainers() : Could not get access to Mwpc1GeoPar container.";
        return;
    }
    else
        LOG(INFO) << "R3BSofFissionAnalysis::SetParContainers() : Container Mwpc1GeoPar found.";

    fMw2GeoPar = (R3BTGeoPar*)rtdb->getContainer("Mwpc2GeoPar");
    if (!fMw2GeoPar)
    {
        LOG(ERROR) << "R3BSofFissionAnalysis::SetParContainers() : Could not get access to Mwpc2GeoPar container.";
        return;
    }
    else
        LOG(INFO) << "R3BSofFissionAnalysis::SetParContainers() : Container Mwpc2GeoPar found.";

    fMw3GeoPar = (R3BTGeoPar*)rtdb->getContainer("Mwpc3GeoPar");
    if (!fMw3GeoPar)
    {
        LOG(ERROR) << "R3BSofFissionAnalysis::SetParContainers() : Could not get access to Mwpc3GeoPar container.";
        return;
    }
    else
        LOG(INFO) << "R3BSofFissionAnalysis::SetParContainers() : Container Mwpc3GeoPar found.";

    fTofWGeoPar = (R3BTGeoPar*)rtdb->getContainer("TofwGeoPar");
    if (!fTofWGeoPar)
    {
        LOG(ERROR) << "R3BSofFissionAnalysis::SetParContainers() : Could not get access to TofwGeoPar container.";
        return;
    }
    else
        LOG(INFO) << "R3BSofFissionAnalysis::SetParContainers() : Container TofwGeoPar found.";

}

// -----   Private method SetParameter   -----------------------------------
void R3BSofFissionAnalysis::SetParameter() {

   fFieldCentre= fGladPar->GetFieldCentre();
   fEffLength= fGladPar->GetEffectiveLength();
   fBfield_Glad= fGladPar->GetMagneticField();
}

// -----   Public method Init   --------------------------------------------
InitStatus R3BSofFissionAnalysis::Init()
{
    LOG(INFO) << "R3BSofFissionAnalysis: Init tracking analysis at Cave-C";

    // INPUT DATA
    FairRootManager* rootManager = FairRootManager::Instance();
    if (!rootManager)
    {
        return kFATAL;
    }

    fMwpc0HitDataCA = (TClonesArray*)rootManager->GetObject("Mwpc0HitData");
    if (!fMwpc0HitDataCA)
    {
        return kFATAL;
    }
    
    fTwimHitDataCA = (TClonesArray*)rootManager->GetObject("TwimHitData");
    if (!fTwimHitDataCA)
    {
        return kFATAL;
    }   
    
    fMwpc1HitDataCA = (TClonesArray*)rootManager->GetObject("Mwpc1HitData");
    if (!fMwpc1HitDataCA)
    {
        return kFATAL;
    }

    fMwpc2HitDataCA = (TClonesArray*)rootManager->GetObject("Mwpc2HitData");
    if (!fMwpc2HitDataCA)
    {
        return kFATAL;
    }

    fMwpc3HitDataCA = (TClonesArray*)rootManager->GetObject("Mwpc3HitData");
    if (!fMwpc3HitDataCA)
    {
        return kFATAL;
    }

    fTofWHitDataCA = (TClonesArray*)rootManager->GetObject("TofWHitData");
    if (!fTofWHitDataCA)
    {
        return kFATAL;
    }

    // OUTPUT DATA
    fTrackingDataCA = new TClonesArray("R3BSofTrackingData", 2);
    if (!fOnline)
    {
        rootManager->Register("SofTrackingData", "GLAD Tracking Analysis", fTrackingDataCA, kTRUE);
    }
    else
    {
        rootManager->Register("SofTrackingData", "GLAD Tracking Analysis", fTrackingDataCA, kFALSE);
    }

    SetParameter();
    return kSUCCESS;
}

// -----   Public method ReInit   ----------------------------------------------
InitStatus R3BSofFissionAnalysis::ReInit()
{
    SetParContainers();
    SetParameter();
    return kSUCCESS;
}

// -----   Public method Execution   --------------------------------------------
void R3BSofFissionAnalysis::Exec(Option_t* option)
{
    // Reset entries in output arrays, local arrays
    Reset();
    double Length=0.;
    double Brho=0.;
    int pdid[2]={0,0};
    double tof[2]={0.,0.};
    double zf[2]={0.,0.};

    Int_t nHitMwpc0 = fMwpc0HitDataCA->GetEntries();
    Int_t nHitTwim  = fTwimHitDataCA->GetEntries();
    Int_t nHitMwpc1 = fMwpc1HitDataCA->GetEntries();
    Int_t nHitMwpc2 = fMwpc2HitDataCA->GetEntries();
    Int_t nHitMwpc3 = fMwpc3HitDataCA->GetEntries();
    Int_t nHitTofW  = fTofWHitDataCA->GetEntries();

    R3BSofMwpcHitData** HitMwpc0 = new R3BSofMwpcHitData*[nHitMwpc0];
    R3BSofTwimHitData** HitTwim = new R3BSofTwimHitData*[nHitTwim];
    R3BSofMwpcHitData** HitMwpc1 = new R3BSofMwpcHitData*[nHitMwpc1];
    R3BSofMwpcHitData** HitMwpc2 = new R3BSofMwpcHitData*[nHitMwpc2];
    R3BSofMwpcHitData** HitMwpc3 = new R3BSofMwpcHitData*[nHitMwpc3];
    R3BSofTofWHitData** HitTofW = new R3BSofTofWHitData*[nHitTofW];

    TVector3 pos1[2];
    pos1[0].SetXYZ(-100.,0.,0.);
    pos1[1].SetXYZ(-100.,0.,0.);
    TVector3 pos2[2];
    pos2[0].SetXYZ(-100.,0.,0.);
    pos2[1].SetXYZ(-100.,0.,0.);
    TVector3 pos3[2];
    pos3[0].SetXYZ(-1000.,0.,0.);
    pos3[1].SetXYZ(-1000.,0.,0.);

    for (Int_t i = 0; i < nHitTwim; i++)
    {
        HitTwim[i] = (R3BSofTwimHitData*)(fTwimHitDataCA->At(i));
        if(HitTwim[i]->GetSecID()==0 || HitTwim[i]->GetSecID()==1)//Left
          zf[0] = HitTwim[i]->GetZcharge();
        else
          zf[1] = HitTwim[i]->GetZcharge();
    }

    for (Int_t i = 0; i < nHitMwpc1; i++)
    {
        HitMwpc1[i] = (R3BSofMwpcHitData*)(fMwpc1HitDataCA->At(i));
        if(HitMwpc1[i]->GetX()>0.)
          pos1[0].SetXYZ(HitMwpc1[i]->GetX(), HitMwpc1[i]->GetY(), 0.);
        else
          pos1[1].SetXYZ(HitMwpc1[i]->GetX(), HitMwpc1[i]->GetY(), 0.);

        // std::cout<<i<<" "<< HitMwpc1[i]->GetX()<<" "<< HitMwpc1[i]->GetY()<<" "<< pos.Phi() <<std::endl;
    }

    for (Int_t i = 0; i < nHitMwpc2; i++)
    {

        HitMwpc2[i] = (R3BSofMwpcHitData*)(fMwpc2HitDataCA->At(i));

        if(HitMwpc2[i]->GetX()>0.)
          pos2[0].SetXYZ(HitMwpc2[i]->GetX(), HitMwpc2[i]->GetY(), 0.);
        else
          pos2[1].SetXYZ(HitMwpc2[i]->GetX(), HitMwpc2[i]->GetY(), 0.);

        // std::cout<<i<<" "<< HitMwpc2[i]->GetX() <<" "<< HitMwpc2[i]->GetY()<<" "<< pos.Phi() <<std::endl;
    }

    for (Int_t i = 0; i < nHitMwpc3; i++)
    {

        HitMwpc3[i] = (R3BSofMwpcHitData*)(fMwpc3HitDataCA->At(i));
        if(i==0)
          pos3[0].SetXYZ(HitMwpc3[i]->GetX(), HitMwpc3[i]->GetY(), 0.);
        else{
          if(HitMwpc3[i]->GetX()>pos3[0].X()){
          
           pos3[1].SetXYZ(pos3[0].X(), pos3[0].Y(), 0.);
           pos3[0].SetXYZ(HitMwpc3[i]->GetX(), HitMwpc3[i]->GetY(), 0.);
          
          }else{
           pos3[1].SetXYZ(HitMwpc3[i]->GetX(), HitMwpc3[i]->GetY(), 0.);
          }
        
        }

        // std::cout<<i<<" "<< HitMwpc3[i]->GetX() <<" "<< HitMwpc3[i]->GetY()<<" "<< pos.Phi() <<std::endl;
    }

    for (Int_t i = 0; i < nHitTofW; i++)
    {

        HitTofW[i] = (R3BSofTofWHitData*)(fTofWHitDataCA->At(i));
       if(i==0){
           pdid[0] = HitTofW[i]->GetPaddle();
           tof[0] = HitTofW[i]->GetTof();         
       } else{
        if(HitTofW[i]->GetPaddle()>pdid[0]){
           pdid[1] = HitTofW[i]->GetPaddle();
           tof[1] = HitTofW[i]->GetTof();
          }else{
           pdid[1] = pdid[0];
           tof[1] = tof[0];          
           pdid[0] = HitTofW[i]->GetPaddle();
           tof[0] = HitTofW[i]->GetTof();
          }
        }
    }
    
    if(tof[0]>1.0&&zf[0]>1.&&pos1[0].X()>-100.&&pos2[0].X()>-100&&pos3[0].X()>-1000.){
     Length = GetLength(pos1[0].X()/10.,pos2[0].X()/10.,pos3[0].X()/10.);
     Brho = GetBrho(pos1[0].X()/10.,pos2[0].X()/10.,pos3[0].X()/10.);
     //Length = sqrt(Length*Length + pos3.Y()*pos3.Y()/100.);
     double v = Length/tof[0]/c;
     double gamma = 1./sqrt(1.-v*v);
     AddData(zf[0], Brho/v/gamma/3.107, v, Length, Brho, pdid[0]);
    }
    
    if(tof[1]>1.0&&zf[1]>1.&&pos1[1].X()>-100.&&pos2[1].X()>-100&&pos3[1].X()>-1000.){
     Length = GetLength(pos1[1].X()/10.,pos2[1].X()/10.,pos3[1].X()/10.);
     Brho = GetBrho(pos1[1].X()/10.,pos2[1].X()/10.,pos3[1].X()/10.);
     //Length = sqrt(Length*Length + pos3.Y()*pos3.Y()/100.);
     double v = Length/tof[1]/c;
     double gamma = 1./sqrt(1.-v*v);
     AddData(zf[1], Brho/v/gamma/3.107, v, Length, Brho, pdid[1]);
    }

    if (HitMwpc0)
        delete HitMwpc0;
    if (HitMwpc1)
        delete HitMwpc1;
    if (HitMwpc2)
        delete HitMwpc2;
    if (HitMwpc3)
        delete HitMwpc3;
    return;
}

// -----   Private method for length  -----------------------------------------
Double_t R3BSofFissionAnalysis::GetLength(Double_t mw1, Double_t mw2, Double_t mw3)
{
    Double_t length = 0.;
    double L=fEffLength;//cm
    double alpha = -14.*TMath::DegToRad();
    double beta = -20.*TMath::DegToRad();
    double theta = atan((mw2-mw1)/(fMw2GeoPar->GetPosZ() - fMw1GeoPar->GetPosZ()) );

    //std::cout<<mw1<<" "<<mw2<<" "<<fMw1GeoPar->GetPosZ()<<" "<<fMw2GeoPar->GetPosZ()<<std::endl;
    //std::cout<<alpha<<" "<<theta<<std::endl;

    double xc = 1./(1.+tan(alpha)*tan(theta))*(mw1+(fFieldCentre-fMw1GeoPar->GetPosZ())*tan(theta));
    double zc =fFieldCentre-xc*tan(alpha);
    
    TVector3 posc={xc,0.,zc};
    
    //std::cout<<"c: "<<xc<<" "<<zc<<std::endl;
    
    double xb = xc - L/2.*sin(theta)/cos(theta-alpha);
    double zb = zc - L/2.*cos(theta)/cos(theta-alpha);
    
    TVector3 posb={xb,0.,zb};
    
    //std::cout<<"b: "<<xb<<" "<<zb<<std::endl;

    TVector3 ftrans={fMw3GeoPar->GetPosX(),fMw3GeoPar->GetPosY(),fMw3GeoPar->GetPosZ()};
    TRotation frot;
    frot.RotateX(-1.*fMw3GeoPar->GetRotX()*TMath::DegToRad());    
    frot.RotateY(-1.*fMw3GeoPar->GetRotY()*TMath::DegToRad());
    frot.RotateZ(-1.*fMw3GeoPar->GetRotZ()*TMath::DegToRad());
    TVector3 pos;
    pos.SetXYZ(mw3,0.,0.);
    
    auto pos2=frot*pos+ftrans;
    
    //std::cout<<"E: "<<pos2.X()<<" "<<pos2.Y()<<" "<<pos2.Z()<<std::endl;
    
    auto pos3 = pos2-posc;
    
    double eta= atan(pos3.X()/pos3.Z());
    
    //std::cout<<eta<<std::endl;
    
    double xd = xc + L/2.*sin(eta)/cos(eta+alpha);
    double zd = zc + L/2.*cos(eta)/cos(eta+alpha);
    
    TVector3 posd={xd,0.,zd};
    
    //std::cout<<"d: "<<xd<<" "<<zd<<std::endl;
    
    double xf = pos2.X() + (fTofWGeoPar->GetPosZ()-fMw3GeoPar->GetPosZ())*sin(eta)/cos(eta-beta);
    double zf = pos2.Z() + (fTofWGeoPar->GetPosZ()-fMw3GeoPar->GetPosZ())*cos(eta)/cos(eta-beta);
    
    TVector3 posf={xf,0.,zf};
    
    //std::cout<<"f: "<<xf<<" "<<zf<<std::endl;
    
    //double rho = -0.5*L/sin((eta-theta)/2.0)/cos(alpha-(theta+eta)/2.0);
    
    double rho = 0.5*L/sin((theta-eta)/2.0);    
    
    //double omega = 2.0/sin(sqrt( (posd.Z()-posb.Z())*(posd.Z()-posb.Z()) + (posd.X()-posb.X())*(posd.X()-posb.X())   )/2.0/rho);
    
    double omega = 2.0*asin( sqrt( (posd.Z()-posb.Z())*(posd.Z()-posb.Z()) + (posd.X()-posb.X())*(posd.X()-posb.X())   )/2.0/rho);

    length = (zb-fTargetGeoPar->GetPosZ())/cos(theta)+ omega*rho + (posf.Z()-posd.Z())/cos(-eta) + 98.75;
    
    
    //std::cout<<mw2<<" "<<mw3<<" "<< rho <<" "<<omega <<" "<<omega*rho <<" "<<(posd-posb).Mag()<<" "<< fTargetGeoPar->GetPosZ()<<" "<<length<<std::endl;

    return length;// [cm]
}

// -----   Private method for Brho  --------------------------------------------
Double_t R3BSofFissionAnalysis::GetBrho(Double_t mw1, Double_t mw2, Double_t mw3)
{
    Double_t brho = 0.;

    double L=fEffLength;//cm
    double alpha = -14.*TMath::DegToRad();
    double beta = -20.*TMath::DegToRad();
    double theta = atan((mw2-mw1)/(fMw2GeoPar->GetPosZ() - fMw1GeoPar->GetPosZ()) );

    double xc = 1./(1.+tan(alpha)*tan(theta))*(mw1+(fFieldCentre-fMw1GeoPar->GetPosZ())*tan(theta));
    double zc =fFieldCentre-xc*tan(alpha);
    
    TVector3 posc={xc,0.,zc};
    
    //std::cout<<"c: "<<xc<<" "<<zc<<std::endl;
    
    double xb = xc - L/2.*sin(theta)/cos(theta-alpha);
    double zb = zc - L/2.*cos(theta)/cos(theta-alpha);
    
    TVector3 posb={xb,0.,zb};
    
    //std::cout<<"b: "<<xb<<" "<<zb<<std::endl;

    TVector3 ftrans={fMw3GeoPar->GetPosX(),fMw3GeoPar->GetPosY(),fMw3GeoPar->GetPosZ()};
    TRotation frot;
    frot.RotateX(-1.*fMw3GeoPar->GetRotX()*TMath::DegToRad());    
    frot.RotateY(-1.*fMw3GeoPar->GetRotY()*TMath::DegToRad());
    frot.RotateZ(-1.*fMw3GeoPar->GetRotZ()*TMath::DegToRad());

    TVector3 pos;        
    pos.SetXYZ(mw3,0.,0.);
    
    auto pos2=frot*pos+ftrans;
    
    //std::cout<<"E: "<<pos2.X()<<" "<<pos2.Y()<<" "<<pos2.Z()<<std::endl;
    
    auto pos3 = pos2-posc;
    
    double eta= atan(pos3.X()/pos3.Z());
    
    //std::cout<<eta<<std::endl;
    
    double xd = xc + L/2.*sin(eta)/cos(eta+alpha);
    double zd = zc + L/2.*cos(eta)/cos(eta+alpha);
    
    TVector3 posd={xd,0.,zd};
    
    //std::cout<<"d: "<<xd<<" "<<zd<<std::endl;
    
    double xf = pos2.X() + (fTofWGeoPar->GetPosZ()-fMw3GeoPar->GetPosZ())*sin(eta)/cos(eta-beta);
    double zf = pos2.Z() + (fTofWGeoPar->GetPosZ()-fMw3GeoPar->GetPosZ())*cos(eta)/cos(eta-beta);
    
    TVector3 posf={xf,0.,zf};
    
    //std::cout<<"f: "<<xf<<" "<<zf<<std::endl;
    
    double rho = 0.5*L/sin((theta-eta)/2.0);  
    
    brho = rho*fBfield_Glad;  

    return brho/100.;//[mT]
}

// -----   Protected method Finish   ------------------------------------------
void R3BSofFissionAnalysis::Finish() {}

// -----   Protected method FinishEvent   -------------------------------------
void R3BSofFissionAnalysis::FinishEvent() {}

// -----   Public method Reset   ----------------------------------------------
void R3BSofFissionAnalysis::Reset()
{
    LOG(DEBUG) << "Clearing SofTrackingData Structure";
    if (fTrackingDataCA)
        fTrackingDataCA->Clear();
}

// -----   Private method AddData  --------------------------------------------
R3BSofTrackingData* R3BSofFissionAnalysis::AddData(Double_t z,
                                                   Double_t aq,
                                                   Double_t beta,
                                                   Double_t length,
                                                   Double_t brho,
                                                   Int_t paddle)
{
    // It fills the R3BSofTrackingData
    TClonesArray& clref = *fTrackingDataCA;
    Int_t size = clref.GetEntriesFast();
    return new (clref[size]) R3BSofTrackingData(z, aq, beta, length, brho, paddle);
}

ClassImp(R3BSofFissionAnalysis);
