#ifndef __WZdecay_randombase_H__
#define __WZdecay_randombase_H__ 1

namespace WZdecay {

  /**
   * @brief abstract interface Class to return random number
   */
  class aCRandomBase 
  {
    
  public:
    virtual double Uniform() = 0;

  }; // end of class aCRandomBase

}; // end of namespace WZdecay

#endif