#ifndef __WZdecay_LHEFREADER_H__
#define __WZdecay_LHEFREADER_H__ 1

// standard include
#include <iostream>
#include <stdexcept>
#include "lorentzvector.h"
#include "events.h"
#include "particle.h"
#include "LHEF.h"
#include "readerbase.h"

namespace WZdecay
{
  /**
   * @brief Class reading information from input HepMC file to CEvent class
   */
  class LHEFReader : public ReaderBase
  {
  private:
    LHEF::LHEFParser* m_pioInput;
    double m_dSumOfWeights;
    double m_dCrossSection;
    double m_dCrossSectionError;
    double m_dCrossSectionMax;
    int m_iEventID;
    int m_IndexCurrentFile;
    std::vector<const char*> m_Filenames;


  public:
    /**
     * @brief constructor
     * @param filename name of input file
     */
    LHEFReader(const char* filename):
      m_dSumOfWeights(0),
      m_iEventID(0),
      m_IndexCurrentFile(0)
      {
        m_pioInput = new LHEF::LHEFParser(filename);
        m_dCrossSection = m_pioInput->Crosssection();
        m_dCrossSectionError = m_pioInput->CrosssectionError();
        m_dCrossSectionMax = m_pioInput->CrosssectionMax();
        m_Filenames = std::vector<const char*> {filename};
      }

    /**
     * @brief constructor
     * @param filename name of input file
     * @param dCrossSection Crosssection in fb
     */
     LHEFReader(const char* filename, double xsection):
       m_dSumOfWeights(0),
       m_dCrossSection(xsection),
       m_iEventID(0),
       m_IndexCurrentFile(0)
       {
         m_pioInput = new LHEF::LHEFParser(filename);
         m_Filenames = std::vector<const char*> {filename};
         m_dCrossSectionError = m_pioInput->CrosssectionError();
         m_dCrossSectionMax = m_pioInput->CrosssectionMax();
       }

     LHEFReader(std::vector<const char*> filenames, double dCrossSection = 0);

     ~LHEFReader()
       {
         delete m_pioInput;
       }

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
      return m_dCrossSectionError;
    }

    const double CrossSectionMax() const {
      return m_dCrossSectionMax;
    }

    void SetCrossSection(double xsection) {
      m_dCrossSection = xsection;
    }

    void AddToSumOfWeights(double dWeight) {
      m_dSumOfWeights += dWeight;
    }

  }; // end of class LHEFReader

}; // end of namespace WZdecay

#endif
