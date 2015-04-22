#ifndef __WZdecay_THREEVECTOR_H__
#define __WZdecay_THREEVECTOR_H__ 1

// standard include
#include <iostream>
#include <cmath>


namespace WZdecay {

  class CThreeVector
  {
  protected:
    double m_dX, m_dY, m_dZ;

  public:
    // constructors and destructors
    CThreeVector(double dX=0, double dY=0, double dZ=0)
      :m_dX(dX), m_dY(dY), m_dZ(dZ)
    {
    }

    CThreeVector(const CThreeVector& vector1) = default;

    ~CThreeVector() = default;

    // Getter and Setter

    const double & X() const
    {
      return this->m_dX;
    }

    const double & Y() const
    {
      return this->m_dY;
    }

    const double & Z() const
    {
      return this->m_dZ;
    }

    // physical member functions
    double Magnitude2() const
    {
      return (m_dX * m_dX + m_dY * m_dY + m_dZ * m_dZ);
    }

    double Magnitude() const
    {
      return (std::sqrt(this->Magnitude2()));
    }

    double Abs2() const
    {
      return (this->Magnitude2());
    }

    double Abs() const
    {
      return (this->Magnitude());
    }

    double TransverseRadius2() const
    {
      return (m_dX * m_dX + m_dY * m_dY);
    }

    double TransverseRadius() const
    {
      return (std::sqrt(this->TransverseRadius2()));
    }

    double Rho2() const
    {
      return (this->TransverseRadius2());
    }

    double Rho() const
    {
      return (this->TransverseRadius());
    }

    double Phi() const
    {
      return ((m_dX==0 && m_dY==0)? 0.0 : std::atan2(m_dY, m_dX));
    }

    double Theta() const
    {
      double p = (this->Rho());
      return (p == 0.0 && m_dZ == 0.0 ? 0.0 : std::atan2(p,m_dZ));
    }
    void RotateX(double dAngleYZ)
    {
      double dTempY = m_dY;
      m_dY = std::cos(dAngleYZ)*m_dY - std::sin(dAngleYZ)*m_dZ;
      m_dZ = std::sin(dAngleYZ)*dTempY + std::cos(dAngleYZ)*m_dZ;
    }

    void RotateY(double dAngleXZ)
    {
      double dTempX = m_dX;
      m_dX = std::cos(dAngleXZ)*m_dX - std::sin(dAngleXZ)*m_dZ;
      m_dZ = std::sin(dAngleXZ)*dTempX + std::cos(dAngleXZ)*m_dZ;
    }

    void RotateZ(double dAngleXY)
    {
      double dTempZ = m_dZ;
      m_dX = std::cos(dAngleXY)*m_dX - std::sin(dAngleXY)*m_dY;
      m_dY = std::sin(dAngleXY)*dTempZ + std::cos(dAngleXY)*m_dY;
    }

    // overloading of arithmetic operators
    CThreeVector & operator += (const CThreeVector & vector2)
    {
      m_dX += vector2.m_dX;
      m_dY += vector2.m_dY;
      m_dZ += vector2.m_dZ;
      return *this;
    }

    CThreeVector & operator -= (const CThreeVector & vector2)
    {
      m_dX -= vector2.m_dX;
      m_dY -= vector2.m_dY;
      m_dZ -= vector2.m_dZ;
      return *this;
    }

    CThreeVector & operator *= (const double &dFactor)
    {
      m_dX *= dFactor;
      m_dY *= dFactor;
      m_dZ *= dFactor;
      return *this;
    }

    CThreeVector & operator /= (const double &dFactor)
    {
      m_dX /= dFactor;
      m_dY /= dFactor;
      m_dZ /= dFactor;
      return *this;
    }

    bool operator == (const CThreeVector & vector2)
    {
      bool IsEqual = 1;
      IsEqual *= (fabs(m_dX - vector2.X()) < 0.00001);
      IsEqual *= (fabs(m_dY - vector2.Y()) < 0.00001);
      IsEqual *= (fabs(m_dZ - vector2.Z()) < 0.00001);
      return IsEqual;
    }

    CThreeVector Unit() {
      double norm = this->Abs();
      return CThreeVector((m_dX / norm), (m_dY / norm), (m_dZ / norm));
    }

  }; // end of class CThreeVector

  // streaming operator (has to be outside, since first parameter is stream)
  inline std::ostream & operator << (std::ostream & stream, const CThreeVector & vector)
  {
    stream << "ThreeVector["<<vector.X()<<","<<vector.Y()<<","<<vector.Z() <<"]";
    return stream;
  }

  // scalarprod
  /**
   * @brief Alias of Scalarproduct
   */
  double Dot(const CThreeVector & vector1, const CThreeVector & vector2);
  double Scalarproduct (const CThreeVector& vector1, const CThreeVector& vector2);

  // crossproduct
  CThreeVector Crossproduct(const CThreeVector& vector1, const CThreeVector& vector2);

  // arithmetic operations

  inline CThreeVector operator + (const CThreeVector& vector1, const CThreeVector & vector2)
  {
    return CThreeVector(
      vector1.X() + vector2.X(),
      vector1.Y() + vector2.Y(),
      vector1.Z() + vector2.Z());
  }

  inline CThreeVector operator - (const CThreeVector& vector1, const CThreeVector & vector2)
  {
    return CThreeVector(
      vector1.X() - vector2.X(),
      vector1.Y() - vector2.Y(),
      vector1.Z() - vector2.Z());
  }

  // operations with doubles
  inline CThreeVector operator * (const CThreeVector& vector1, const double& dFactor)
  {
    return CThreeVector(
      vector1.X() * dFactor,
      vector1.Y() * dFactor,
      vector1.Z() * dFactor);
  }

  inline CThreeVector operator / (const CThreeVector& vector1, const double &dFactor)
  {
    return CThreeVector(
      vector1.X() / dFactor,
      vector1.Y() / dFactor,
      vector1.Z() / dFactor);
  }

  inline bool operator == (const CThreeVector& vector1, const CThreeVector& vector2)
  {
    return (vector1.X() == vector2.X()) * (vector1.Y() == vector2.Y()) * (vector1.Z() == vector2.Z());
  }

  inline CThreeVector operator + (const CThreeVector& vector1, const double & dSummand)
  {
    return CThreeVector(
      vector1.X() + dSummand,
      vector1.Y() + dSummand,
      vector1.Z() + dSummand);
  }

  inline CThreeVector operator - (const CThreeVector& vector1, const double & dSummand)
  {
    return vector1 + (-1*dSummand);
  }

  inline CThreeVector operator * (const double& dFactor, const CThreeVector& vector1)
  {
    return CThreeVector(
      vector1.X() * dFactor,
      vector1.Y() * dFactor,
      vector1.Z() * dFactor);
  }

  inline CThreeVector operator / (const double &dFactor, const CThreeVector& vector1)
  {
    return CThreeVector(
      vector1.X() / dFactor,
      vector1.Y() / dFactor,
      vector1.Z() / dFactor);
  }

  inline CThreeVector operator + (const double & dSummand, const CThreeVector& vector1)
  {
    return CThreeVector(
      vector1.X() + dSummand,
      vector1.Y() + dSummand,
      vector1.Z() + dSummand);
  }

  inline CThreeVector operator - (const double & dSummand, const CThreeVector& vector1)
  {
    return vector1 + (-1*dSummand);
  }

}; // end of namespace WZdecay

#endif
