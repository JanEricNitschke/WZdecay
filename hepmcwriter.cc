// standard include
#include <iostream>
#include <vector>
#include <string>
#include "lorentzvector.h"
#include "events.h"
#include "particle.h"
#include "HepMC/GenEvent.h"
#include "hepmcwriter.h"

namespace WZdecay
{

  void CHepMCWriter::WriteEvent(CEvent*& evInput, int Init)
  {
    HepMC::GenEvent* evhepmcOutput = new HepMC::GenEvent();
    evhepmcOutput->use_units(HepMC::Units::GEV, HepMC::Units::MM);
    // add vertex
    HepMC::GenVertex* phepVertex = new HepMC::GenVertex();
    evhepmcOutput->add_vertex(phepVertex);

    // set ID
    evhepmcOutput->set_event_number(evInput->EventID());

    // set energy scale
    evhepmcOutput->set_event_scale(evInput->Scale());

    // set couplings
    evhepmcOutput->set_alphaQCD(evInput->AlphaStrong());
    evhepmcOutput->set_alphaQED(evInput->AlphaWeak());

    // set weight
    evhepmcOutput->weights().push_back(evInput->Weight());
    this->AddToSumOfWeights(evInput->Weight());

    // set particles
    std::vector<HepMC::GenParticle*> vpartIncoming;

    for (CEvent::particle_iterator iter = evInput->particles_begin(); iter != evInput->particles_end(); ++iter) {
      HepMC::GenParticle* phepParticle = WriteParticle(&(*iter));
      phepVertex->add_particle_out(phepParticle);
      if (phepParticle->status() == 4 || phepParticle->status() == -1) {
	if (!(phepParticle->pdg_id() == 2212 || phepParticle->pdg_id() == -2212 || phepParticle->pdg_id() ==  11 || phepParticle->pdg_id() == -11)){
	phepParticle->set_status(-1);
	}
	else {phepParticle->set_status(4);}
        vpartIncoming.push_back(phepParticle);
      }
    }
    if (vpartIncoming.size() != 2) {
      std::cout << "Unexpected Number of incoming particles: " << vpartIncoming.size() << std::endl;
      return;
    }

    evhepmcOutput->set_beam_particles(vpartIncoming[0], vpartIncoming[1]);

    m_pioOutput->write_event(evhepmcOutput);
    // garbage collection
    delete evhepmcOutput; // also deletes all vertices and particles
    delete evInput;
    evInput = 0;
  }

  /**
   * @brief Writes crosssection as comment to output file
   */
  void CHepMCWriter::WriteCrossSection() {
    std::string output{"Crosssection: "};
    output.append(std::to_string(m_dCrossSection)).append("fb. ");
    m_pioOutput->write_comment(output);
  }

  /**
   * @brief interface from CParticle to GenParticle
   */
  HepMC::GenParticle* CHepMCWriter::WriteParticle(CParticle* partInput)
  {
    const double kdPi = 3.1415926535;
    HepMC::GenParticle* phepParticle = new HepMC::GenParticle();

    // set PDG ID
    phepParticle->set_pdg_id(partInput->Flavor());
    
    // set momentum
    HepMC::FourVector fvMomentum(
        partInput->Momentum().X(), 
        partInput->Momentum().Y(), 
        partInput->Momentum().Z(), 
        partInput->Momentum().T());
    phepParticle->set_momentum(fvMomentum);

    // set status
    phepParticle->set_status(partInput->Status());

    // set generated mass
    phepParticle->set_generated_mass(partInput->GeneratedMass());

    // set polarization
    HepMC::Polarization polOutput(0,0);
    switch (partInput->Helicity())
    {
      case -1:
        polOutput.set_theta(kdPi);
        break;
      case 0:
        polOutput.set_theta(0.5*kdPi);
        break;
      case 1:
        polOutput.set_theta(0);
        break;
      default:
        std::cout << "Unexpected Helicity" << std::endl;
        HepMC::GenParticle* phepNull = 0;
        return phepNull;
    }
    phepParticle->set_polarization(polOutput);

    return phepParticle;
  }


}; // end of namespace WZdecay
