// standard include
#include <iostream>
#include <vector>
#include <string>
#include "lorentzvector.h"
#include "events.h"
#include "particle.h"
#include "LHEF.h"
#include "lhefwriter.h"
#include "logger.h"

namespace WZdecay
{

  void CLHEFWriter::WriteEvent(CEvent*& evInput, int Init)
  {
    logger::CLogger log("CLHEFWriter::WriteEvent");
    if (evInput->CntEvent() == 1) {
      log.toLog("Start evaluating InitBlock", 2);
      m_pioOutput->m_iIDBeam1 = 2212;
      m_pioOutput->m_dEnergyBeam1 = 6500;
      m_pioOutput->m_iPDFSetBeam1 = 10800;

      m_pioOutput->m_iIDBeam2 = 2212;
      m_pioOutput->m_dEnergyBeam2 = 6500;
      m_pioOutput->m_iPDFSetBeam2 = 10800; 
      
      m_pioOutput->m_iWeightingStrategy = 3;
      m_pioOutput->m_iNProcesses = 1;
      if (m_pioInput == 0) {
        log.toLog("Reader is null pointer", 4);
      }
      log.toLog("Next: cross sections", 2);
      m_pioOutput->m_vdCrosssection = std::vector<double>({m_pioInput->CrossSection()});
      log.toLog("Next: error cross sections" + std::to_string(m_pioInput->CrossSection()), 2);
      m_pioOutput->m_vdCrosssectionError = std::vector<double>({m_pioInput->CrossSectionError()});
      log.toLog("Next: max cross sections" + std::to_string(m_pioInput->CrossSectionError()), 2);
      m_pioOutput->m_vdCrosssectionMax = std::vector<double>({m_pioInput->CrossSectionMax()});
      log.toLog("Next: process ID" + std::to_string(m_pioInput->CrossSectionMax()), 2);
      m_pioOutput->m_viProcessID = std::vector<int>({1});
      log.toLog("Done setting all information for init block", 2);
      m_pioOutput->WriteInitBlock();
    }
    else if (Init==1) {
      m_pioOutput->m_iIDBeam1 = 2212;
      m_pioOutput->m_dEnergyBeam1 = 6500;
      m_pioOutput->m_iPDFSetBeam1 = 10800;

      m_pioOutput->m_iIDBeam2 = 2212;
      m_pioOutput->m_dEnergyBeam2 = 6500;
      m_pioOutput->m_iPDFSetBeam2 = 10800; 
      
      m_pioOutput->m_iWeightingStrategy = 3;
      m_pioOutput->m_iNProcesses = 1;
      if (m_pioInput == 0) {
        log.toLog("Reader is null pointer", 4);
      }
      m_pioOutput->m_vdCrosssection = std::vector<double>({m_pioInput->CrossSection()});
      m_pioOutput->m_vdCrosssectionError = std::vector<double>({m_pioInput->CrossSectionError()});
      m_pioOutput->m_vdCrosssectionMax = std::vector<double>({m_pioInput->CrossSectionMax()});
      m_pioOutput->m_viProcessID = std::vector<int>({1});
      m_pioOutput->WriteInitBlock();
    }

    m_pioOutput->m_iIDProcess = 0;
    m_pioOutput->m_dWeight = evInput->Weight();
    m_pioOutput->m_dEventscale = evInput->Scale();
    m_pioOutput->m_dQEDCoupling = evInput->AlphaWeak();
    m_pioOutput->m_dQCDCoupling = evInput->AlphaStrong();
    m_pioOutput->m_iNParticles = evInput->VecParticles().size();
    this->AddToSumOfWeights(evInput->Weight());
    for (int i = 0; i < evInput->VecParticles().size(); i++) {
      CParticle particle = evInput->VecParticles()[i];
      m_pioOutput->m_viPDGID.push_back( particle.Flavor());         
      m_pioOutput->m_viStatus.push_back( particle.Status());        
      m_pioOutput->m_viIDMother1.push_back( particle.MotherID1());  
      m_pioOutput->m_viIDMother2.push_back( particle.MotherID2());  
      m_pioOutput->m_viIDColor1.push_back(particle.ColorID1());     
      m_pioOutput->m_viIDColor2.push_back(particle.ColorID2());     
      m_pioOutput->m_vdPx.push_back(particle.Momentum().X());       
      m_pioOutput->m_vdPy.push_back(particle.Momentum().Y());       
      m_pioOutput->m_vdPz.push_back(particle.Momentum().Z());       
      m_pioOutput->m_vdE.push_back(particle.Momentum().T());        
      if (particle.Flavor()==24 or particle.Flavor()==-24 or particle.Flavor()==25 or particle.Flavor()==23){
      m_pioOutput->m_vdGeneratedMass.push_back(particle.GeneratedMass());
      }
      else {m_pioOutput->m_vdGeneratedMass.push_back(0);}
      m_pioOutput->m_vdLifetime.push_back(0);                        
      m_pioOutput->m_vdHelicity.push_back(particle.Helicity());     
    }

    m_pioOutput->WriteEventBlock();
    // garbage collection
    delete evInput;
    evInput = 0;
  }

  /**
   * @brief Writes crosssection as comment to output file
   */
  void CLHEFWriter::WriteCrossSection() {
    logger::CLogger log("LHEFWriter::WriteCrossSection");
    log.toLog("Not implemented yet",3);
    //std::string output{"Crosssection: "};
    //output.push_back(std::to_string(m_dCrossSection)).push_back("fb. ");
    //m_pioOutput->write_comment(output);
  }

  /**
   * @brief interface from CParticle to GenParticle
   */
  void CLHEFWriter::WriteParticle(CParticle* partInput)
  {
    // const double kdPi = 3.1415926535;
    // HepMC::GenParticle* phepParticle = new HepMC::GenParticle();

    // // set PDG ID
    // phepParticle->set_pdg_id(partInput->Flavor());
    
    // // set momentum
    // HepMC::FourVector fvMomentum(
    //     partInput->Momentum().X(), 
    //     partInput->Momentum().Y(), 
    //     partInput->Momentum().Z(), 
    //     partInput->Momentum().T());
    // phepParticle->set_momentum(fvMomentum);

    // // set status
    // phepParticle->set_status(partInput->Status());

    // // set generated mass
    // phepParticle->set_generated_mass(partInput->GeneratedMass());

    // // set polarization
    // HepMC::Polarization polOutput(0,0);
    // switch (partInput->Helicity())
    // {
    //   case -1:
    //     polOutput.set_theta(kdPi);
    //     break;
    //   case 0:
    //     polOutput.set_theta(0.5*kdPi);
    //     break;
    //   case 1:
    //     polOutput.set_theta(0);
    //     break;
    //   default:
    //     std::cout << "Unexpected Helicity" << std::endl;
    //     HepMC::GenParticle* phepNull = 0;
    //     return phepNull;
    // }
    // phepParticle->set_polarization(polOutput);

    // return phepParticle;
  }


}; // end of namespace WZdecay
