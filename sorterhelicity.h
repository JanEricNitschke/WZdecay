#ifndef __WZdecay_sorterHelicity_H__
#define __WZdecay_sorterHelicity_H__ 1

// standard include
#include <vector>
#include <cassert>
#include "events.h"
#include "sorterbase.h"
#include "particle.h"
#include "logger.h"


namespace WZdecay {

  const CParticle* GetParticleByID(int iPDGID, const std::vector<CParticle> vecParticles);

  bool IsInVector(int iTested, std::vector<int> vecOfIntegers);

  /**
   * @brief abstract Class sort events
   */
  class CSorterHelicity : public aCSorterBase 
  {
  private:
    std::vector<int> m_iWHelicity;
    std::vector<int> m_iZHelicity;

  public:
    CSorterHelicity(std::vector<int> vecWHelicity, std::vector<int> vecZHelicity):
      m_iWHelicity(vecWHelicity),
      m_iZHelicity(vecZHelicity)
      {}


    /**
     * @brief returns true if W boson is longitudinally polarized and Z boson is longitudinally polarized 
     * @warning assumes only one W and Z boson
     * @todo expand to work for WW or ZZ decay 
     * @param event CEvent to be sorted
     */
    bool IsAccepted(CEvent* event)
    {
      logger::CLogger log("SorterHelicity::IsAccepted()");
      if (event == 0) {
        return 0;
      }
      const CParticle*partWBoson = GetParticleByID(24, event->VecParticles() );
      if (partWBoson == 0) {
        partWBoson = GetParticleByID(-24, event->VecParticles() );
      }
      if (partWBoson == 0) {
        log.toLog("No W Boson found in event:",3);
      }
      else {
        if (!IsInVector(partWBoson->Helicity(), m_iWHelicity)){
          return 0;
        }
      }
      const CParticle* partZBoson = GetParticleByID(23, event->VecParticles() );
      if (IsInVector(partZBoson->Helicity(), m_iZHelicity) ) {
        return 1;
      }
      return 0;
    }
  }; // end of class CSorterHelicity

  /**
   * @brief returns particle with given PDG-ID from vector. NullPtr if not included
   */
  const CParticle* GetParticleByID(int iPDGID,const std::vector<CParticle> vecParticles){
    logger::CLogger log("SorterHelicity::GetParticleByID()");
    for (const CParticle& particle : vecParticles ) {
      if (particle.Flavor() == iPDGID) {
        return &particle;
      }
    }
    return nullptr;
  }

  /**
   * @brief checks whether given integer is included in vector
   */
  bool IsInVector(int iTested, std::vector<int> vecOfIntegers) {
    logger::CLogger log("SorterHelicity::GetParticleByID()");
    for (const int &iterator : vecOfIntegers) {
      if (iTested == iterator) {
        return 1;
      }
    }
    return 0;
  }

}; // end of namespace WZdecay

#endif
