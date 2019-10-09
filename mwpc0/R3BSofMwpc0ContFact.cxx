#include "R3BSofMwpc0ContFact.h"
#include "FairRuntimeDb.h"
#include "FairParRootFileIo.h"
#include "FairParAsciiFileIo.h"
#include "FairLogger.h"
#include "R3BSofMwpc0CalPar.h"

#include "TClass.h"

#include <iostream>
#include <iomanip>

using std::cout;
using std::endl;

static R3BSofMwpc0ContFact gR3BSofMwpc0ContFact;


/* --------- Constructor --------- */
R3BSofMwpc0ContFact::R3BSofMwpc0ContFact () {

  fName="R3BSofMwpc0ContFact";
  fTitle="Factory for parameter containers in libR3BSofMwpc0";
  setAllContainers();
  FairRuntimeDb::instance()->addContFactory(this);

}


/* -------- Create the Containers -------*/
void R3BSofMwpc0ContFact::setAllContainers() {

 FairContainer* cont = new FairContainer("mwpc0CalPar","Sofia Mwpc0 Calibration Parameters","SofMwpc0CalParContext");
 cont->addContext("SofMwpc0CalParContext");
 containers->Add(cont);

}


FairParSet* R3BSofMwpc0ContFact::createContainer(FairContainer* cnt) {

  const char* name= cnt->GetName();
  LOG(INFO) << "R3BSofMwpc0ContFact: Create container name: "<< name;
  FairParSet* par=0;
  if (strcmp(name,"mwpc0CalPar")==0) {

     par= new R3BSofMwpc0CalPar (cnt->getConcatName().Data(), cnt->GetTitle(),cnt->getContext());

  }

  return par;
}

void  R3BSofMwpc0ContFact::activateParIo(FairParIo* io) {


}

ClassImp(R3BSofMwpc0ContFact)
