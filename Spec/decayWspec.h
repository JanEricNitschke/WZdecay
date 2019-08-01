#ifndef __WZdecay_decayW_H__
#define __WZdecay_decayW_H__ 1
#define _USE_MATH_DEFINES 

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

#include <math.h>

namespace WZdecay {

  /**
   * @brief wrapper class for decay of W boson
   */
  class CDecayW : public aCDecayBase 
   // class CDecayW
  {
  private:
    bool m_isENuAllowed;
    bool m_isMuNuAllowed;
    bool m_isTauNuAllowed;
    bool m_isQQAllowed;
    std::vector<CParticle*> m_vecWBosons;
    double m_dBRENu;
    double m_dBRMuNu;
    double m_dBRTauNu;
    double m_dBRQQ;
    double m_AllowedBranchingRatios;
    CRandom* m_random;

    void FindBosons(CEvent* event) {
      m_vecWBosons = {};
      // for (CParticle particle : event->VecParticles() ) {
      for (int i = 0; i < event->VecParticles().size(); i++ ) {
        CParticle particle = event->SetParticle(i);
        if (particle.Flavor() == 24 && particle.Status() == 1) {
          m_vecWBosons.push_back(&(event->SetParticle(i)));
        }
        if (particle.Flavor() == -24 && particle.Status() == 1) {
          m_vecWBosons.push_back(&(event->SetParticle(i)));
        }
      }
    }

    void FillMomentumUsingAngles(CParticle* particle, double energy, double theta, double phi, double mass = 0) {
      double AbsMomentum = sqrt(energy*energy - mass*mass);
      particle->SetMomentum() = CLorentzVector(
          AbsMomentum * sin(theta) * cos(phi), // x        
          AbsMomentum * sin(theta) * sin(phi), // y
          AbsMomentum * cos(theta), // z
          energy // energy
        );
    }

    void RandomMomentum(CParticle* partParent, CParticle* partChild1, CParticle* partChild2) {
      // fill energy
      double dEnergy = 0.5 * partParent->Momentum().Abs();

      // fill phi (isotropic)
      double dPhi = 2 * M_PI * (m_random->Uniform() - 0.5);

      // fill theta with angular distributions according to helicity
      double dTheta =0;
      if (partParent->Helicity() == 0) {
        while (1) {
          double rdmForCosTheta = (2 * m_random->Uniform() - 1);
          double rdmIfKept = m_random->Uniform();
          if (rdmIfKept < 3.0/4.0 * (1- rdmForCosTheta * rdmForCosTheta)) {
            dTheta = M_PI/2;//acos(rdmForCosTheta);
            break;
          }
        }
      }
      else {
        const int ciSign = (partParent->Flavor() * partParent->Helicity() > 0) ? 1: -1;
        while (1) {
          double rdmForCosTheta = (2 * m_random->Uniform() - 1);
          double rdmIfKept = 2*m_random->Uniform();
          if (rdmIfKept < 3.0/8.0 * (1 + ciSign * rdmForCosTheta) * (1 + ciSign * rdmForCosTheta) ) {
            dTheta = M_PI/2;//acos(rdmForCosTheta);
            break;
          }
        }
      }

      FillMomentumUsingAngles(partChild1, dEnergy, dTheta, dPhi);
      FillMomentumUsingAngles(partChild2, dEnergy, M_PI - dTheta, dPhi + M_PI);
    }

  public:
    explicit CDecayW(bool ENu=1, bool MuNu = 1, bool TauNu =1, bool QQ = 0) :
      m_isENuAllowed(ENu),
      m_isMuNuAllowed(MuNu),
      m_isTauNuAllowed(TauNu),
      m_isQQAllowed(QQ),
      // Numbers have to be rechecked, currently only BRENu is set to whizard output, others are as in PDG
      m_dBRENu(0.111),
      m_dBRMuNu(0.105),
      m_dBRTauNu(0.112),
      m_dBRQQ(0.676)
      {
        m_vecWBosons = {};
        m_AllowedBranchingRatios = m_isENuAllowed * m_dBRENu +
          m_isMuNuAllowed * m_dBRMuNu + 
          m_isTauNuAllowed * m_dBRTauNu + 
          m_isQQAllowed * m_dBRQQ;
      }

    virtual void DecayAllPDGID(CEvent* event, CRandom* random) {
      FindBosons(event);
      m_random = random;


      for (CParticle* pBoson : m_vecWBosons) {
        pBoson->SetStatus(3);
        CParticle* product1 = new CParticle(1, 1, CLorentzVector(), 0);
        CParticle* product2 = new CParticle(1, 1, CLorentzVector(), 0);

        double dWhichChannel = random->Uniform();
        double ScaleToRelative = 1/m_AllowedBranchingRatios;
        if (0 < dWhichChannel && dWhichChannel < m_isENuAllowed* m_dBRENu *ScaleToRelative) {
          if (pBoson->Flavor() > 0) { // W+
            product1->SetFlavor(-11);
            product2->SetFlavor(12);
          }
          else {
           product1->SetFlavor(11);
           product2->SetFlavor(-12);
          }
        }
        if (m_isENuAllowed*m_dBRENu*ScaleToRelative < dWhichChannel &&
          dWhichChannel < (m_isENuAllowed* m_dBRENu + m_isMuNuAllowed* m_dBRMuNu) *ScaleToRelative) {
          if (pBoson->Flavor() > 0) { // W+
            product1->SetFlavor(-13);
            product2->SetFlavor(14);
          }
          else {
           product1->SetFlavor(13);
           product2->SetFlavor(-14);
          }
        }
        if ((m_isENuAllowed* m_dBRENu + m_isMuNuAllowed* m_dBRMuNu)*ScaleToRelative < dWhichChannel &&
          dWhichChannel < (m_isENuAllowed* m_dBRENu + 
              m_isMuNuAllowed* m_dBRMuNu +
              m_isTauNuAllowed* m_dBRTauNu) *ScaleToRelative) {
          if (pBoson->Flavor() > 0) { // W+
            product1->SetFlavor(-15);
            product2->SetFlavor(16);
          }
          else {
           product1->SetFlavor(15);
           product2->SetFlavor(-16);
          }
        }

        RandomMomentum(pBoson, product1, product2);

        // rotate products from W boson rest frame (z equals W direction in lab frame) to labframe
        CRotation rotWDirectionInLabframeToZAxis(CThreeVector(0,0,1), pBoson->Momentum().ThreeVector().Unit());

        CThreeVector v3RotatedMomentumProd1 = rotWDirectionInLabframeToZAxis.Rotate(product1->Momentum().ThreeVector());
        CThreeVector v3RotatedMomentumProd2 = rotWDirectionInLabframeToZAxis.Rotate(product2->Momentum().ThreeVector());
        
        // boost products from W boson rest frame to labframe
        CThreeVector BoostWRestFrameToLabFrame = pBoson->Momentum().BoostVector();

        product1->SetMomentum() = CLorentzVector(v3RotatedMomentumProd1, product1->Momentum().T()).Boost(BoostWRestFrameToLabFrame);
        product2->SetMomentum() = CLorentzVector(v3RotatedMomentumProd2, product2->Momentum().T()).Boost(BoostWRestFrameToLabFrame);

        // set boson as mother particle
        product1->SetMotherID1(pBoson->ID());
        product1->SetMotherID2(pBoson->ID());
        product2->SetMotherID1(pBoson->ID());
        product2->SetMotherID2(pBoson->ID());

        event->AddParticle(*product1);
        event->AddParticle(*product2);
      }
    }


    void ReduceCrossSection(ReaderBase* reader) {
      double xsection = reader->CrossSection();
      xsection *= m_AllowedBranchingRatios;
      reader->SetCrossSection(xsection);
    }
  }; // end of class CDecayW

}; // end of namespace WZdecay

#endif
