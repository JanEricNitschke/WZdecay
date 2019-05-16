#ifndef __WZdecay_HEPMCWRITER_H__
#define __WZdecay_HEPMCWRITER_H__ 1

// standard include
#include <iostream>
#include "lorentzvector.h"
#include "events.h"
#include "particle.h"
#include "HepMC/GenEvent.h"
#include "HepMC/IO_GenEvent.h"
#include "writerbase.h"

namespace WZdecay
{
  /**
   * @brief Class for writing information from CEvent class to HepMC container
   * @details one object per hepmc file
   */
  class CHepMCWriter : public aCWriterBase {
  private:
    HepMC::IO_GenEvent* m_pioOutput;

  public:
    /**
     * @brief constructor
     * @param filename name of output file
     */
    CHepMCWriter(const char* filename) 
      // m_dSumOfWeights(0)
      {
        m_pioOutput = new HepMC::IO_GenEvent(filename, std::ios::out);
        m_dSumOfWeights = 0;
      }

    ~CHepMCWriter()
      {
        delete m_pioOutput;
      }

    /**
     * @brief writes CEvent to HepMC container
     * @param event CEvent to be written
     */
    virtual void WriteEvent(CEvent*& evInput, int Init=0);

    virtual void WriteCrossSection();

  private:
    /**
     * @brief writes one particle from CEvent to HepMC::GenEvent
     */
    HepMC::GenParticle* WriteParticle(CParticle*);

  }; // end of class CHepMCWriter

}; // end of namespace WZdecay

#endif
