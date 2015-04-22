#ifndef __WZdecay_ROTATION_H__
#define __WZdecay_ROTATION_H__ 1

#include "threevector.h"
#include <math.h>

namespace WZdecay {

  /**
   * @brief Class for rotation of ThreeVectors
   */
  class CRotation 
  {
  private:
    CThreeVector m_v3RotationAxis;
    double m_dAngle;
    

  public:
    // constructors and destructors
    CRotation(CThreeVector from, CThreeVector to)
      {
        m_v3RotationAxis = Crossproduct(from, to).Unit();
        m_dAngle = acos(Dot(from.Unit(), to.Unit()));
      }

    CThreeVector Rotate(CThreeVector v3From) {
      return m_v3RotationAxis * Dot(m_v3RotationAxis, v3From) + cos(m_dAngle) * Crossproduct(Crossproduct(m_v3RotationAxis, v3From), m_v3RotationAxis) + sin(m_dAngle) * Crossproduct(m_v3RotationAxis, v3From);
    }

    const CThreeVector Axis() const {
      return m_v3RotationAxis;
    }

    const double Angle() const {
      return m_dAngle;
    }


  }; // end of class CRotation

}; // end of namespace WZdecay

#endif