#include "R3BSofSciContFact.h"

#include "FairLogger.h"
#include "FairParAsciiFileIo.h"
#include "FairParRootFileIo.h"
#include "FairRuntimeDb.h"

#include "R3BSofSciRawPosPar.h"
//#include "R3BSofSciRawTofPar.h" ///////////////////////// to uncomment when ready

#include "TClass.h"

static R3BSofSciContFact gR3BSofSciContFact;

R3BSofSciContFact::R3BSofSciContFact()
{
    // Constructor (called when the library is loaded)
    fName = "R3BSofSciContFact";
    fTitle = "Factory for Sci parameter containers in libR3BSofSci";
    setAllContainers();
    LOG(INFO) << "Enter in the R3BSofSciContFact constructor";
    FairRuntimeDb::instance()->addContFactory(this);
}

void R3BSofSciContFact::setAllContainers()
{
  /** Creates the Container objects with all accepted contexts and adds them to
   *  the list of containers for the STS library.*/
  
  FairContainer* p1 = new FairContainer("SofSciRawPosPar", "Sof Sci Parameters", "SofSciParContext");
  p1->addContext("SofSciParContext");
  containers->Add(p1);
    
  // /////////////////////// to uncomment when ready
  //FairContainer* p2 = new FairContainer("SofSciRawTofPar", "Sof Sci Parameters", "SofSciParContext");
  //p2->addContext("SofSciParContext");
  //containers->Add(p2);
}

FairParSet* R3BSofSciContFact::createContainer(FairContainer* c)
{
  /** Trals the constructor of the corresponding parameter container.
   * For an actual context, which is not an empty string and not the default context
   * of this container, the name is concatinated with the context. */
  
  const char* name = c->GetName();
  LOG(INFO) << "R3BSofSciContFact: Create container name: " << name;
  FairParSet* p = 0;
  
  if (strcmp(name, "SofSciRawPosPar") == 0)
    {
      p = new R3BSofSciRawPosPar(c->getConcatName().Data(), c->GetTitle(), c->getContext());
    }
  // /////////////////////// to uncomment when ready
  //if (strcmp(name, "SofSciRawTofPar") == 0)
  //{
  //    p = new R3BSofSciRawTofPar(c->getConcatName().Data(), c->GetTitle(), c->getContext());
  //}
  return p;
}

void R3BSofSciContFact::activateParIo(FairParIo* io)
{
    // activates the input/output class for the parameters
    // needed by the Sts
}

ClassImp(R3BSofSciContFact)
