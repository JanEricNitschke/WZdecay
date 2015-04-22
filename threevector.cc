
#include "threevector.h"


namespace WZdecay {

  // member functions
  // scalarprod
  double Dot(const CThreeVector & vector1, const CThreeVector & vector2)
  {
    return (vector1.X()*vector2.X() + 
      vector1.Y()*vector2.Y() + 
      vector1.Z()*vector2.Z());
  }

  double Scalarproduct (const CThreeVector& vector1, const CThreeVector& vector2)
  {
    return Dot(vector1, vector2);
  }

  // crossproduct
  CThreeVector Crossproduct(const CThreeVector& vector1, const CThreeVector& vector2)
  {
    double x = vector1.Y()*vector2.Z() - vector1.Z()*vector2.Y();
    double y = vector1.Z()*vector2.X() - vector1.X()*vector2.Z();
    double z = vector1.X()*vector2.Y() - vector1.Y()*vector2.X();
    return CThreeVector(x,y,z);
  }
  

}; // end of namespace WZdecay
