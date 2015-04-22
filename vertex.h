#ifndef __WZdecay_VERTEX_H__
#define __WZdecay_VERTEX_H__ 1

// standard include
#include <iostream>

namespace WZdecay {

  /**
   * @brief Class to store information about vertices
   */
  class CVertex 
  {
  private:
    

  public:
    // constructors and destructors
    CVertex()
      {}


  }; // end of class CVertex

  /**
   * @brief Streaming operator for CVertex
   */
  // inline std::ostream & operator << (std::ostream & stream, const CVertex & particle)
  // {
  //   stream << "Particle["<< particle.Flavor() <<",Status["<< particle.Status() <<"],"<< particle.Momentum() <<"]";
  //   return stream;
  // }

}; // end of namespace WZdecay

#endif