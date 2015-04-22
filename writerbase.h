#ifndef __WZdecay_BASEWRITER_H__
#define __WZdecay_BASEWRITER_H__ 1

// standard include
#include "events.h"

namespace WZdecay
{
  /**
   * @brief abstract base class for writing information from CEvent class to file
   * @details one object per hepmc file
   */
  class aCWriterBase
  {
  protected:
    double m_dSumOfWeights;

    double m_dCrossSection;

  public:
    /**
     * @brief writes CEvent to file
     * @param event CEvent to be written
     */
    virtual void WriteEvent(CEvent*& event) = 0;

    const double SumOfWeights() const {
      return m_dSumOfWeights;
    }

    void AddToSumOfWeights(double dWeight) {
      m_dSumOfWeights += dWeight;
    }

    const double CrossSection() const {
      return m_dCrossSection;
    }

    void SetCrossSection(double xsection) {
      m_dCrossSection = xsection;
    }

  }; // end of class aCWriterBase

}; // end of namespace WZdecay

#endif