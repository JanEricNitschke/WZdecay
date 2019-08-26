#ifndef __WZdecay_READERBASE_H__
#define __WZdecay_READERBASE_H__ 1

// standard include
#include <iostream>
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
  class ReaderBase
  {
  private:
    double m_dSumOfWeights;
    double m_dCrossSection;
    xmlNode* m_lheHeader_readerbase; //@patmasid

  public:
    virtual bool ReadEvent(CEvent*& evInput) = 0;
    virtual double SumOfWeights() = 0;
    virtual const double CrossSection() const = 0;
    virtual const double CrossSectionError() const = 0;
    virtual const double CrossSectionMax() const = 0;
    virtual void SetCrossSection(double xsection) = 0;
    virtual void AddToSumOfWeights(double dWeight) = 0;
    
    virtual xmlNode* Return_lheHeader() //@patmasid
    {
      return m_lheHeader_readerbase;
    }

    virtual void Set_lheHeader(xmlNode* lheHeaderIn) //@patmasid
    {
      m_lheHeader_readerbase = lheHeaderIn;
    }

  }; // end of class ReaderBase

}; // end of namespace WZdecay

#endif
