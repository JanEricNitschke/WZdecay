#ifndef __WZdecay_decaybase_H__
#define __WZdecay_decaybase_H__ 1

// standard include
#include "events.h"
#include "random.h"

namespace WZdecay {

  /**
   * @brief abstract Class to let particles of certain flavor in Event decay
   */
  class aCDecayBase 
  {
  // private:
    
  public:
    virtual void DecayAllPDGID(CEvent* event, CRandom* random) = 0;

  }; // end of class aCDecayBase

}; // end of namespace WZdecay

#endif