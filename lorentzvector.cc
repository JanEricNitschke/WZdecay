#include "lorentzvector.h"

namespace WZdecay {

  // member functions

    void CLorentzVector::Boost(double dBetaX, double dBetaY, double dBetaZ)
    {
      double dBeta2 = dBetaX*dBetaX + dBetaY*dBetaY + dBetaZ*dBetaZ;
      double dGamma = 1.0/std::sqrt(1.0-dBeta2);
      double dBetaBoost = dBetaX*m_dX + dBetaY*m_dY + dBetaZ*m_dZ;
      double dGamma2  = (dGamma - 1.0)/dBeta2;
      m_dX = m_dX + dGamma2*dBetaBoost*dBetaX + dGamma*dBetaX*m_dT;
      m_dY = m_dY + dGamma2*dBetaBoost*dBetaY + dGamma*dBetaY*m_dT;
      m_dZ = m_dZ + dGamma2*dBetaBoost*dBetaZ + dGamma*dBetaZ*m_dT;
      SetT(dGamma*(m_dT + dBetaBoost));
    }

  double Dot(const CLorentzVector & vector1, const CLorentzVector & vector2)
  {
    return std::sqrt(vector1.T()*vector2.T() - vector1.X()*vector2.X() - vector1.Y()*vector2.Y() - vector1.Z()*vector2.Z());
  }
  
}; //end of namespace WZdecay