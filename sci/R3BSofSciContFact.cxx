#include "R3BSofSciContFact.h"

#include "FairLogger.h"
#include "FairParAsciiFileIo.h"
#include "FairParRootFileIo.h"
#include "FairRuntimeDb.h"

#include "R3BSofSciRawPosPar.h"

#ifdef NUMBER_OF_SOFSCI_TOF
#include "R3BSofSciRawTofPar.h"
#endif

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
    /** Creates the Container objects with all accepted contexts and adds them fto
     *  the list of containers for the STS library.*/

    FairContainer* p1 = new FairContainer("SofSciRawPosPar", "Sof Sci RawPos Parameters", "SofSciPosParContext");
    p1->addContext("SofSciPosParContext");
    containers->Add(p1);

#ifdef NUMBER_OF_SOFSCI_TOF

    FairContainer* p2 = new FairContainer("SofSciRawTofPar", "Sof Sci RawTof Parameters", "SofSciTofParContext");
    p2->addContext("SofSciTofParContext");
    containers->Add(p2);
#endif
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
#ifdef NUMBER_OF_SOFSCI_TOF
    if (strcmp(name, "SofSciRawTofPar") == 0)
    {
        p = new R3BSofSciRawTofPar(c->getConcatName().Data(), c->GetTitle(), c->getContext());
    }
#endif
    return p;
}

void R3BSofSciContFact::activateParIo(FairParIo* io)
{
    // activates the input/output class for the parameters
    // needed by the Sts
}

ClassImp(R3BSofSciContFact)
