// standard include
#include <iostream>
#include <vector>
#include <sstream>
#include "lorentzvector.h"
#include "events.h"
#include "particle.h"
#include "HepMC/GenEvent.h"
#include "hepmcreader269.h"
#include "HepMC/IO_GenEvent.h"
#include "logger.h"


namespace WZdecay
{
  /**
   * @brief constructor
   * @param filename name of input file
   */
  CHepMCReader269::CHepMCReader269(const char* filename):
    m_dSumOfWeights(0),
    m_Filenames({}),
    m_IndexCurrentFile(0)
    {
      m_pioInput = new HepMC::IO_GenEvent(filename, std::ios::in);
      // std::vector<const char*> m_Filenames;
    }

  /**
   * @brief constructor
   * @param filename name of input file
   * @param dCrossSection Crosssection in fb
   */
  CHepMCReader269::CHepMCReader269(const char* filename, double xsection):
    m_dSumOfWeights(0),
    m_dCrossSection(xsection),
    m_Filenames({}),
    m_IndexCurrentFile(0)
    {
      m_pioInput = new HepMC::IO_GenEvent(filename, std::ios::in);
    }

  /**
   * @brief constructor
   * @param filename name of input file
   * @param dCrossSection Crosssection in fb
   */
  CHepMCReader269::CHepMCReader269(std::vector<const char*> filenames, double xsection):
    m_dSumOfWeights(0),
    m_dCrossSection(xsection),
    m_IndexCurrentFile(0)
    {
      m_pioInput = new HepMC::IO_GenEvent(filenames[0], std::ios::in);
      m_Filenames = filenames;
    }

  CHepMCReader269::~CHepMCReader269()
    {
      delete m_pioInput;
    }


  bool CHepMCReader269::ReadEvent(CEvent*& evInput)
      {
      logger::CLogger log("CHepMCReader269::ReadEvent()");
      // Create object if evInput is Nullpointer
      if (evInput == 0) {
        evInput = new CEvent(0);
      }

      // Read in GenEvent from file
      HepMC::GenEvent* evhepmcInput = new HepMC::GenEvent();
      while (! m_pioInput->fill_next_event(evhepmcInput)) {
        // if next event could not be read, do:
        int nextIndex = m_IndexCurrentFile + 1;
        if (nextIndex >= m_Filenames.size() ){
          delete evhepmcInput;
          log.toLog("End of last File",1);
          return 0;
        }
        log.toLog("End of File",1);
        // goto lext file
        m_pioInput = new HepMC::IO_GenEvent(m_Filenames[nextIndex], std::ios::in);
        m_IndexCurrentFile += 1;
      }

      // Read in ID
      evInput->SetEventID(evhepmcInput->event_number());


      // Read in energy scale
      evInput->SetScale(evhepmcInput->event_scale());

      // Read in couplings
      evInput->SetAlphaStrong(evhepmcInput->alphaQCD());
      evInput->SetAlphaWeak(evhepmcInput->alphaQED());

      // Read in particles from GenEvent
      for (HepMC::GenEvent::particle_const_iterator iter = evhepmcInput->particles_begin(); iter != evhepmcInput->particles_end(); ++iter)
        {
          evInput->AddParticle(*(ReadParticle(*iter)) );
        }

      // Read in weight of event
      double dInputWeight ;
      if (evhepmcInput->weights().size() == 0) {
        dInputWeight = 1.;
      }
      else {
        dInputWeight = evhepmcInput->weights()[0];
      }
      evInput->SetWeight(dInputWeight);
      this->AddToSumOfWeights(dInputWeight);

      std::stringstream output;
      output << "Number of particles read: " << evInput->VecParticles().size() << std::endl;
      log.toLog(output.str(),1);

      delete evhepmcInput;
      return 1;
    }

  CParticle* CHepMCReader269::ReadParticle(HepMC::GenParticle* partHepMCinput)
    {
      logger::CLogger log("CHepMCReader269::ReadParticle()");
      /**
       * @todo has to be deleted in WriteHepMC class to avoid memory leaks
       */
      CParticle* partNew = new CParticle();
      switch(static_cast<int>(partHepMCinput->polarization().theta()))
      {
       case 0:  partNew->SetHelicity() = 1;
                break;
       
       case 1:  partNew->SetHelicity() = 0;
                break;
       
       case 3:  partNew->SetHelicity() = -1;
      }
      partNew->SetStatus()   = partHepMCinput->status();
      partNew->SetMomentum() = CLorentzVector(
          partHepMCinput->momentum().x(), partHepMCinput->momentum().y(),
          partHepMCinput->momentum().z(), partHepMCinput->momentum().t());

      partNew->SetGeneratedMass(partHepMCinput->generated_mass());

      partNew->SetFlavor()   = partHepMCinput->pdg_id();
      return partNew;

    } 
} // end of namespace WZdecay