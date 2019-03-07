// standard include
#include <iostream>
#include "lorentzvector.h"
#include "events.h"
#include "particle.h"
#include "LHEF.h"
#include "lhefreader.h"
#include "logger.h"


namespace WZdecay
{
  LHEFReader::LHEFReader(std::vector<const char*> filenames, double xsection):
    m_dSumOfWeights(0),
    m_dCrossSection(xsection),
    m_iEventID(0),
    m_IndexCurrentFile(0)
    {
      m_pioInput = new LHEF::LHEFParser(filenames[0]);
      m_Filenames = filenames;
      if (m_dCrossSection == 0) {
        m_dCrossSection = m_pioInput->Crosssection();
      }
      m_dCrossSectionError = m_pioInput->CrosssectionError();
      m_dCrossSectionMax = m_pioInput->CrosssectionMax();
    }


  bool LHEFReader::ReadEvent(CEvent*& evInput) {
    logger::CLogger log("LHEFReader::ReadEvent()");

    // Create object if evInput is Nullpointer
    if (evInput == 0) {
      evInput = new CEvent(0);
    }


    // Read in next event from file
    while (! m_pioInput->ReadEvent() /* returns 0 if end of file */) {
      log.toLog("Could not read next event",1);
      // if next event could not be read, do:
      int nextIndex = m_IndexCurrentFile + 1;
      if (nextIndex >= m_Filenames.size() ){
        // delete evhepmcInput; // falls keine probleme beim einlesen von mehreren lhefs auftreten, kann diese zeile gelöscht werden
        log.toLog("End of last File",1);
        return 0;
      }
      log.toLog("End of File. Going to next File",1);
      delete m_pioInput;
      m_pioInput = new LHEF::LHEFParser(m_Filenames[nextIndex]);
      m_IndexCurrentFile += 1;
    }

    // Read in ID
    m_iEventID += 1;
    evInput->SetEventID(m_iEventID);

    // Read in energy scale in GeV
    evInput->SetScale(m_pioInput->Eventscale()); 

    // Read in couplings
    evInput->SetAlphaStrong(m_pioInput->QCDCoupling());
    evInput->SetAlphaWeak(m_pioInput->QEDCoupling());

    // Read in particles from GenEvent
    int iNumberOfParticles = m_pioInput->NParticles();

    for (int iter = 0; iter < iNumberOfParticles; iter++) {
      CParticle* particle = new CParticle();

      // fill particle
      particle->SetFlavor(m_pioInput->PDGID()[iter]);
      particle->SetGeneratedMass(m_pioInput->GeneratedMass()[iter]);
      particle->SetColorID1(m_pioInput->ColorID1()[iter]);
      particle->SetColorID2(m_pioInput->ColorID2()[iter]);
      particle->SetMotherID1(m_pioInput->MotherID1()[iter]);
      particle->SetMotherID2(m_pioInput->MotherID2()[iter]);
      particle->SetID(iter+1);

      // different status id conventions
      double status = m_pioInput->Status()[iter];
      //if (status == -1) {
      //  particle->SetStatus(-1);//4
      //}
      //else if (status == 4) {
      //  particle->SetStatus(-1);
      //} 
      //else {
      particle->SetStatus(status);
	//}

      particle->SetHelicity() = m_pioInput->Helicity()[iter];
      particle->SetMomentum() = CLorentzVector(
        m_pioInput->Px()[iter],
        m_pioInput->Py()[iter],
        m_pioInput->Pz()[iter],
        m_pioInput->E()[iter]);
      evInput->AddParticle(*particle);
    }

    // Read in weight of event
    evInput->SetWeight(m_pioInput->Weight());
    this->AddToSumOfWeights(m_pioInput->Weight());

    return 1;
  }
} // end of namespace WZdecay
