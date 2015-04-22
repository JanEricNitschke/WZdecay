#ifndef __WZdecay_sorterbase_H__
#define __WZdecay_sorterbase_H__ 1

// standard include
#include "events.h"

namespace WZdecay {

  /**
   * @brief abstract Class to sort events
   */
  class aCSorterBase 
  {
  private:
    
  public:
    /**
     * @brief returns false if event is sorted out
     * @param event CEvent to be sorted
     */
    virtual bool IsAccepted(CEvent* event) = 0;


  }; // end of class aCSorterBase

}; // end of namespace WZdecay

#endif