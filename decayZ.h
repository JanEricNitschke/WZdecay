#ifndef __WZdecay_decayZ_H__
#define __WZdecay_decayZ_H__ 1

// standard include
#include "events.h"
#include "decaybase.h"
#include "sorterhelicity.h"
#include "particle.h"
#include "lorentzvector.h"
#include <vector>
#include "random.h"
#include "rotation.h"
#include "readerbase.h"
#include "logger.h"

#include "cmath"

namespace WZdecay {

  /**
   * @brief wrapper class for decay of Z boson
   */
  class CDecayZ : public aCDecayBase 
  {
  private:
    bool m_isEEAllowed;
    bool m_isMuMuAllowed;
    bool m_isTauTauAllowed;
    bool m_isRestAllowed;
    std::vector<CParticle*> m_vecZBosons;
    double m_dBREE;
    double m_dBRMuMu;
    double m_dBRTauTau;
    double m_dBRRest;
    double m_AllowedBranchingRatios;
    CRandom* m_random;

    void FindBosons(CEvent* event) {
      m_vecZBosons = {};
      for (int i = 0; i < event->VecParticles().size(); i++ ) {
        CParticle particle = event->SetParticle(i);
        if (particle.Flavor() == 23 && particle.Status() == 1) {
          m_vecZBosons.push_back(&(event->SetParticle(i)));
        }
      }
    }

    void FillMomentumUsingAngles(CParticle* particle, double energy, double theta, double phi, double mass = 0) {
      double AbsMomentum = sqrt(energy*energy - mass*mass);
      particle->SetMomentum() = CLorentzVector(
          AbsMomentum * sin(theta) * cos(phi), // x
          AbsMomentum * sin(theta) * sin(phi), // y
          AbsMomentum * cos(theta), // z
          energy//energy
        );
    }

    void RandomMomentum(CParticle* partParent, CParticle* partChild1, CParticle* partChild2) {
      // fill energy
      double dEnergy = 0.5 * partParent->Momentum().Abs();

      // fill phi (isotropic)
      double dPhi = 2 * M_PI * (m_random->Uniform() - 0.5);

      // fill theta with angular distributions according to helicity
      double dTheta;
      if (partParent->Helicity() == 0) {
        while (1) {
          double rdmForTheta = M_PI * m_random->Uniform();
          double rdmIfKept = m_random->Uniform();
          if (rdmIfKept < 2/M_PI * sin(rdmForTheta) * sin(rdmForTheta)) {
            dTheta = rdmForTheta;
            break;
          }
        }
      }
      else {
        while (1) {
          double rdmForTheta = M_PI * m_random->Uniform();
          double rdmIfKept = m_random->Uniform();
          if (rdmIfKept < 0.21221 * (1 + cos(rdmForTheta) * cos(rdmForTheta) - partParent->Helicity() * 0.29816*cos(rdmForTheta)) ) {
            dTheta = rdmForTheta;
            break;
          }
        }
      }

      FillMomentumUsingAngles(partChild1, dEnergy, dTheta, dPhi);
      FillMomentumUsingAngles(partChild2, dEnergy, M_PI - dTheta, dPhi + M_PI);
    }

  public:
    explicit CDecayZ(bool EE=1, bool MuMu = 1, bool TauTau =1, bool Rest = 0) :
      m_isEEAllowed(EE),
      m_isMuMuAllowed(MuMu),
      m_isTauTauAllowed(TauTau),
      m_isRestAllowed(Rest),
      // BR from PDG, except BRMuMu which is set to whizard value
      m_dBREE(0.03363),
      m_dBRMuMu(0.03436),
      m_dBRTauTau(0.03370),
      m_dBRRest(0.89901)
      {
        m_vecZBosons = {};
        m_AllowedBranchingRatios = m_isEEAllowed * m_dBREE +
          m_isMuMuAllowed * m_dBRMuMu + 
          m_isTauTauAllowed * m_dBRTauTau + 
          m_isRestAllowed * m_dBRRest;
      }

    virtual void DecayAllPDGID(CEvent* event, CRandom* random) {
      m_random = random;
      FindBosons(event);
      logger::CLogger log("DecayZ::DecayAllPDGID()");

      for (CParticle* pBoson : m_vecZBosons) {
        pBoson->SetStatus(3);
        CParticle* product1 = new CParticle(1, 1, CLorentzVector(), 0);
        CParticle* product2 = new CParticle(1, 1, CLorentzVector(), 0);

        double dWhichChannel = random->Uniform();
        double ScaleToRelative = 1/m_AllowedBranchingRatios;
        if (0 < dWhichChannel && dWhichChannel < m_isEEAllowed* m_dBREE *ScaleToRelative) {
          product1->SetFlavor(11);
          product2->SetFlavor(-11);
        }
        if (m_isEEAllowed*m_dBREE*ScaleToRelative < dWhichChannel &&
          dWhichChannel < (m_isEEAllowed* m_dBREE + m_isMuMuAllowed* m_dBRMuMu) *ScaleToRelative) {
          product1->SetFlavor(13);
          product2->SetFlavor(-13);
        }
        if ((m_isEEAllowed* m_dBREE + m_isMuMuAllowed* m_dBRMuMu)*ScaleToRelative < dWhichChannel &&
          dWhichChannel < (m_isEEAllowed* m_dBREE + 
              m_isMuMuAllowed* m_dBRMuMu +
              m_isTauTauAllowed* m_dBRTauTau) *ScaleToRelative) {
          product1->SetFlavor(15);
          product2->SetFlavor(-15);
        }

        RandomMomentum(pBoson, product1, product2);

        // rotate products from Z boson rest frame (z equals Z direction in lab frame) to labframe
        //CRotation rotZDirectionInLabframeToZAxis(CThreeVector(0,0,1), pBoson->Momentum().ThreeVector().Unit());
        CThreeVector v3RotatedMomentumProd1 = product1->Momentum().ThreeVector();
        CThreeVector v3RotatedMomentumProd2 = product2->Momentum().ThreeVector();
        
        // boost products from W boson rest frame to labframe
        CThreeVector BoostZRestFrameToLabFrame = pBoson->Momentum().BoostVector();

        // set boson as mother particle
        product1->SetMotherID1(pBoson->ID());
        product1->SetMotherID2(pBoson->ID());
        product2->SetMotherID1(pBoson->ID());
        product2->SetMotherID2(pBoson->ID());

        log.toLog("momentum in rest frame first product: x="+std::to_string(v3RotatedMomentumProd1.X()), 1);
        product1->SetMomentum() = CLorentzVector(v3RotatedMomentumProd1, product1->Momentum().T()).Boost(BoostZRestFrameToLabFrame);
        product2->SetMomentum() = CLorentzVector(v3RotatedMomentumProd2, product2->Momentum().T()).Boost(BoostZRestFrameToLabFrame);

        log.toLog("Momentum first product: Px="+std::to_string(product1->Momentum().X()), 1);
        event->AddParticle(*product1);
        event->AddParticle(*product2);
      }
    } // end of method DecayAllPDGID

    void ReduceCrossSection(ReaderBase* reader) {
      double xsection = reader->CrossSection();
      xsection *= m_AllowedBranchingRatios;
      reader->SetCrossSection(xsection);
    }
  }; // end of class CDecayZ

}; // end of namespace WZdecay

#endif
