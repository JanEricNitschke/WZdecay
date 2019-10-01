#ifndef __WZdecay_LHEFWRITER_H__
#define __WZdecay_LHEFWRITER_H__ 1

// standard include
#include <iostream>
#include "lorentzvector.h"
#include "events.h"
#include "particle.h"
#include "LHEF.h"
#include "writerbase.h"
#include "logger.h"
#include "readerbase.h"

namespace WZdecay
{
  /**
   * @brief Class for writing information from CEvent class to LHEF container
   * @details one object per LHEF file
   */
  class CLHEFWriter : public aCWriterBase {
  private:
    LHEF::LHEFWriter* m_pioOutput;
    ReaderBase* m_pioInput;
    xmlNode* m_lheHeader_Write; //@patmasid 

  public:
    /**
     * @brief constructor
     * @param filename name of output file
     */
    CLHEFWriter(const char* filename, ReaderBase* reader):
      m_pioInput(reader)
      {
	m_pioOutput = new LHEF::LHEFWriter(filename, reader->Return_lheHeader(), "WZdecay"); //@patmasid 
        m_dSumOfWeights = 0;
	m_lheHeader_Write = reader->Return_lheHeader(); //@patmasid 
      }
      xmlNode* Return_lheHeader_write() //@patmasid
      {
        return m_lheHeader_Write;
      }
      
    ~CLHEFWriter()
      {
        delete m_pioOutput;
      }

    /**
     * @brief writes CEvent to LHEF container
     * @param event CEvent to be written
     */
    virtual void WriteEvent(CEvent*& evInput, int Init);

    virtual void WriteCrossSection();

  private:
    /**
     * @brief writes one particle from CEvent to LHEF::LHEFWriter
     */
    void WriteParticle(CParticle*);

  }; // end of class CLHEFWriter

}; // end of namespace WZdecay

#endif
