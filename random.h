#ifndef __WZdecay_random_H__
#define __WZdecay_random_H__ 1

// standard include
#include "randombase.h"
#include <random>

namespace WZdecay {

  /**
   * @brief interface class for random numbers
   */
  class CRandom : public aCRandomBase 
  {
  private:
    double m_dSeed;
    std::mt19937_64 random_engine;
    std::uniform_real_distribution<double> rng;

  public:
    CRandom(double seed=0) :
      m_dSeed(seed)
      {}

    virtual double Uniform() {
      return rng(random_engine);
    } 
  }; // end of class CRandom

}; // end of namespace WZdecay

#endif