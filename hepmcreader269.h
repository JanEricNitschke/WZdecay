#ifndef __WZdecay_HEPMCREADER_H__
#define __WZdecay_HEPMCREADER_H__ 1

// standard include
#include <iostream>
#include <vector>
#include "lorentzvector.h"
#include "events.h"
#include "particle.h"
#include "readerbase.h"

namespace HepMC {
  class IO_GenEvent;
  class GenParticle;
}

namespace WZdecay
{
  /**
   * @brief Class reading information from input HepMC file to CEvent class
   */
  class CHepMCReader269 : public ReaderBase {
  private:
    HepMC::IO_GenEvent* m_pioInput;
    double m_dSumOfWeights;
    double m_dCrossSection;
    int m_IndexCurrentFile;
    std::vector<const char*> m_Filenames;


  public:
    /**
     * @brief constructor
     * @param filename name of input file
     */
    CHepMCReader269(const char* filename);

    /**
     * @brief constructor
     * @param filename name of input file
     * @param dCrossSection Crosssection in fb
     */
    CHepMCReader269(const char* filename, double dCrossSection);

    /**
     * @brief constructor
     * @param vector of filenames name of input file
     * @param dCrossSection Crosssection in fb
     */
    CHepMCReader269(std::vector<const char*> filenames, double dCrossSection);

    ~CHepMCReader269();

    /**
     * @brief reads next input event
     * @param evInput Pointer to CEvent to be filled, create object if null-pointer
     * @return true if new event was read, false if end of file
     */
    bool ReadEvent(CEvent*& evInput);

    double SumOfWeights() {
      return m_dSumOfWeights;
    }

    const double CrossSection() const {
      return m_dCrossSection;
    }

    const double CrossSectionError() const {
      return 0;
    }

    const double CrossSectionMax() const {
      return 0;
    }

    void SetCrossSection(double xsection) {
      m_dCrossSection = xsection;
    }

    void AddToSumOfWeights(double dWeight) {
      m_dSumOfWeights += dWeight;
    }

  private:
    /**
     * @brief read-in information about one particle
     */
    CParticle* ReadParticle(HepMC::GenParticle*);

  }; // end of class CHepMCReader

}; // end of namespace WZdecay

#endif